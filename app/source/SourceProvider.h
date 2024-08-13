#pragma once

#include <QString>
#include <QObject>
#include <QNetworkRequest>
#include <MetaInfo.h>
#include <MediaInfo.h>

class QNetworkAccessManager;

class SourceProvider : public QObject
{
	Q_OBJECT
public:
	explicit SourceProvider(QObject* parent = nullptr);
	virtual ~SourceProvider();

	virtual QString getType() const = 0;
	virtual QStringList getMatches() const = 0;

	virtual void fetchMeta(const QString& in) = 0;
	virtual void fetchMedia(const QString& in) = 0;

signals:
	void gotMeta(MetaInfo*);
	void gotMedia(MediaInfo*);


protected:
	virtual QString getUserAgent() const {
		static QString ua("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
		return ua;
	};
};
