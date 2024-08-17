#include "Model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

#include "Types.h"

Model::Model(QObject *parent)
	: QObject(parent)
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
	while (query.next()) {
		quint64 id = query.value(idx_id).toULongLong();
		QString type = query.value(idx_type).toString();
		QString rid = query.value(idx_rid).toString();
		QString nick = query.value(idx_nick).toString();
		QString avatar = query.value(idx_avatar).toString();
		QString category = query.value(idx_category).toString();
		bool fav = query.value(idx_fav).toBool();

		MetaInfo *mi = new MetaInfo;
		mi->id = id;
		mi->type = type;
		mi->rid = rid;
		mi->nick = nick;
		mi->avatar = avatar;
		mi->category = category;
		mi->follow = true;
		mi->fav = fav;

		follows_.append(mi);
	}
}

void Model::updateFollow(MetaInfo* mi)
{
	QSqlQuery query;
	query.prepare("insert into follows (type, rid, nick, avatar, category) "
				  "values (:type, :rid, :nick, :avatar, :catetory) "
				  "on conflict(type, rid) "
				  "do update set nick = :nick, avatar = :avatar, category = :category, fav = :fav");
	query.bindValue(":type", mi->type);
	query.bindValue(":rid", mi->rid);
	query.bindValue(":nick", mi->nick);
	query.bindValue(":avatar", mi->avatar);
	query.bindValue(":category", mi->category);
	bool ok = query.exec();
	if (!ok) {
		qDebug() << "failed to update follow, type:" << mi->type << ", rid:" << mi->rid << ", nick:" << mi->nick;
		qDebug() << query.lastError().text();
		return;
	}

	auto it = std::find_if(follows_.begin(), follows_.end(), [mi](auto it) {
		return it->type == mi->type && it->rid == mi->rid;
	});
	if (it == follows_.end()) {
		mi->follow = true;
		follows_.append(mi);
	} else {
		auto id = (*it)->id;
		*it = mi;
		(*it)->id = id;
	}

}

QList<MetaInfo *> Model::data()
{
	return follows_;
}
