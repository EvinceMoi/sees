
#include "Application.h"
#include <QDebug>
#include "Model.h"

int main(int argc, char *argv[])
{
	Application app(argc, argv);
	app.init();

	Model m;
	m.initSchema();

	auto ec = app.exec();
	return ec;
}
