#pragma once

#include "SourceProvider.h"

class QNetworkAccessManager;

class DouyuProvider : public SourceProvider
{
	Q_OBJECT
public:
	explicit DouyuProvider(QObject* parent = 0);
	~DouyuProvider();

	QString getType() const {
		return QStringLiteral("douyu");
	}
	QStringList getMatches() const {
		return { "www.douyu.com" };
	}

	void fetchMeta(const QString& in);
	void fetchMedia(const QString& in);

private:
	QNetworkRequest genRequest(const QString& rid, bool mobile) const;

	std::optional<MetaInfo> processMeta(const QByteArray& data);
	void processMedia(const QString& rid, const QByteArray& data);
	void handleMediaInfo(const QString& rid, const QByteArray& data);

private:
	QNetworkAccessManager *nam_;
};
