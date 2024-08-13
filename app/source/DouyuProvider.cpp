#include "DouyuProvider.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QRegularExpression>
#include <MetaInfo.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJSEngine>

DouyuProvider::DouyuProvider(QObject* parent)
	: SourceProvider(parent)
	, nam_(new QNetworkAccessManager(this))
{

}

DouyuProvider::~DouyuProvider()
{

}

QNetworkRequest DouyuProvider::genRequest(const QString& rid, bool mobile) const
{
	QNetworkRequest req;
	if (rid.isEmpty())
		return req;

	auto sub = mobile ? QString("m") : QString("www");
	auto url = QUrl(QString("https://%1.douyu.com/%2").arg(sub).arg(rid));
	req.setUrl(url);
	req.setRawHeader(QByteArrayLiteral("User-Agent"), getUserAgent().toUtf8());
	req.setRawHeader(QByteArrayLiteral("Referer"), QByteArrayLiteral("https://m.douyu.com"));

	return req;
}

void DouyuProvider::fetchMeta(const QString& rid)
{
	auto req = genRequest(rid, true);
	auto reply = nam_->get(req);
	connect(reply, &QNetworkReply::finished, [this, reply](){
		auto data = reply->readAll();
		auto mi = processMeta(data);
		// roomUpsert(mi);
		emit gotMeta(mi);
		reply->deleteLater();
	});
	connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError code){
		qDebug() << "network error fetchMeta: " << code;
		reply->deleteLater();
	});
}

void DouyuProvider::fetchMedia(const QString& rid)
{
	auto req = genRequest(rid, false);
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

MetaInfo* DouyuProvider::processMeta(const QByteArray& data)
{
	static QRegularExpression re(R"(<script id="vike_pageContext" type="application/json">(.*)</script>)");
	QString html(data);
	auto matches = re.match(html);
	if (!matches.hasMatch()) return nullptr;

	auto props = matches.captured(1);
	// qDebug() << props;
	auto doc = QJsonDocument::fromJson(props.toUtf8());
	if (doc.isNull()) return nullptr;
	auto ri = doc["pageProps"]["room"]["roomInfo"]["roomInfo"];
	qDebug() << ri;
	if (!ri.isObject()) return nullptr;

	auto mi = new MetaInfo;
	mi->id = ri["rid"].toString();
	mi->type = QString("douyu");
	mi->title = ri["roomName"].toString();
	mi->nick = ri["nickname"].toString();
	mi->avatar = ri["avatar"].toString();
	mi->image = ri["roomSrc"].toString();
	mi->heat = ri["hn"].toString();
	mi->live = ri["isLive"].toBool();
	mi->category = ri["cate2Name"].toString();
	mi->startTime = ri["showTime"].toInteger();
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

	auto params = QString("%1&cdn=ws-h5&rate=-1").arg(sign);

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
	auto mi = new MediaInfo;
	mi->video = url_live;
	emit gotMedia(mi);
}
