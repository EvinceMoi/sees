#pragma once

#include "SourceProvider.h"

class QNetworkAccessManager;
class QNetworkReply;

class HuyaProvider : public SourceProvider
{
	Q_OBJECT
public:
	explicit HuyaProvider(QObject* parent = 0);
	~HuyaProvider();

	QString getType() const {
		return QStringLiteral("huya");
	}
	QString getName() const {
		return QStringLiteral("虎牙");
	}
	QStringList getMatches() const {
		return { "www.huya.com" };
	}

	void fetchMeta(const QString& in);
	void fetchMedia(const QString& in);

	void search(const QString& in);

private:
	QNetworkReply* doRequest(const QString& rid);
	std::optional<MetaInfo> processMeta(const QByteArray& data);
	std::optional<MediaInfo> processMedia(const QByteArray& data);

	void processSearch(const QByteArray& data);

private:
	QNetworkAccessManager *nam_;
};
