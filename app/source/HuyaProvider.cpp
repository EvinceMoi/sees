#include "HuyaProvider.h"
#include <algorithm>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJSEngine>
#include "Types.h"

HuyaProvider::HuyaProvider(QObject* parent)
	: SourceProvider(parent)
	, nam_(new QNetworkAccessManager(this))
{

}

HuyaProvider::~HuyaProvider()
{

}

void HuyaProvider::fetchMeta(const QString& rid)
{
	auto reply = doRequest(rid);
	connect(reply, &QNetworkReply::finished, [this, reply](){
		auto data = reply->readAll();
		auto mi = processMeta(data);
		if (mi)
			emit gotMeta(mi.value());
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
		qDebug() << "network error fetchMeta: " << code;
		reply->deleteLater();
	});
}

void HuyaProvider::fetchMedia(const QString& rid)
{
	auto reply = doRequest(rid);
	connect(reply, &QNetworkReply::finished, [this, rid, reply](){
		auto data = reply->readAll();
		auto mi = processMedia(data);
		if (mi)
			emit gotMedia(mi.value());
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
		qDebug() << "network error fetchMedia: " << code;
		reply->deleteLater();
	});
}

void HuyaProvider::search(const QString &in)
{
	QUrl url("https://search.cdn.huya.com/");
	QUrlQuery q;
	q.addQueryItem("m", "Search");
	q.addQueryItem("do", "getSearchContent");
	q.addQueryItem("q", in);
	q.addQueryItem("uid", "0");
	q.addQueryItem("v", "1");
	q.addQueryItem("typ", "-5");
	q.addQueryItem("livestate", "0");
	q.addQueryItem("rows", "20");
	q.addQueryItem("start", "0");
	url.setQuery(q);

	QNetworkRequest req(url);
	auto reply = nam_->get(req);
	connect(reply, &QNetworkReply::finished, [this, reply]() {
		auto data = reply->readAll();
		processSearch(data);
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code) {
		reply->deleteLater();
	});
}

QNetworkReply *HuyaProvider::doRequest(const QString &rid)
{
	QUrl url(QString("https://mp.huya.com/cache.php?m=Live&do=profileRoom&roomid=%1").arg(rid));
	QNetworkRequest req(url);
	return nam_->get(req);
}

std::optional<MetaInfo> HuyaProvider::processMeta(const QByteArray& data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return {};
	{
		auto status = doc["status"].toInt(0);
		if (status != 200) return {};
	}
	auto jdata = doc["data"];
	auto live = jdata["liveStatus"].toString() == "ON";
	auto pi = jdata["profileInfo"];
	auto ld = jdata["liveData"];

	MetaInfo mi;
	mi.rid = QString::number(pi["profileRoom"].toInt());
	mi.type = QString("huya");
	mi.title = ld["introduction"].toString();
	mi.nick = ld["nick"].toString();
	mi.avatar = ld["avatar180"].toString();
	mi.snapshot = ld["screenshot"].toString();
	mi.heat = ld["totalCount"].toInt(0);
	mi.live = live;
	mi.category = ld["gameFullName"].toString();
	mi.startTime = ld["startTime"].toInteger();
	return mi;
}

std::optional<MediaInfo> HuyaProvider::processMedia(const QByteArray& data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return {};
	{
		auto status = doc["status"].toInt(0);
		if (status != 200) return {};
	}
	auto jdata = doc["data"];
	auto live = jdata["liveStatus"].toString() == "ON";
	auto pi = jdata["profileInfo"];
	MediaInfo mi;
	mi.type = "huya";
	mi.rid = QString::number(pi["profileRoom"].toInt());
	if (live) {
		auto stream = jdata["stream"];
		auto flvs = stream["flv"]["multiLine"];

		QList<std::tuple<QString, int>> items;
		auto flvarr = flvs.toArray();
		for (auto it : flvarr) {
			auto obj = it.toObject();
			auto url = obj["url"].toString();
			auto pri = obj["webPriorityRate"].toInt(0);
			items.append(std::make_tuple(url, pri));
		}
		auto max = std::max_element(items.begin(), items.end(), [](auto l, auto r){
			return std::get<int>(l) < std::get<int>(r);
		});
		if (max == items.end()) return {};
		mi.video = std::get<QString>(*max);
	} else {
		auto ld = jdata["liveData"];
		mi.video = ld["hlsUrl"].toString();
	}
	return mi;
}

void HuyaProvider::processSearch(const QByteArray &data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return;

	auto lst = doc["response"]["3"]["docs"];
	if (!lst.isArray()) return;

	QList<MetaInfo> ret;

	auto arr = lst.toArray();
	for (auto it : arr) {
		auto obj = it.toObject();
		MetaInfo mi;
		mi.rid = QString::number(obj["room_id"].toInt());
		mi.type = "huya";
		mi.title = obj["game_introduction"].toString();
		mi.nick = obj["game_nick"].toString();
		mi.avatar = obj["game_imgUrl"].toString();
		mi.snapshot = obj["game_screenshot"].toString();
		mi.category = obj["gameName"].toString();
		mi.heat = obj["game_total_count"].toInt();
		mi.live = true;
		ret.append(mi);
	}

	emit searchResult(ret);
}
