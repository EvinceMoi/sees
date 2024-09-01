#include "MpvPlayer.h"

#include "mpvcontroller.h"

#include "MpvProps.h"

MpvPlayer::MpvPlayer(QQuickItem *parent)
    : MpvAbstractItem(parent)
{
    observeProperty(MpvProps::self()->MediaTitle, MPV_FORMAT_STRING);
    observeProperty(MpvProps::self()->Position, MPV_FORMAT_DOUBLE);
    observeProperty(MpvProps::self()->Duration, MPV_FORMAT_DOUBLE);
    observeProperty(MpvProps::self()->Pause, MPV_FORMAT_FLAG);
    observeProperty(MpvProps::self()->Volume, MPV_FORMAT_INT64);
    observeProperty(MpvProps::self()->Mute, MPV_FORMAT_FLAG);

    setupConnections();

    // since this is async the effects are not immediately visible
    // to do something after the property was set do it in onAsyncReply
    // use the id to identify the correct call
    setPropertyAsync(QStringLiteral("volume"), 64, static_cast<int>(MpvPlayer::AsyncIds::SetVolume));
    setProperty(QStringLiteral("mute"), false);

    // since this is async the effects are not immediately visible
    // to get the value do it in onGetPropertyReply
    // use the id to identify the correct call
    getPropertyAsync(MpvProps::self()->Volume, static_cast<int>(MpvPlayer::AsyncIds::GetVolume));
}

void MpvPlayer::setupConnections()
{
    // clang-format off
    connect(mpvController(), &MpvController::propertyChanged,
            this, &MpvPlayer::onPropertyChanged, Qt::QueuedConnection);

    connect(mpvController(), &MpvController::fileStarted,
            this, &MpvPlayer::fileStarted, Qt::QueuedConnection);

    connect(mpvController(), &MpvController::fileLoaded,
            this, &MpvPlayer::fileLoaded, Qt::QueuedConnection);

    connect(mpvController(), &MpvController::endFile,
            this, &MpvPlayer::endFile, Qt::QueuedConnection);

    connect(mpvController(), &MpvController::videoReconfig,
            this, &MpvPlayer::videoReconfig, Qt::QueuedConnection);

    connect(mpvController(), &MpvController::asyncReply,
            this, &MpvPlayer::onAsyncReply, Qt::QueuedConnection);
    // clang-format on
}

void MpvPlayer::onPropertyChanged(const QString &property, const QVariant &value)
{
    if (property == MpvProps::self()->MediaTitle) {
        Q_EMIT mediaTitleChanged();

    } else if (property == MpvProps::self()->Position) {
        m_formattedPosition = formatTime(value.toDouble());
        Q_EMIT positionChanged();

    } else if (property == MpvProps::self()->Duration) {
        m_formattedDuration = formatTime(value.toDouble());
        Q_EMIT durationChanged();

    } else if (property == MpvProps::self()->Pause) {
        Q_EMIT pauseChanged();

    } else if (property == MpvProps::self()->Volume) {
        Q_EMIT volumeChanged();

    } else if (property == MpvProps::self()->Mute) {
    	Q_EMIT muteChanged();

    }
}

void MpvPlayer::onAsyncReply(const QVariant &data, mpv_event event)
{
    switch (static_cast<AsyncIds>(event.reply_userdata)) {
    case AsyncIds::None: {
        break;
    }
    case AsyncIds::SetVolume: {
        qDebug() << "onSetPropertyReply" << event.reply_userdata;
        break;
    }
    case AsyncIds::GetVolume: {
        qDebug() << "onGetPropertyReply" << event.reply_userdata << data;
        break;
    }
    case AsyncIds::ExpandText: {
        qDebug() << "onGetPropertyReply" << event.reply_userdata << data;
        break;
    }
    }
}

QString MpvPlayer::formatTime(const double time)
{
    int totalNumberOfSeconds = static_cast<int>(time);
    int seconds = totalNumberOfSeconds % 60;
    int minutes = (totalNumberOfSeconds / 60) % 60;
    int hours = (totalNumberOfSeconds / 60 / 60);

    QString timeString =
        QStringLiteral("%1:%2:%3").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));

    return timeString;
}

void MpvPlayer::loadFile(const QString &file)
{
    auto url = QUrl::fromUserInput(file);
    if (m_currentUrl != url) {
        m_currentUrl = url;
        Q_EMIT currentUrlChanged();
    }

    Q_EMIT command(QStringList() << QStringLiteral("loadfile") << m_currentUrl.toLocalFile());
}

void MpvPlayer::loadMedia(const QString &video, const QString &audio, const QString &subtitle)
{
    auto url = QUrl::fromUserInput(video);
    if (m_currentUrl != url) {
        m_currentUrl = url;
        Q_EMIT currentUrlChanged();
    }

	QStringList commands;
	commands << QStringLiteral("loadfile") << video;
	if (!audio.isEmpty() || !subtitle.isEmpty()) {
		commands << QStringLiteral("replace") << QStringLiteral("0");

		QStringList options;
		if (!audio.isEmpty()) {
			options.append(QString("audio-file=%1").arg(audio));
		}
		if (!subtitle.isEmpty()) {
			options.append(QString("sub-file=%1").arg(subtitle));
		}

		commands << options.join(',');
	}

	Q_EMIT command(commands);
}

void MpvPlayer::showInfo()
{
	QStringList commands;
	commands << QStringLiteral("script-binding") << QStringLiteral("stats/display-stats");
	Q_EMIT command(commands);
}

void MpvPlayer::toggleMute()
{
	auto prop = QStringLiteral("mute");
	auto mute = getProperty(prop).toBool();
	setProperty(prop, !mute);
}

QString MpvPlayer::mediaTitle()
{
    return getProperty(MpvProps::self()->MediaTitle).toString();
}

double MpvPlayer::position()
{
    return getProperty(MpvProps::self()->Position).toDouble();
}

void MpvPlayer::setPosition(double value)
{
    if (qFuzzyCompare(value, position())) {
        return;
    }
    Q_EMIT setPropertyAsync(MpvProps::self()->Position, value);
}

double MpvPlayer::duration()
{
    return getProperty(MpvProps::self()->Duration).toDouble();
}

bool MpvPlayer::pause()
{
    return getProperty(MpvProps::self()->Pause).toBool();
}

void MpvPlayer::setPause(bool value)
{
    if (value == pause()) {
        return;
    }
    Q_EMIT setPropertyAsync(MpvProps::self()->Pause, value);
}

int MpvPlayer::volume()
{
    return getProperty(MpvProps::self()->Volume).toInt();
}

void MpvPlayer::setVolume(int value)
{
    if (value == volume()) {
        return;
    }
    Q_EMIT setPropertyAsync(MpvProps::self()->Volume, value);
}

QString MpvPlayer::formattedDuration() const
{
    return m_formattedDuration;
}

QString MpvPlayer::formattedPosition() const
{
    return m_formattedPosition;
}

QUrl MpvPlayer::currentUrl() const
{
    return m_currentUrl;
}

bool MpvPlayer::mute()
{
	return getProperty(MpvProps::self()->Mute).toBool();
}
