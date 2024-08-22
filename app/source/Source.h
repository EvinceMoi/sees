#pragma once

#include <QObject>
#include <mutex>
#include <QList>
#include "Model.h"

class SourceProvider;

class Source : public QObject
{
	Q_OBJECT
public:
	explicit Source(QObject* parent = 0);
	~Source();

	static Source* self();

	Q_INVOKABLE void getMetaInfo(const QString& type, const QString& ref); // ("douyu", "roomid / url")
	Q_INVOKABLE void getMediaInfo(const QString& type, const QString& ref);

	Q_INVOKABLE Model* follows();

	Q_INVOKABLE void refresh(int gap);

signals:
	void mediaInfoFetched(const QString& video, const QString& audio, const QString& subtitle);

private:
	void registerProviders();
	void loadData();

private:
	using SourceProviderPtr = std::shared_ptr<SourceProvider>;
	std::unordered_map<QString, SourceProviderPtr> mtype_;
	std::unordered_map<QString, SourceProviderPtr> mmatch_;

	static std::once_flag once_flag_;

	Model* model_;
};
