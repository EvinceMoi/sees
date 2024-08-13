#include "Application.h"
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QDebug>

#include "MpvPlayer.h"
#include "MpvProps.h"

#include "Source.h"
#include "MediaInfo.h"

Application::Application(int argc, char* argv[])
	: QGuiApplication(argc, argv)
	, qmlengine_(new QQmlApplicationEngine(this))
	, nam_(new QNetworkAccessManager(this))
{
	connect(qmlengine_, &QQmlApplicationEngine::quit, this, &Application::doQuit);
	connect(this, &QGuiApplication::aboutToQuit, this, &Application::doQuit);

}

Application::~Application()
{

}

void Application::init()
{
	qmlRegisterType<MpvPlayer>("MpvPlayer", 1, 0, "MpvPlayer");
	qmlRegisterSingletonInstance("MpvPlayer", 1, 0, "MpvProps", MpvProps::self());

	qmlRegisterType<MediaInfo>("Source", 1, 0, "MediaInfo");
	qmlRegisterSingletonInstance("Source", 1, 0, "Source", Source::self());

	qmlengine_->load(QUrl(QStringLiteral("qrc:/qt/qml/ui/MainWindow.qml")));

}

void Application::doQuit()
{
	// qmlengine quit only quit the eventloop of engine.

	// it seems like the app won't exit if qmlengine is live
	// so it have to be manually deleted
	if (qmlengine_)
		qmlengine_->deleteLater();
}
