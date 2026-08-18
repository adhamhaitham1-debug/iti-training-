#include "MediaPlayerBackend.h"
#include <QMediaMetaData>
#include <QFileInfo>
#include <QVideoSink>
#include <QMediaDevices>
#include <QAudioDevice>

MediaPlayerBackend::MediaPlayerBackend(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_title("Audio Name")
    , m_artist("Unknown Artist")
    , m_position(0)
    , m_duration(0)
    , m_isPlaying(false)
    , m_isRepeatMode(false)
    , m_isRadioMode(false)
    , m_currentView(VIEW_QUEUE)
{
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.7f);

    // --- RADIO STATIONS ---
    m_radioStations = {
        QVariantMap{
            {"name", "Quran Radio Cairo"},
            {"country", "🇪🇬 Egypt"},
            {"url", "http://n12.radiojar.com/8s5u5tpdtwzuv"}
        },
        QVariantMap{
            {"name", "Saudi Quran Radio"},
            {"country", "🇸🇦 Saudi Arabia"},
            {"url", "http://live.mp3quran.net:8008/"}
        },
        QVariantMap{
            {"name", "Quran Radio Nablus"},
            {"country", "🇵🇸 Palestine"},
            {"url", "http://www.quran-radio.org:8002/"}
        },
        QVariantMap{
            {"name", "Quran Radio"},
            {"country", "🇸🇦 Saudi Arabia"},
            {"url", "http://live.mp3quran.net:8002/"}
        }
    };

    loadData();
    setupConnections();
}

MediaPlayerBackend::~MediaPlayerBackend()
{
    saveData();
}

void MediaPlayerBackend::loadData()
{
    QSettings settings("MyCompany", "QtMediaPlayer");
    m_queueFiles = settings.value("queue").toStringList();
    m_favoritesFiles = settings.value("favorites").toStringList();
    m_downloadsFiles = settings.value("downloads").toStringList();
}

void MediaPlayerBackend::saveData()
{
    QSettings settings("MyCompany", "QtMediaPlayer");
    settings.setValue("queue", m_queueFiles);
    settings.setValue("favorites", m_favoritesFiles);
    settings.setValue("downloads", m_downloadsFiles);
}

void MediaPlayerBackend::setupConnections()
{
    // Auto-notify QML when Bluetooth/audio devices are connected or disconnected
    connect(&m_mediaDevices, &QMediaDevices::audioOutputsChanged, this, &MediaPlayerBackend::audioDevicesChanged);

    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 pos) {
        m_position = pos;
        emit positionChanged();
    });

    connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 dur) {
        m_duration = dur;
        emit durationChanged();
    });

    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        m_isPlaying = (state == QMediaPlayer::PlayingState);
        emit playbackStateChanged();
    });

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_isRepeatMode && !m_currentlyPlayingFile.isEmpty()) {
                m_player->setPosition(0);
                m_player->play();
            } else {
                playNextTrack();
            }
        }
    });

    connect(m_player, &QMediaPlayer::metaDataChanged, this, [this]() {
        QMediaMetaData meta = m_player->metaData();
        QString title = meta.stringValue(QMediaMetaData::Title);
        QString artist = meta.stringValue(QMediaMetaData::ContributingArtist);
        if (artist.isEmpty()) artist = meta.stringValue(QMediaMetaData::Author);

        if (!title.isEmpty()) m_title = title;
        if (!artist.isEmpty()) m_artist = artist;
        else if (title.isEmpty()) m_artist = "Unknown Artist";

        emit titleChanged();
        emit artistChanged();
    });

    connect(m_player, &QMediaPlayer::hasVideoChanged, this, &MediaPlayerBackend::hasVideoChanged);
}

// Audio/Bluetooth Output Enumeration and Selection
QStringList MediaPlayerBackend::audioDevices() const
{
    QStringList deviceList;
    const auto devices = QMediaDevices::audioOutputs();
    for (const auto &device : devices) {
        deviceList.append(device.description());
    }
    return deviceList;
}

void MediaPlayerBackend::selectAudioDevice(int index)
{
    const auto devices = QMediaDevices::audioOutputs();
    if (index >= 0 && index < devices.size()) {
        bool wasPlaying = (m_player->playbackState() == QMediaPlayer::PlayingState);

        // Pause briefly to flush the audio buffer
        if (wasPlaying) {
            m_player->pause();
        }

        // Switch output sink
        m_audioOutput->setDevice(devices.at(index));

        // Resume playback on new device
        if (wasPlaying) {
            m_player->play();
        }

        qDebug() << "Audio output switched to:" << devices.at(index).description();
    }
}
void MediaPlayerBackend::playTrack(const QString &filePath)
{
    if (filePath.isEmpty()) return;

    m_isRadioMode = false;
    emit radioModeChanged();

    m_currentlyPlayingFile = filePath;
    QFileInfo fi(filePath);
    m_title = fi.fileName();
    m_artist = "Loading...";

    emit titleChanged();
    emit artistChanged();
    emit favoriteStatusChanged();
    emit downloadStatusChanged();

    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
}

void MediaPlayerBackend::playRadioStation(const QString &name, const QString &streamUrl)
{
    m_isRadioMode = true;
    emit radioModeChanged();

    m_currentlyPlayingFile = streamUrl;
    m_title = name;
    m_artist = "Live Radio Broadcast";

    emit titleChanged();
    emit artistChanged();

    m_player->setSource(QUrl(streamUrl));
    m_player->play();
}

void MediaPlayerBackend::togglePlayPause()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
    } else if (!m_player->source().isEmpty()) {
        m_player->play();
    }
}

void MediaPlayerBackend::playNextTrack()
{
    QStringList currentList = activeFileList();
    if (currentList.isEmpty()) return;

    int idx = currentList.indexOf(m_currentlyPlayingFile);
    if (idx >= 0 && idx < currentList.size() - 1) {
        playTrack(currentList[idx + 1]);
    } else {
        playTrack(currentList.first());
    }
}

void MediaPlayerBackend::playPrevTrack()
{
    QStringList currentList = activeFileList();
    if (currentList.isEmpty()) return;

    int idx = currentList.indexOf(m_currentlyPlayingFile);
    if (idx > 0) {
        playTrack(currentList[idx - 1]);
    } else {
        playTrack(currentList.last());
    }
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    m_player->setPosition(position);
}

void MediaPlayerBackend::setVolume(float volume)
{
    m_audioOutput->setVolume(volume);
    emit volumeChanged();
}

float MediaPlayerBackend::volume() const
{
    return m_audioOutput->volume();
}

void MediaPlayerBackend::toggleMute()
{
    m_audioOutput->setMuted(!m_audioOutput->isMuted());
    emit mutedChanged();
}

bool MediaPlayerBackend::isMuted() const
{
    return m_audioOutput->isMuted();
}

void MediaPlayerBackend::toggleRepeat()
{
    m_isRepeatMode = !m_isRepeatMode;
    emit repeatModeChanged();
}

void MediaPlayerBackend::toggleFavorite()
{
    if (m_currentlyPlayingFile.isEmpty()) return;

    if (!m_favoritesFiles.contains(m_currentlyPlayingFile)) {
        m_favoritesFiles.append(m_currentlyPlayingFile);
    } else {
        m_favoritesFiles.removeAll(m_currentlyPlayingFile);
    }

    saveData();
    emit favoriteStatusChanged();
    if (m_currentView == VIEW_FAVORITES) emit activeFileListChanged();
}

bool MediaPlayerBackend::isFavorite() const
{
    return m_favoritesFiles.contains(m_currentlyPlayingFile);
}

void MediaPlayerBackend::toggleDownload()
{
    if (m_currentlyPlayingFile.isEmpty()) return;

    if (!m_downloadsFiles.contains(m_currentlyPlayingFile)) {
        m_downloadsFiles.append(m_currentlyPlayingFile);
    } else {
        m_downloadsFiles.removeAll(m_currentlyPlayingFile);
    }

    saveData();
    emit downloadStatusChanged();
    if (m_currentView == VIEW_DOWNLOADS) emit activeFileListChanged();
}

bool MediaPlayerBackend::isDownloaded() const
{
    return m_downloadsFiles.contains(m_currentlyPlayingFile);
}

void MediaPlayerBackend::importFiles(const QList<QUrl> &urls)
{
    for (const QUrl &url : urls) {
        QString path = url.toLocalFile();
        if (!path.isEmpty() && !m_queueFiles.contains(path)) {
            m_queueFiles.append(path);
        }
    }
    saveData();
    m_currentView = VIEW_QUEUE;
    emit currentViewChanged();
    emit activeFileListChanged();
}

void MediaPlayerBackend::setCurrentView(int view)
{
    m_currentView = static_cast<ViewMode>(view);
    emit currentViewChanged();
    emit activeFileListChanged();
}

void MediaPlayerBackend::setRadioMode(bool isRadio)
{
    m_isRadioMode = isRadio;
    emit radioModeChanged();
}

void MediaPlayerBackend::addRadioStation(const QString &name, const QString &url)
{
    if (name.isEmpty() || url.isEmpty()) return;
    QVariantMap station;
    station["name"] = name;
    station["url"] = url;
    m_radioStations.append(station);
    emit radioStationsChanged();
}

QStringList MediaPlayerBackend::activeFileList() const
{
    switch (m_currentView) {
    case VIEW_FAVORITES: return m_favoritesFiles;
    case VIEW_DOWNLOADS: return m_downloadsFiles;
    case VIEW_QUEUE:
    default: return m_queueFiles;
    }
}

void MediaPlayerBackend::setVideoSink(QObject *sink)
{
    QVideoSink *videoSink = qobject_cast<QVideoSink*>(sink);
    if (videoSink) {
        m_player->setVideoOutput(videoSink);
    }
}

void MediaPlayerBackend::scanExternalStorage()
{
    QStringList foundFiles;
    QStringList nameFilters = {"*.mp3", "*.wav", "*.m4a", "*.flac", "*.aac", "*.mp4", "*.mkv", "*.avi", "*.mov"};

    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady() && !storage.isRoot()) {
            QString path = storage.rootPath();

#if defined(Q_OS_LINUX)
            if (!path.startsWith("/media/") && !path.startsWith("/run/media/") && !path.startsWith("/mnt/")) continue;
#elif defined(Q_OS_MAC)
            if (!path.startsWith("/Volumes/")) continue;
#elif defined(Q_OS_WIN)
            if (path.startsWith("C:")) continue;
#endif

            QDirIterator it(path, nameFilters, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                foundFiles.append(it.next());
            }
        }
    }

    if (!foundFiles.isEmpty()) {
        for (const QString &file : foundFiles) {
            if (!m_queueFiles.contains(file)) {
                m_queueFiles.append(file);
            }
        }
        saveData();
        setCurrentView(VIEW_QUEUE);
        setRadioMode(false);
        emit activeFileListChanged();
    }
}
