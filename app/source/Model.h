#pragma once
#include <QObject>

class Model : public QObject
{
	Q_OBJECT
public:
	explicit Model(QObject *parent = 0);
	~Model();

	void initSchema();

private:
};
