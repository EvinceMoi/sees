#pragma once
#include <QObject>
#include <QList>

class MetaInfo;

class Model : public QObject
{
	Q_OBJECT
public:
	explicit Model(QObject *parent = 0);
	~Model();

	void initSchema();

	void loadFollows();
	void updateFollow(MetaInfo*);

	QList<MetaInfo*> data();

signals:

private:
	QList<MetaInfo*> follows_;
};
