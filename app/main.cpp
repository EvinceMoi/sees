
#include "Application.h"
#include <QDebug>


int main(int argc, char *argv[])
{
	Application app(argc, argv);
	app.init();

	auto ec = app.exec();
	return ec;
}
