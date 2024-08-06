
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "MpvPlayer.h"
#include "MpvProps.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	qmlRegisterType<MpvPlayer>("MpvPlayer", 1, 0, "MpvPlayer");
	qmlRegisterSingletonInstance("MpvPlayer", 1, 0, "MpvProps", MpvProps::self());

	QQmlApplicationEngine engine(&app);
	engine.load(QUrl(QStringLiteral("qrc:/qt/qml/ui/MainWindow.qml")));

	return app.exec();
}