#include "Application.h"
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QDebug>

#include "MpvPlayer.h"
#include "MpvProps.h"

Application::Application(int argc, char* argv[])
	: QGuiApplication(argc, argv)
	, qmlengine_(new QQmlApplicationEngine(this))
	, nam_(new QNetworkAccessManager(this))
{
	connect(qmlengine_, &QQmlApplicationEngine::quit, this, [this](){
		// qmlengine quit only quit the eventloop of engine.

		// it seems like the app won't exit if qmlengine is live
		// so it have to be manually deleted
		qmlengine_->deleteLater();

		// call application quit
		QGuiApplication::quit();
	}, Qt::QueuedConnection);
}

Application::~Application()
{

}

void Application::init()
{
	qmlRegisterType<MpvPlayer>("MpvPlayer", 1, 0, "MpvPlayer");
	qmlRegisterSingletonInstance("MpvPlayer", 1, 0, "MpvProps", MpvProps::self());

	qmlengine_->load(QUrl(QStringLiteral("qrc:/qt/qml/ui/MainWindow.qml")));

}

QNetworkReply* Application::httpGet(const QNetworkRequest& req)
{
	return nam_->get(req);
}
