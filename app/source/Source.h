#pragma once

#include <QObject>
#include <mutex>
#include <QList>

class MetaInfo;
class MediaInfo;
class SourceProvider;
class QNetworkAccessManager;

class Source : public QObject
{
	Q_OBJECT
public:
	explicit Source(QObject* parent = 0);
	~Source();

	static Source* self();

	Q_INVOKABLE void getMetaInfo(const QString& type, const QString& ref); // ("douyu", "roomid / url")
	Q_INVOKABLE void getMediaInfo(const QString& type, const QString& ref);

signals:
	void gotMedia(MediaInfo* mi);


private:
	void registerProviders();
	void roomUpsert(MetaInfo *);

private:
	using SourceProviderPtr = std::shared_ptr<SourceProvider>;
	std::unordered_map<QString, SourceProviderPtr> mtype_;
	std::unordered_map<QString, SourceProviderPtr> mmatch_;

	static std::once_flag once_flag_;

	// QNetworkAccessManager *nam_;
	QList<MetaInfo *> rooms_;
};
