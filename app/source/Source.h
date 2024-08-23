#pragma once

#include <QObject>
#include <mutex>
#include <QList>
#include "Types.h"

class SourceProvider;
class MetaModel;

class Source : public QObject
{
	Q_OBJECT
public:
	explicit Source(QObject* parent = 0);
	~Source();

	static Source* self();

	Q_INVOKABLE void getMetaInfo(const QString& type, const QString& ref); // ("douyu", "roomid / url")
	Q_INVOKABLE void getMediaInfo(const QString& type, const QString& ref);

	Q_INVOKABLE MetaModel* follows();

	Q_INVOKABLE void refresh(int gap);

	Q_INVOKABLE void search(const QString& type, const QString& kw);

signals:
	void mediaInfoFetched(const QString& video, const QString& audio, const QString& subtitle);

private:
	void registerProviders();
	void initSchema();

	void loadFollows();
	void updateFollow(const MetaInfo& mi);
	void refreshOutdated(int duration);

private:
	using SourceProviderPtr = std::shared_ptr<SourceProvider>;
	std::optional<SourceProviderPtr> getProvider(const QString& type);

private:
	std::unordered_map<QString, SourceProviderPtr> mtype_;
	std::unordered_map<QString, SourceProviderPtr> mmatch_;

	static std::once_flag once_flag_;

	QList<MetaInfo> follows_;
	QList<MetaInfo> search_;

	MetaModel* followModel_;
	MetaModel* searchModel_;
};
