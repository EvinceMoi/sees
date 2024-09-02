#include <QGuiApplication>

class QQmlApplicationEngine;

class Application : public QGuiApplication
{
	Q_OBJECT
public:
	Application(int argc, char* argv[]);
	~Application();

public:
	void init();

private slots:
	void doQuit();

private:
	QQmlApplicationEngine* qmlengine_;
};
