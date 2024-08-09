#pragma once

#include <QObject>
#include <QQmlEngine>

class Douyu : public QObject
{
	Q_OBJECT
	QML_ELEMENT
public:
	explicit Douyu(QObject *parent = nullptr);

	void getRoomInfo(const QString& rid);

signals:

};
