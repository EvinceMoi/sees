﻿#pragma once

#include "mpvabstractitem.h"

class MpvRenderer;

class MpvPlayer : public MpvAbstractItem
{
    Q_OBJECT
public:
    explicit MpvPlayer(QQuickItem *parent = nullptr);
    ~MpvPlayer() = default;

    enum class AsyncIds {
        None,
        SetVolume,
        GetVolume,
        ExpandText,
    };
    Q_ENUM(AsyncIds)

    Q_PROPERTY(QString mediaTitle READ mediaTitle NOTIFY mediaTitleChanged)
    QString mediaTitle();

    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged)
    double position();
    void setPosition(double value);

    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    double duration();

    Q_PROPERTY(QString formattedPosition READ formattedPosition NOTIFY positionChanged)
    QString formattedPosition() const;

    Q_PROPERTY(QString formattedDuration READ formattedDuration NOTIFY durationChanged)
    QString formattedDuration() const;

    Q_PROPERTY(bool pause READ pause WRITE setPause NOTIFY pauseChanged)
    bool pause();
    void setPause(bool value);

    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    int volume();
    void setVolume(int value);

    Q_PROPERTY(QUrl currentUrl READ currentUrl NOTIFY currentUrlChanged)
    QUrl currentUrl() const;

    Q_PROPERTY(bool mute READ mute NOTIFY muteChanged)
    bool mute();

    Q_INVOKABLE void loadFile(const QString &file);
    Q_INVOKABLE void loadMedia(const QString &video, const QString &audio, const QString &subtitle);
    Q_INVOKABLE void showInfo();
    Q_INVOKABLE void toggleMute();
    Q_INVOKABLE void stop();

Q_SIGNALS:
    void mediaTitleChanged();
    void currentUrlChanged();
    void positionChanged();
    void durationChanged();
    void pauseChanged();
    void volumeChanged();
    void muteChanged();

    void fileStarted();
    void fileLoaded();
    void endFile(QString reason);
    void videoReconfig();

private:
    void setupConnections();
    void onPropertyChanged(const QString &property, const QVariant &value);
    void onAsyncReply(const QVariant &data, mpv_event event);
    QString formatTime(const double time);

    double m_position{0.0};
    QString m_formattedPosition;
    double m_duration{0.0};
    QString m_formattedDuration;
    QUrl m_currentUrl;
};

