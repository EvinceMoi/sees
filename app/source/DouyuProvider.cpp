#include "DouyuProvider.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJSEngine>
#include "Types.h"

static quint64 parseHeat(const QString& hn) {
	if (hn.endsWith("万")) {
		bool ok;
		auto r = hn.chopped(1).trimmed().toDouble(&ok);
		return ok ? r * 10000 : 0;
	} else {
		bool ok;
		auto r = hn.toULongLong(&ok, 10);
		return ok ? r : 0;
	}
}

DouyuProvider::DouyuProvider(QObject* parent)
	: SourceProvider(parent)
	, nam_(new QNetworkAccessManager(this))
{

}

DouyuProvider::~DouyuProvider()
{

}

QNetworkRequest DouyuProvider::genRequest(const QString& url, const QString& referer) const
{
	QNetworkRequest req;

	req.setUrl(url);
	req.setRawHeader(QByteArrayLiteral("User-Agent"), getUserAgent().toUtf8());
	req.setRawHeader(QByteArrayLiteral("Referer"), referer.toUtf8());

	return req;
}

void DouyuProvider::fetchMeta(const QString& rid)
{
	auto url = QString("https://www.douyu.com/betard/%1").arg(rid);
	auto req = genRequest(url, QString("https://www.douyu.com/%1").arg(rid));
	auto reply = nam_->get(req);
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

void DouyuProvider::fetchMedia(const QString& rid)
{
	auto req = genRequest(QString("https://www.douyu.com/%1").arg(rid), "https://www.douyu.com");
	auto reply = nam_->get(req);
	connect(reply, &QNetworkReply::finished, [this, rid, reply](){
		auto data = reply->readAll();
		processMedia(rid, data);
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
		qDebug() << "network error fetchMedia: " << code;
		reply->deleteLater();
	});
}

void DouyuProvider::search(const QString &in)
{
	auto req = genRequest(QString("https://www.douyu.com/japi/search/api/searchShow?kw=%1&page=1&pageSize=20").arg(in));
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

std::optional<MetaInfo> DouyuProvider::processMeta(const QByteArray& data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return {};
	auto ri = doc["room"];
	if (!ri.isObject()) return {};

	MetaInfo mi;
	mi.rid = QString::number(ri["room_id"].toInteger());
	mi.type = QString("douyu");
	mi.title = ri["room_name"].toString();
	mi.nick = ri["owner_name"].toString();
	mi.avatar = ri["owner_avatar"].toString();
	mi.snapshot = ri["room_pic"].toString();
	mi.heat = parseHeat(ri["room_biz_all"]["hot"].toString().trimmed());
	mi.live = ri["show_status"].toInt() == 1;
	mi.record = ri["videoLoop"].toInt() == 1;
	mi.category = ri["second_lvl_name"].toString();
	mi.startTime = ri["show_time"].toInteger();
	return mi;
}

void DouyuProvider::processMedia(const QString& rid, const QByteArray& data)
{
	static QRegularExpression re(R"((var vdwdae325w_64we[\s\S]*function ub98484234[\s\S]*?)function)");
	QString html(data);
	auto matches = re.match(html);
	if (!matches.hasMatch()) return;
	auto func_ub9 = matches.captured(1);
	func_ub9.replace(QRegularExpression(R"(eval.*?;})"), "strc;}");

	QJSEngine engine;
	auto ret = engine.evaluate(QString("%1 ub98484234()").arg(func_ub9)).toString();

	auto match_v = QRegularExpression(R"(v=(\d+))").match(ret);
	if (!match_v.hasMatch()) return;
	auto v = match_v.captured(1);
	auto ns = QDateTime::currentSecsSinceEpoch();
	static const QString did = "10000000000000000000000000001501";
	auto rbx = QString("%1%2%3%4").arg(rid).arg(did).arg(ns).arg(v);
	auto rb = QString(QCryptographicHash::hash(rbx.toUtf8(), QCryptographicHash::Md5).toHex());

	ret.replace(QRegularExpression(R"(return rt;}\);?)"), "return rt;}");
	ret.replace("(function (", "function sign(");
	ret.replace("CryptoJS.MD5(cb).toString()", QString(R"("%1")").arg(rb));

	auto sign = engine.evaluate(QString("%1; sign('%2', '%3', '%4')").arg(ret).arg(rid).arg(did).arg(ns)).toString();

	auto params = QString("%1&cdn=ws-h5&rate=0").arg(sign);

	QNetworkRequest req;
	req.setUrl(QUrl(QString("https://www.douyu.com/lapi/live/getH5Play/%1").arg(rid)));
	req.setRawHeader(QByteArrayLiteral("User-Agent"), getUserAgent().toUtf8());
	req.setRawHeader(QByteArrayLiteral("Referer"), QByteArrayLiteral("https://www.douyu.com"));
	req.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/x-www-form-urlencoded; charset=UTF-8"));

	auto reply = nam_->post(req, params.toUtf8());
	connect(reply, &QNetworkReply::finished, [this, rid, reply](){
		auto data = reply->readAll();
		handleMediaInfo(rid, data);
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
		qDebug() << "network error: " << code;
		reply->deleteLater();
	});
}

void DouyuProvider::handleMediaInfo(const QString &rid, const QByteArray &data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return;

	auto info = doc["data"];
	if (info.isNull()) return;
	auto url = info["rtmp_url"].toString("");
	auto live = info["rtmp_live"].toString("");
	if (url.isEmpty() || live.isEmpty()) return;

	auto url_live = url + "/" + live;
	MediaInfo mi;
	mi.type = "douyu";
	mi.rid = rid;
	mi.video = url_live;
	emit gotMedia(mi);
}

void DouyuProvider::processSearch(const QByteArray &data)
{
	auto doc = QJsonDocument::fromJson(data);
	if (doc.isNull()) return;

	if (doc["error"].toInt(-1) != 0) return;
	auto lst = doc["data"]["relateShow"];
	if (!lst.isArray()) return;

	QList<MetaInfo> ret;

	auto arr = lst.toArray();
	for (auto it : arr) {
		auto obj = it.toObject();
		MetaInfo mi;
		mi.rid = QString::number(obj["rid"].toInt());
		mi.type = "douyu";
		mi.title = obj["roomName"].toString();
		mi.nick = obj["nickName"].toString();
		mi.avatar = obj["avatar"].toString();
		mi.snapshot = obj["roomSrc"].toString();
		mi.category = obj["cateName"].toString();
		mi.heat = parseHeat(obj["hot"].toString().trimmed());
		mi.live = obj["isLive"].toInt() == 1;
		ret.append(mi);
	}

	emit searchResult(ret);
}
