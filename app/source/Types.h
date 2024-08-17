#pragma once

#include <QObject>

// class RoomInfo : public QObject
// {
// 	Q_OBJECT

// public:
// 	Q_PROPERTY(QString id MEMBER id CONSTANT)
// 	QString id;
// 	Q_PROPERTY(QString type MEMBER type CONSTANT)
// 	QString type;
// 	Q_PROPERTY(QString rid MEMBER rid CONSTANT)
// 	QString rid;
// 	Q_PROPERTY(QString nick MEMBER nick CONSTANT)
// 	QString nick;
// 	Q_PROPERTY(QString avatar MEMBER avatar CONSTANT)
// 	QString avatar;
// 	Q_PROPERTY(QString category MEMBER category CONSTANT)
// 	QString category;

// 	Q_PROPERTY(bool fav MEMBER fav CONSTANT)
// 	bool fav;
// };

class MetaInfo : public QObject
{
	Q_OBJECT

public:
	Q_PROPERTY(quint64 id MEMBER id CONSTANT)
	quint64 id;
	Q_PROPERTY(QString rid MEMBER rid CONSTANT)
	QString rid;
	Q_PROPERTY(QString type MEMBER type CONSTANT)
	QString type;
	Q_PROPERTY(QString title MEMBER title CONSTANT)
	QString title;
	Q_PROPERTY(QString nick MEMBER nick CONSTANT)
	QString nick;
	Q_PROPERTY(QString avatar MEMBER avatar CONSTANT)
	QString avatar;
	Q_PROPERTY(QString snapshot MEMBER snapshot CONSTANT)
	QString snapshot;
	Q_PROPERTY(QString category MEMBER category CONSTANT)
	QString category;
	Q_PROPERTY(bool follow MEMBER follow CONSTANT)
	bool follow;
	Q_PROPERTY(bool fav MEMBER fav CONSTANT)
	bool fav;
	Q_PROPERTY(QString heat MEMBER heat CONSTANT)
	QString heat;
	Q_PROPERTY(bool live MEMBER live CONSTANT)
	bool live;
	Q_PROPERTY(qint64 startTime MEMBER startTime CONSTANT)
	qint64 startTime;

	explicit MetaInfo(QObject* parent = nullptr)
		: QObject(parent)
		, id(std::numeric_limits<std::uint64_t>::max())
	{

	}
};

class MediaInfo : public QObject
{
	Q_OBJECT

public:
	Q_PROPERTY(QString id MEMBER id CONSTANT)
	QString id;
	Q_PROPERTY(QString type MEMBER type CONSTANT)
	QString type;
	Q_PROPERTY(QString video MEMBER video CONSTANT)
	QString video;
	Q_PROPERTY(QString audio MEMBER audio CONSTANT)
	QString audio;
	Q_PROPERTY(QString subtitle MEMBER subtitle CONSTANT)
	QString subtitle;

	Q_PROPERTY(QString referer MEMBER referer CONSTANT)
	QString referer;
	Q_PROPERTY(QStringList httpHeaders MEMBER httpHeaders CONSTANT)
	QStringList httpHeaders;
	Q_PROPERTY(QStringList playerOptions MEMBER playerOptions CONSTANT)
	QStringList playerOptions;
};
