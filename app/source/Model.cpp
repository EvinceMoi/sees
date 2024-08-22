#include "Model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDir>
#include <QModelIndex>
#include <QDebug>

#include "Types.h"

Model::Model(QObject *parent)
	: QAbstractListModel(parent)
	, follows_()
{
	auto db = QSqlDatabase::addDatabase("QSQLITE");
	QString path = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
	QDir().mkpath(path);
	db.setDatabaseName(path + "/db.sqlite");
	if(!db.open())
		qDebug() << "db open failed:" << db.lastError().text();
}

Model::~Model()
{
	auto db = QSqlDatabase::database();
	if (db.isOpen())
		db.close();
}

void Model::initSchema()
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

void Model::loadFollows()
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

		doLoad(mi);
	}
}

void Model::updateFollow(const MetaInfo& mi)
{
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

	doUpdate(mi);

}

QList<std::pair<QString, QString>> Model::outdated(int duration)
{
	QList<std::pair<QString, QString>> ret; // <type, rid>
	auto sql = QString("select type, rid from follows where lastUpdate <= datetime('now', '-%1 seconds')").arg(duration);
	QSqlQuery query(sql);
	while (query.next()) {
		auto type = query.value(0).toString();
		auto rid = query.value(1).toString();
		ret.append(std::make_pair(type, rid));
	}
	return ret;
}

QHash<int, QByteArray> Model::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[IdRole] = "id";
	roles[RidRole] = "rid";
	roles[TypeRole] = "type";
	roles[TitleRole] = "title";
	roles[NickRole] = "nick";
	roles[AvatarRole] = "avatar";
	roles[SnapshotRole] = "snapshot";
	roles[CategoryRole] = "category";
	roles[FollowRole] = "follow";
	roles[FavRole] = "fav";
	roles[HeatRole] = "heat";
	roles[LiveRole] = "live";
	roles[StartTimeRole] = "startTime";
	roles[LastUpdateRole] = "lastUpdate";
	return roles;
}

QVariant Model::data(const QModelIndex &idx, int role) const
{
	if (!idx.isValid()) return {};
	auto n = idx.row();
	if (n >= follows_.size()) return {};
	auto it = follows_[n];
	switch (role) {
		case IdRole: return it.id;
		case RidRole: return it.rid;
		case TypeRole: return it.type;
		case TitleRole: return it.title;
		case NickRole: return it.nick;
		case AvatarRole: return it.avatar;
		case SnapshotRole: return it.snapshot;
		case CategoryRole: return it.category;
		case FollowRole: return it.follow;
		case FavRole: return it.fav;
		case HeatRole: return it.heat;
		case LiveRole: return it.live;
		case StartTimeRole: return it.startTime;
		case LastUpdateRole: return it.lastUpdate;
	}
	return {};
}

int Model::rowCount(const QModelIndex &) const
{
	return follows_.size();
}

void Model::doLoad(const MetaInfo& mi)
{
	auto size = follows_.size();
	beginInsertRows(QModelIndex(), size, size);
	follows_.append(mi);
	endInsertRows();
}


void Model::doUpdate(const MetaInfo& mi)
{
	QList<int> roles = {
		TitleRole,
		NickRole,
		AvatarRole,
		SnapshotRole,
		CategoryRole,
		FollowRole,
		FavRole,
		HeatRole,
		LiveRole,
		StartTimeRole
	};
	auto it = std::find_if(follows_.begin(), follows_.end(), [mi](auto& it) {
		return it.type == mi.type && it.rid == mi.rid;
	});
	if (it == follows_.end()) {
		follows_.append(mi);
		follows_.last().follow = true;
		auto idx = createIndex(follows_.size() - 1, 0);
		emit dataChanged(idx, idx, roles);
	} else {
		auto id = it->id;
		*it = mi;
		it->id = id;
		it->lastUpdate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		qDebug() << "last update:" << it->lastUpdate;
		auto idx = createIndex(std::distance(follows_.begin(), it), 0);
		emit dataChanged(idx, idx, roles);
	}
	qDebug() << "update rid:" << mi.rid << ", type:" << mi.type;
}
