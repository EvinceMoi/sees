#include "Source.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "DouyuProvider.h"


std::once_flag Source::once_flag_;

Source::Source(QObject* parent)
	: QObject(parent)
	, model_(new Model(this))
{
	model_->initSchema();
}

Source::~Source()
{

}

Source* Source::self()
{
	static Source s;
	std::call_once(once_flag_, [&](){
		s.registerProviders();
		s.loadData();
	});
	return &s;
}

void Source::registerProviders()
{
	{
		// douyu
		auto it = std::make_shared<DouyuProvider>();
		mtype_.insert({it->getType(), it});
		for (auto &m : it->getMatches()) {
			mmatch_.insert({m, it});
		}
	}

	for (auto &[k, sp]: mtype_) {
		connect(sp.get(), &SourceProvider::gotMeta, [this](const MetaInfo& mi){
			// got meta
			model_->updateFollow(mi);
		});
		connect(sp.get(), &SourceProvider::gotMedia, [this](const MediaInfo& mi){
			emit mediaInfoFetched(mi.video, mi.audio, mi.subtitle);
		});
	}
}

void Source::loadData()
{
	model_->loadFollows();
}

void Source::getMetaInfo(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMeta(ref);
}

void Source::getMediaInfo(const QString& type, const QString& ref)
{
	if (!mtype_.contains(type)) return;

	auto sp = mtype_[type];
	sp->fetchMedia(ref);
}

Model* Source::follows()
{
	return model_;
}

void Source::refresh(int gap)
{
	gap = std::max(0, gap);
	auto outdated = model_->outdated(gap);
	for (auto& it : outdated) {
		getMetaInfo(it.first, it.second);
	}
}
