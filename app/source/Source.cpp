#include "Source.h"

#include <algorithm>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "DouyuProvider.h"


std::once_flag Source::once_flag_;

Source::Source(QObject* parent)
	: QObject(parent)
{
}

Source::~Source()
{

}

Source* Source::self()
{
	static Source s;
	std::call_once(once_flag_, [&](){
		s.registerProviders();
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
		connect(sp.get(), &SourceProvider::gotMeta, [](MetaInfo* mi){
			// got meta
		});
		connect(sp.get(), &SourceProvider::gotMedia, [this](MediaInfo* mi){
			emit gotMedia(mi);
		});
	}
}

void Source::roomUpsert(MetaInfo *mi)
{
	auto it = std::find_if(rooms_.begin(), rooms_.end(), [mi](auto it){
		return it->id == mi->id && it->type == mi->type;
	});
	if (it == rooms_.end()) {
		// insert
		rooms_.append(mi);
	} else {
		// update
		auto old = *it;
		*it = mi;
		old->deleteLater();
	}
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
