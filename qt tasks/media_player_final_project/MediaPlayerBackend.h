#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QStringList>
#include <QSettings>
#include <QVariantMap>
#include <QStorageInfo>
#include <QDirIterator>

// NEW: Bluetooth & Audio Device Detection
#include <QMediaDevices>
#include <QAudioDevice>

class MediaPlayerBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool isMuted READ isMuted NOTIFY mutedChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playbackStateChanged)
    Q_PROPERTY(bool isRepeatMode READ isRepeatMode NOTIFY repeatModeChanged)
    Q_PROPERTY(bool isRadioMode READ isRadioMode NOTIFY radioModeChanged)
    Q_PROPERTY(bool isFavorite READ isFavorite NOTIFY favoriteStatusChanged)
    Q_PROPERTY(bool isDownloaded READ isDownloaded NOTIFY downloadStatusChanged)
    Q_PROPERTY(int currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)
    Q_PROPERTY(QStringList activeFileList READ activeFileList NOTIFY activeFileListChanged)
    Q_PROPERTY(QVariantList radioStations READ radioStations NOTIFY radioStationsChanged)

    // Video Property
    Q_PROPERTY(bool hasVideo READ hasVideo NOTIFY hasVideoChanged)

    // NEW: Available Audio / Bluetooth Outputs
    Q_PROPERTY(QStringList audioDevices READ audioDevices NOTIFY audioDevicesChanged)

public:
    enum ViewMode { VIEW_QUEUE = 0, VIEW_FAVORITES = 1, VIEW_DOWNLOADS = 2 };
    Q_ENUM(ViewMode)

    explicit MediaPlayerBackend(QObject *parent = nullptr);
    ~MediaPlayerBackend();

    // Getters
    QString title() const { return m_title; }
    QString artist() const { return m_artist; }
    qint64 position() const { return m_position; }
    qint64 duration() const { return m_duration; }
    float volume() const;
    bool isMuted() const;
    bool isPlaying() const { return m_isPlaying; }
    bool isRepeatMode() const { return m_isRepeatMode; }
    bool isRadioMode() const { return m_isRadioMode; }
    bool isFavorite() const;
    bool isDownloaded() const;
    int currentView() const { return static_cast<int>(m_currentView); }
    QStringList activeFileList() const;
    QVariantList radioStations() const { return m_radioStations; }

    // Video Getter
    bool hasVideo() const { return m_player->hasVideo(); }

    // NEW: Audio Devices Getter
    QStringList audioDevices() const;

    // Invokable QML Methods
    Q_INVOKABLE void playTrack(const QString &filePath);
    Q_INVOKABLE void playRadioStation(const QString &name, const QString &streamUrl);
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void playNextTrack();
    Q_INVOKABLE void playPrevTrack();
    Q_INVOKABLE void setPosition(qint64 position);
    Q_INVOKABLE void setVolume(float volume);
    Q_INVOKABLE void toggleMute();
    Q_INVOKABLE void toggleRepeat();
    Q_INVOKABLE void toggleFavorite();
    Q_INVOKABLE void toggleDownload();
    Q_INVOKABLE void importFiles(const QList<QUrl> &urls);
    Q_INVOKABLE void setCurrentView(int view);
    Q_INVOKABLE void setRadioMode(bool isRadio);
    Q_INVOKABLE void addRadioStation(const QString &name, const QString &url);

    // USB Scanning and Video Sink linking
    Q_INVOKABLE void scanExternalStorage();
    Q_INVOKABLE void setVideoSink(QObject *sink);

    // NEW: Select active audio output device by list index
    Q_INVOKABLE void selectAudioDevice(int index);

signals:
    void titleChanged();
    void artistChanged();
    void positionChanged();
    void durationChanged();
    void volumeChanged();
    void mutedChanged();
    void playbackStateChanged();
    void repeatModeChanged();
    void radioModeChanged();
    void favoriteStatusChanged();
    void downloadStatusChanged();
    void currentViewChanged();
    void activeFileListChanged();
    void radioStationsChanged();

    // Video Signal
    void hasVideoChanged();

    // NEW: Signal emitted when audio devices connect/disconnect
    void audioDevicesChanged();

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;

    // NEW: Media Devices Monitor
    QMediaDevices m_mediaDevices;

    QString m_title;
    QString m_artist;
    qint64 m_position;
    qint64 m_duration;
    bool m_isPlaying;
    bool m_isRepeatMode;
    bool m_isRadioMode;

    ViewMode m_currentView;
    QString m_currentlyPlayingFile;

    QStringList m_queueFiles;
    QStringList m_favoritesFiles;
    QStringList m_downloadsFiles;
    QVariantList m_radioStations;

    void loadData();
    void saveData();
    void setupConnections();
    void updateMetadata();
};

#endif // MEDIAPLAYERBACKEND_H
