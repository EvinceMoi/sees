#include "Model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

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
	fav TINYINT DEFAULT 0
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
