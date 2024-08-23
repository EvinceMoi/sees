#include "Source.h"

#include "Model.h"
#include "DouyuProvider.h"

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
	, searchModel_(new MetaModel(search_, this))
{
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

void Source::registerProviders()
{
	{
		// douyu
		auto it = std::make_shared<DouyuProvider>();
		mtype_.insert({it->getType(), it});
		for (auto &m : it->getMatches()) {
			mmatch_.insert({m, it});
		}
	}

	for (auto &[k, sp]: mtype_) {
		connect(sp.get(), &SourceProvider::gotMeta, [this](const MetaInfo& mi){
			// got meta
			updateFollow(mi);
		});
		connect(sp.get(), &SourceProvider::gotMedia, [this](const MediaInfo& mi){
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
		bool fav = query.value(idx_fav).toBool();
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
}

void Source::updateFollow(const MetaInfo &mi)
{
	// db
	QSqlQuery query;
	query.prepare("insert into follows (type, rid, nick, avatar, category, title, snapshot) "
				  "values (:type, :rid, :nick, :avatar, :catetory, :title, :snapshot) "
				  "on conflict(type, rid) "
				  "do update set nick = :nick, avatar = :avatar, category = :category, fav = :fav, "
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
		return;
	}

	// mem
	{
		auto it = std::find_if(follows_.begin(), follows_.end(), [mi](auto& it) {
			return it.type == mi.type && it.rid == mi.rid;
		});
		if (it == follows_.end()) {
			follows_.append(mi);
			follows_.last().follow = true;
			followModel_->changeAtIndex(follows_.size() - 1);
		} else {
			auto idx = std::distance(follows_.begin(), it);
			auto id = it->id;
			*it = mi;
			it->id = id;
			it->lastUpdate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
			followModel_->changeAtIndex(idx);
		}
	}
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
		getMetaInfo(type, rid);
	}
}


std::optional<Source::SourceProviderPtr> Source::getProvider(const QString &type)
{
	if (!mtype_.contains(type)) return {};
	return mtype_[type];
}

void Source::getMetaInfo(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMeta(ref);
}

void Source::getMediaInfo(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMedia(ref);
}

MetaModel* Source::follows()
{
	return followModel_;
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
