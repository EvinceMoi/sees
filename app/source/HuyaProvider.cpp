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
	auto uidReply = getUid();
	connect(uidReply, &QNetworkReply::finished, [this, rid, uidReply](){
		uidReply->deleteLater();

		auto uidData = uidReply->readAll();
		auto doc = QJsonDocument::fromJson(uidData);
		if (doc.isNull()) return;
		auto uid = doc["data"]["uid"].toString();

		auto reply = doRequest(rid);
		connect(reply, &QNetworkReply::finished, [this, rid, uid, reply](){
			reply->deleteLater();

			auto data = reply->readAll();
			processMedia(uid, data);
		});
		connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
			qDebug() << "network error fetchMedia: " << code;
			reply->deleteLater();
		});
	});
	connect(uidReply, &QNetworkReply::errorOccurred, [uidReply](QNetworkReply::NetworkError code){
		qDebug() << "network error fetchMedia: " << code;
		uidReply->deleteLater();
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

QNetworkReply *HuyaProvider::getUid()
{
	QUrl url("https://udblgn.huya.com/web/anonymousLogin");
	QNetworkRequest req(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonObject jv;
	jv["appId"] = 5002;
	jv["byPass"] = 3;
	jv["context"] = "";
	jv["version"] = "2.4";
	jv["data"] = QJsonObject();
	QJsonDocument doc(jv);
	return nam_->post(req, doc.toJson());
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
	auto profileRoom = pi["profileRoom"];
	mi.rid = profileRoom.isString() ? profileRoom.toString() : QString::number(pi["profileRoom"].toInt());
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

void HuyaProvider::processMedia(const QString& uid, const QByteArray& data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return;
	{
		auto status = doc["status"].toInt(0);
		if (status != 200) return;
	}
	auto jdata = doc["data"];
	auto live = jdata["liveStatus"].toString() == "ON";
	auto pi = jdata["profileInfo"];
	MediaInfo mi;
	mi.type = "huya";
	mi.rid = QString::number(pi["profileRoom"].toInt());
	if (live) {
		auto stream = jdata["stream"]["baseSteamInfoList"].toArray().first().toObject();
		if (stream.isEmpty()) return;

		auto flvUrl = stream["sFlvUrl"].toString();
		auto streamName = stream["sStreamName"].toString();
		auto flvUrlSuffix = stream["sFlvUrlSuffix"].toString();
		auto flvAntiCode = stream["sFlvAntiCode"].toString();
		QUrlQuery q(flvAntiCode);
		q.addQueryItem("ver", "1");
		q.addQueryItem("sv", "2110211124");
		auto seqid = uid.toULongLong() + QDateTime::currentMSecsSinceEpoch();
		q.addQueryItem("seqid", QString::number(seqid));
		q.addQueryItem("uid", uid);
		bool ok = false;
		auto ct = q.queryItemValue("wsTime").toULongLong(&ok, 16) * 1000 + QDateTime::currentMSecsSinceEpoch() % 1000;
		auto uuid = QString::number(ct % 100'0000'0000 + QDateTime::currentMSecsSinceEpoch() % 1000).left(10);
		q.addQueryItem("uuid", uuid);

		auto th = QString("%1|%2|%3").arg(seqid).arg(q.queryItemValue("ctype")).arg(q.queryItemValue("t"));
		auto ss = QString(QCryptographicHash::hash(th.toUtf8(), QCryptographicHash::Md5).toHex());
		auto fm = q.queryItemValue("fm");
		auto th2 = QString(QByteArray::fromBase64(fm.toUtf8()));
		th2.replace("$0", uid)
			.replace("$1", streamName)
			.replace("$2", ss)
			.replace("$3", q.queryItemValue("wsTime"));
		auto wsSecret = QString(QCryptographicHash::hash(th2.toUtf8(), QCryptographicHash::Md5).toHex());
		q.removeQueryItem("wsSecret");
		q.addQueryItem("wsSecret", wsSecret);
		auto params = q.toString();
		mi.video = QString("%1/%2.%3?%4").arg(flvUrl).arg(streamName).arg(flvUrlSuffix).arg(params);
	} else {
		auto ld = jdata["liveData"];
		mi.video = ld["hlsUrl"].toString();
	}
	emit gotMedia(mi);
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
