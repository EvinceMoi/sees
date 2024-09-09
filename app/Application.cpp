#include "Application.h"
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QStandardPaths>
#include <QIcon>
#include <QDebug>

#include "MpvPlayer.h"
#include "MpvProps.h"

#include "Source.h"
// #include "Types.h"

Application::Application(int argc, char* argv[])
	: QGuiApplication(argc, argv)
	, qmlengine_(new QQmlApplicationEngine(this))
{
	setOrganizationName("sees");
	setOrganizationName("sees");
	setApplicationName("sees");
	setWindowIcon(QIcon(":/assets/res/icon.png"));
}

Application::~Application()
{

}

void Application::init()
{
	connect(qmlengine_, &QQmlApplicationEngine::quit, this, &Application::doQuit);
	connect(this, &QGuiApplication::aboutToQuit, this, &Application::doQuit);

	qmlRegisterType<MpvPlayer>("MpvPlayer", 1, 0, "MpvPlayer");
	qmlRegisterSingletonInstance("MpvPlayer", 1, 0, "MpvProps", MpvProps::self());

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
