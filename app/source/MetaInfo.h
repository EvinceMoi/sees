#pragma once

#include <QObject>

class MetaInfo : public QObject
{
	Q_OBJECT

public:
	Q_PROPERTY(QString id MEMBER id CONSTANT)
	QString id;
	Q_PROPERTY(QString type MEMBER type CONSTANT)
	QString type;
	Q_PROPERTY(QString title MEMBER title CONSTANT)
	QString title;
	Q_PROPERTY(QString nick MEMBER nick CONSTANT)
	QString nick;
	Q_PROPERTY(QString avatar MEMBER avatar CONSTANT)
	QString avatar;
	Q_PROPERTY(QString image MEMBER image CONSTANT)
	QString image;
	Q_PROPERTY(QString heat MEMBER heat CONSTANT)
	QString heat;
	Q_PROPERTY(bool live MEMBER live CONSTANT)
	bool live;
	Q_PROPERTY(QString category MEMBER category CONSTANT)
	QString category;
	Q_PROPERTY(qint64 startTime MEMBER startTime CONSTANT)
	qint64 startTime;
};
