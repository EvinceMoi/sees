#include <QGuiApplication>

#include <QNetworkRequest>
class QQmlApplicationEngine;
class QNetworkAccessManager;
class QNetworkReply;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QGuiApplication::instance()))

#if defined(qGuiApp)
#undef qGuiApp
#endif

#define qGuiApp qApp

class Application : public QGuiApplication
{
	Q_OBJECT
public:
	Application(int argc, char* argv[]);
	~Application();

public:
	void init();

	QNetworkReply* httpGet(const QNetworkRequest& req);

private:
	QQmlApplicationEngine* qmlengine_;
	QNetworkAccessManager* nam_;
};
