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

	Q_INVOKABLE void follow(const QString& type, const QString& rid, bool f);
	Q_INVOKABLE void fav(const QString& type, const QString& rid, bool f);

	Q_INVOKABLE void fetchMeta(const QString& type, const QString& ref); // ("douyu", "roomid / url")
	Q_INVOKABLE void fetchMedia(const QString& type, const QString& ref);

	Q_INVOKABLE MetaModel* followsModel();
	Q_INVOKABLE MetaModel* searchModel();

	Q_INVOKABLE void refresh(int gap);

	Q_INVOKABLE void search(const QString& type, const QString& kw);

public:
	bool dbSaveFollow(const MetaInfo& mi);
	bool dbRemoveFollow(const QString& type, const QString& rid);
	bool dbSetFav(const QString& type, const QString& rid, bool f);
	std::optional<MetaInfo> dbGetFollow(const QString& type, const QString& rid);

	void refreshOutdated(int duration);

signals:
	void mediaInfoFetched(const QString& video, const QString& audio, const QString& subtitle);

private:
	void onMeta(const MetaInfo& mi);

private:
	void registerProviders();
	void initSchema();

	void loadFollows();

private:
	using SourceProviderPtr = std::shared_ptr<SourceProvider>;
	std::optional<SourceProviderPtr> getProvider(const QString& type);

private:
	std::unordered_map<QString, SourceProviderPtr> mtype_;
	std::unordered_map<QString, SourceProviderPtr> mname_;
	std::unordered_map<QString, SourceProviderPtr> mmatch_;

	static std::once_flag once_flag_;

	QList<MetaInfo> follows_;
	QList<MetaInfo> search_;

	MetaModel* followModel_;
	MetaModel* searchModel_;
};
