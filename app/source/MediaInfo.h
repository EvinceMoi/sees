#pragma once

#include <QObject>

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
