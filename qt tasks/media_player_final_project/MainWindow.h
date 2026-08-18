#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVideoWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QListWidget>
#include <QStringList>
#include <QTextEdit>
#include <QMovie>
#include "MediaPlayerBackend.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MediaPlayerBackend *m_backend;

    // Left Panel
    QWidget *m_leftQueuePanel;
    QListWidget *m_queueList;
    QPushButton *m_favTabBtn;
    QPushButton *m_dlTabBtn;

    // Center Display
    QStackedWidget *m_displayStack;
    QLabel *m_albumArtLabel;
    QMovie *m_albumArtMovie;
    QVideoWidget *m_videoWidget;
    QTextEdit *m_lyricsDisplay;
    QLabel *m_trackTitleLabel;
    QLabel *m_trackArtistLabel;

    // Right Panel
    QWidget *m_rightRadioPanel;
    QListWidget *m_radioList;
    QPushButton *m_addRadioBtn;

    // Controls
    QPushButton *m_favBtn;
    QPushButton *m_downloadBtn;
    QPushButton *m_prevBtn;
    QPushButton *m_playBtn;
    QPushButton *m_nextBtn;
    QPushButton *m_importBtn;
    QPushButton *m_repeatBtn; // <--- Added Repeat Button
    QPushButton *m_volumeBtn;
    QPushButton *m_queueToggleBtn;

    // Audio Menu Elements
    QPushButton *m_audioSourceBtn;
    QLabel *m_selectedAudioLabel;

    QSlider *m_timeSlider;
    QSlider *m_volumeSlider;
    QLabel *m_elapsedTimeLabel;
    QLabel *m_totalTimeLabel;

    // --- State & Data Management ---
    enum ViewMode { VIEW_QUEUE, VIEW_FAVORITES, VIEW_DOWNLOADS };
    ViewMode m_currentView;

    QString m_currentlyPlayingFile;
    QStringList m_queueFiles;
    QStringList m_favoritesFiles;
    QStringList m_downloadsFiles;

    bool m_isRepeatMode; // <--- Tracks if repeat is active

    void buildUi();
    void setupConnections();
    QString formatTime(qint64 ms);

    // Core Logic Functions
    void loadData();
    void saveData();
    void updateListWidget();
    void playTrack(const QString &filePath);
    void playNextTrack();
    void playPrevTrack();
};

#endif // MAINWINDOW_H
