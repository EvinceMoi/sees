#pragma once

#include <QObject>

class MpvProps : public QObject
{
    Q_OBJECT

public:
    static MpvProps *self()
    {
        static MpvProps p;
        return &p;
    }

    Q_PROPERTY(QString MediaTitle MEMBER MediaTitle CONSTANT)
    const QString MediaTitle{QStringLiteral("media-title")};

    Q_PROPERTY(QString Position MEMBER Position CONSTANT)
    const QString Position{QStringLiteral("time-pos")};

    Q_PROPERTY(QString Duration MEMBER Duration CONSTANT)
    const QString Duration{QStringLiteral("duration")};

    Q_PROPERTY(QString Pause MEMBER Pause CONSTANT)
    const QString Pause{QStringLiteral("pause")};

    Q_PROPERTY(QString Volume MEMBER Volume CONSTANT)
    const QString Volume{QStringLiteral("volume")};

    Q_PROPERTY(QString Mute MEMBER Mute CONSTANT)
    const QString Mute{QStringLiteral("mute")};

private:
    explicit MpvProps(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    MpvProps(const MpvProps &) = delete;
    MpvProps &operator=(const MpvProps &) = delete;
    MpvProps(MpvProps &&) = delete;
    MpvProps &operator=(MpvProps &&) = delete;
};

