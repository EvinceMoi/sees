#include "Source.h"
#include <ranges>

#include "Model.h"
#include "DouyuProvider.h"
#include "HuyaProvider.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QStandardPaths>

#include <QDebug>


std::once_flag Source::once_flag_;

Source::Source(QObject* parent)
	: QObject(parent)
	, followModel_(new MetaModel(follows_, this))
	, followProxyModel_(new MetaModelProxy(this))
	, searchModel_(new MetaModel(search_, this))
{
	followProxyModel_->setSourceModel(followModel_);

	auto db = QSqlDatabase::addDatabase("QSQLITE");
	QString path = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
	QDir().mkpath(path);
	db.setDatabaseName(path + "/db.sqlite");
	if(!db.open())
		qDebug() << "db open failed:" << db.lastError().text();
}

Source::~Source()
{
	auto db = QSqlDatabase::database();
	if (db.isOpen())
		db.close();
}

Source* Source::self()
{
	static Source s;
	std::call_once(once_flag_, [&](){
		s.initSchema();
		s.registerProviders();
		s.loadFollows();
	});
	return &s;
}

void Source::follow(const QString &type, const QString &rid, bool f)
{
	if (!f) {
		followModel_->remove(type, rid);
		followProxyModel_->invalidate();
		dbRemoveFollow(type, rid);
		return;
	}
	MetaInfo mi;
	mi.type = type;
	mi.rid = rid;
	if (dbSaveFollow(mi))
		fetchMeta(type, rid);
}

void Source::fav(const QString &type, const QString &rid, bool f)
{
	dbSetFav(type, rid, f);
	followModel_->update(type, rid, [&](MetaInfo& mi){
		mi.fav = f;
	});
	followProxyModel_->invalidate();
}

void Source::registerProviders()
{
	auto doRegister = [this](auto it) {
		mtype_.insert({it->getType(), it});
	};
	doRegister(std::make_shared<DouyuProvider>());
	doRegister(std::make_shared<HuyaProvider>());

	for (auto &[k, sp]: mtype_) {
		connect(sp.get(), &SourceProvider::gotMeta, this, &Source::onMeta);
		connect(sp.get(), &SourceProvider::gotMedia, [this](const MediaInfo& mi) {
			emit mediaInfoFetched(mi.video, mi.audio, mi.subtitle);
		});
		connect(sp.get(), &SourceProvider::searchResult, [this](const QList<MetaInfo>& s) {
			search_.clear();
			std::copy(s.begin(), s.end(), std::back_inserter(search_));
			searchModel_->reset();
		});
	}
}

void Source::initSchema()
{
	static const QString sqlCreateFollows = R"(
create table if not exists follows(
	id INTEGER PRIMARY KEY,
	type TEXT NOT NULL,
	rid TEXT NOT NULL,
	nick TEXT,
	avatar TEXT,
	category TEXT,
	fav TINYINT DEFAULT 0,
	title TEXT,
	snapshot TEXT,
	lastUpdate DATETIME DEFAULT CURRENT_TIMESTAMP,
	unique(type, rid) on conflict replace
);
)";
	QSqlQuery sql;
	bool ok = sql.exec(sqlCreateFollows);
	qDebug() << "create table follows:" << ok;
	if (!ok) {
		qDebug() << sql.lastQuery();
		qDebug() << sql.lastError();
	}
}

void Source::loadFollows()
{
	follows_.clear();

	QSqlQuery query("select * from follows order by fav desc");
	auto record = query.record();
	auto idx_id = record.indexOf("id");
	auto idx_type = record.indexOf("type");
	auto idx_rid = record.indexOf("rid");
	auto idx_nick = record.indexOf("nick");
	auto idx_avatar = record.indexOf("avatar");
	auto idx_category = record.indexOf("category");
	auto idx_fav = record.indexOf("fav");
	auto idx_title = record.indexOf("title");
	auto idx_snapshot = record.indexOf("snapshot");
	auto idx_last_update = record.indexOf("lastUpdate");
	while (query.next()) {
		quint64 id = query.value(idx_id).toULongLong();
		QString type = query.value(idx_type).toString();
		QString rid = query.value(idx_rid).toString();
		QString nick = query.value(idx_nick).toString();
		QString avatar = query.value(idx_avatar).toString();
		QString category = query.value(idx_category).toString();
		bool fav = query.value(idx_fav).toInt() == 1;
		QString title = query.value(idx_title).toString();
		QString snapshot = query.value(idx_snapshot).toString();
		QString last_update = query.value(idx_last_update).toString();

		MetaInfo mi;
		mi.id = id;
		mi.type = type;
		mi.rid = rid;
		mi.nick = nick;
		mi.avatar = avatar;
		mi.category = category;
		mi.follow = true;
		mi.fav = fav;
		mi.title = title;
		mi.snapshot = snapshot;
		mi.lastUpdate = last_update;

		follows_.append(mi);
		followModel_->changeAtIndex(follows_.size() - 1);
	}
	followProxyModel_->invalidate();
}


void Source::refreshOutdated(int duration)
{
	duration = std::max(0, duration);
	auto sql = QString("select type, rid from follows where lastUpdate <= datetime('now', '-%1 seconds')").arg(duration);
	QSqlQuery query(sql);
	while (query.next()) {
		auto type = query.value(0).toString();
		auto rid = query.value(1).toString();
		qDebug() << "refresh" << type << ">" << rid;
		fetchMeta(type, rid);
	}
}


std::optional<Source::SourceProviderPtr> Source::getProvider(const QString &type)
{
	if (!mtype_.contains(type)) return {};
	return mtype_[type];
}

void Source::fetchMeta(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMeta(ref);
}

void Source::fetchMedia(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMedia(ref);
}

MetaModelProxy* Source::followsModel()
{
	return followProxyModel_;
}

MetaModel *Source::searchModel()
{
	return searchModel_;
}

void Source::refresh(int gap)
{
	refreshOutdated(gap);
}

void Source::search(const QString &type, const QString &kw)
{
	getProvider(type).and_then([kw](auto sp) -> std::optional<SourceProviderPtr> {
		sp->search(kw);
		return sp;
	});
}

void Source::filterFollows(const QString &kw)
{
	followProxyModel_->search(kw);
}

QStringList Source::getTypes()
{
	auto kv = std::views::keys(mtype_);
	return QStringList{kv.begin(), kv.end()};
}

QString Source::getNameByType(const QString &type)
{
	auto it = getProvider(type).and_then([](auto sp) {
		return std::optional<QString>(sp->getName());
	});
	return it ? it.value() : QString{};
}

bool Source::dbSaveFollow(const MetaInfo &mi)
{
	// db
	QSqlQuery query;
	query.prepare("insert into follows (type, rid, nick, avatar, category, title, snapshot) "
				  "values (:type, :rid, :nick, :avatar, :catetory, :title, :snapshot) "
				  "on conflict(type, rid) "
				  "do update set nick = :nick, avatar = :avatar, category = :category, "
				  "title = :title, snapshot = :snapshot, lastUpdate = CURRENT_TIMESTAMP");
	query.bindValue(":type", mi.type);
	query.bindValue(":rid", mi.rid);
	query.bindValue(":nick", mi.nick);
	query.bindValue(":avatar", mi.avatar);
	query.bindValue(":category", mi.category);
	query.bindValue(":title", mi.title);
	query.bindValue(":snapshot", mi.snapshot);
	bool ok = query.exec();
	if (!ok) {
		qDebug() << "failed to update follow, type:" << mi.type << ", rid:" << mi.rid << ", nick:" << mi.nick;
		qDebug() << query.lastError().text();
		return false;
	}
	return true;
}

bool Source::dbRemoveFollow(const QString &type, const QString &rid)
{
	QSqlQuery query;
	query.prepare("delete from follows where type = :type and rid = :rid");
	query.bindValue(":type", type);
	query.bindValue(":rid", rid);
	bool ok = query.exec();
	if (!ok) {
		qDebug() << "failed to remove follow: type:" << type << ", rid:" << rid;
		return false;
	}
	return true;
}

bool Source::dbSetFav(const QString &type, const QString &rid, bool f)
{
	QSqlQuery query;
	query.prepare("update follows set fav = :fav where type = :type and rid = :rid");
	query.bindValue(":type", type);
	query.bindValue(":rid", rid);
	query.bindValue(":fav", f ? 1 : 0);
	bool ok = query.exec();
	if (!ok) {
		qDebug() << "failed to update follow fav: type:" << type << ", rid:" << rid;
		return false;
	}
	return true;
}

std::optional<MetaInfo> Source::dbGetFollow(const QString &type, const QString &rid)
{
	QSqlQuery query;
	query.prepare("select * from follows where type = :type and rid = :rid");
	query.bindValue(":type", type);
	query.bindValue(":rid", rid);
	bool ok = query.exec();
	if (!ok) {
		qDebug() << "failed to get type:" << type << ", rid:" << rid;
		return {};
	}
	if (query.next()) {
		quint64 id = query.value("id").toULongLong();
		QString type = query.value("type").toString();
		QString rid = query.value("rid").toString();
		QString nick = query.value("nick").toString();
		QString avatar = query.value("avatar").toString();
		QString category = query.value("category").toString();
		bool fav = query.value("fav").toInt() == 1;
		QString title = query.value("title").toString();
		QString snapshot = query.value("snapshot").toString();
		QString last_update = query.value("lastUpdate").toString();

		MetaInfo mi;
		mi.id = id;
		mi.type = type;
		mi.rid = rid;
		mi.nick = nick;
		mi.avatar = avatar;
		mi.category = category;
		mi.follow = true;
		mi.fav = fav;
		mi.title = title;
		mi.snapshot = snapshot;
		mi.lastUpdate = last_update;
		return mi;
	} else {
		return {};
	}
}

void Source::onMeta(const MetaInfo &mi)
{
	// save to db
	bool ok = dbSaveFollow(mi);
	if (!ok)
		return;

	// load from db and update mem
	dbGetFollow(mi.type, mi.rid).and_then([this, mi](MetaInfo m) {
		m.live = mi.live;
		m.heat = mi.heat;
		m.startTime = mi.startTime;
		followModel_->update(m);
		followProxyModel_->invalidate();
		return std::optional<MetaInfo>(m);
	});
}
