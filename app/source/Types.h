#pragma once

// #include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

struct MetaInfo
{
	quint64 id;
	QString rid;
	QString type;
	QString title;
	QString nick;
	QString avatar;
	QString snapshot;
	QString category;
	bool follow;
	bool fav;
	quint64 heat;
	bool live;
	qint64 startTime;
	QString lastUpdate;

	MetaInfo()
		: id(std::numeric_limits<std::uint64_t>::max())
		, fav(false)
	{

	}
};
using MetaInfoPtr = std::shared_ptr<MetaInfo>;

struct MediaInfo
{
	QString type;
	QString rid;
	QString video;
	QString audio;
	QString subtitle;

	QString referer;
	QStringList httpHeaders;
	QStringList playerOptions;
};
using MediaInfoPtr = std::shared_ptr<MediaInfo>;

// class MediaInfo : public QObject
// {
// 	Q_OBJECT

// public:
// 	Q_PROPERTY(QString id MEMBER id CONSTANT)
// 	QString id;
// 	Q_PROPERTY(QString type MEMBER type CONSTANT)
// 	QString type;
// 	Q_PROPERTY(QString video MEMBER video CONSTANT)
// 	QString video;
// 	Q_PROPERTY(QString audio MEMBER audio CONSTANT)
// 	QString audio;
// 	Q_PROPERTY(QString subtitle MEMBER subtitle CONSTANT)
// 	QString subtitle;

// 	Q_PROPERTY(QString referer MEMBER referer CONSTANT)
// 	QString referer;
// 	Q_PROPERTY(QStringList httpHeaders MEMBER httpHeaders CONSTANT)
// 	QStringList httpHeaders;
// 	Q_PROPERTY(QStringList playerOptions MEMBER playerOptions CONSTANT)
// 	QStringList playerOptions;
// };
