#pragma once

#include <QObject>
#include <mutex>
#include <QList>
#include "Model.h"

class MetaInfo;
class MediaInfo;
class SourceProvider;
// class Model;

class Source : public QObject
{
	Q_OBJECT
public:
	explicit Source(QObject* parent = 0);
	~Source();

	static Source* self();

	Q_INVOKABLE void getMetaInfo(const QString& type, const QString& ref); // ("douyu", "roomid / url")
	Q_INVOKABLE void getMediaInfo(const QString& type, const QString& ref);

	Q_INVOKABLE QList<MetaInfo*> follows();

signals:
	void play(MediaInfo* mi);


private:
	void registerProviders();
	void loadData();
	// void roomUpsert(MetaInfo *);

private:
	using SourceProviderPtr = std::shared_ptr<SourceProvider>;
	std::unordered_map<QString, SourceProviderPtr> mtype_;
	std::unordered_map<QString, SourceProviderPtr> mmatch_;

	static std::once_flag once_flag_;

	// QList<MetaInfo *> rooms_;

	Model model_;
};
