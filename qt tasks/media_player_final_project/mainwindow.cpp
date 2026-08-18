#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QUrl>
#include <QFile>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QInputDialog> // Added for the custom radio URL pop-up

// Helper function to dynamically recolor dark PNG/SVG icons to pure white
static QIcon makeWhiteIcon(const QString &resourcePath)
{
    QPixmap pixmap(resourcePath);
    if (pixmap.isNull()) return QIcon();

    QPixmap whitePixmap(pixmap.size());
    whitePixmap.fill(Qt::transparent);

    QPainter painter(&whitePixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(whitePixmap.rect(), Qt::white);
    painter.end();

    return QIcon(whitePixmap);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_backend(new MediaPlayerBackend(this))
    , m_currentView(VIEW_QUEUE)
    , m_isRepeatMode(false) // Default to repeat off
{
    loadData();
    buildUi();
    setupConnections();
    updateListWidget();
}

MainWindow::~MainWindow()
{
    saveData();
}

void MainWindow::loadData()
{
    QSettings settings("MyCompany", "QtMediaPlayer");
    m_queueFiles = settings.value("queue").toStringList();
    m_favoritesFiles = settings.value("favorites").toStringList();
    m_downloadsFiles = settings.value("downloads").toStringList();
}

void MainWindow::saveData()
{
    QSettings settings("MyCompany", "QtMediaPlayer");
    settings.setValue("queue", m_queueFiles);
    settings.setValue("favorites", m_favoritesFiles);
    settings.setValue("downloads", m_downloadsFiles);
}

void MainWindow::buildUi()
{
    this->resize(1000, 680);
    this->setStyleSheet("QMainWindow { background-color: #121214; }");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(20, 20, 20, 20);
    rootLayout->setSpacing(15);

    // --- TOP MAIN AREA ---
    QHBoxLayout *mainAreaLayout = new QHBoxLayout();

    // 1. Left Queue Panel
    m_leftQueuePanel = new QWidget(this);
    m_leftQueuePanel->setFixedWidth(220);
    QVBoxLayout *queueLayout = new QVBoxLayout(m_leftQueuePanel);
    queueLayout->setContentsMargins(0, 0, 0, 0);

    m_favTabBtn = new QPushButton("Favorites", m_leftQueuePanel);
    m_dlTabBtn = new QPushButton("Downloads", m_leftQueuePanel);
    m_queueList = new QListWidget(m_leftQueuePanel);

    queueLayout->addWidget(m_favTabBtn);
    queueLayout->addWidget(m_dlTabBtn);
    queueLayout->addWidget(m_queueList);
    m_leftQueuePanel->setVisible(false);

    // 2. Center Display Stack
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);

    m_displayStack = new QStackedWidget(this);
    m_displayStack->setFixedSize(320, 260);

    // Index 0: Album Art
    m_albumArtLabel = new QLabel(this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
    m_albumArtLabel->setStyleSheet("background-color: #1e1e24; border-radius: 12px;");

    m_albumArtMovie = new QMovie(":/icons/animation.gif", QByteArray(), this);
    if (m_albumArtMovie->isValid()) {
        m_albumArtMovie->setScaledSize(QSize(180, 180));
        m_albumArtLabel->setMovie(m_albumArtMovie);
    } else {
        QPixmap art(":/icons/downloaded.png");
        if (!art.isNull()) m_albumArtLabel->setPixmap(art.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Index 1: Video Widget
    m_videoWidget = new QVideoWidget(this);
    m_backend->setVideoOutput(m_videoWidget);

    // Index 2: Lyrics Display
    m_lyricsDisplay = new QTextEdit(this);
    m_lyricsDisplay->setReadOnly(true);
    m_lyricsDisplay->setStyleSheet("background-color: #1e1e24; color: #ffffff; border-radius: 12px; padding: 15px; border: none; font-size: 14px;");
    m_lyricsDisplay->setAlignment(Qt::AlignCenter);
    m_lyricsDisplay->setText("No track playing.");

    m_displayStack->addWidget(m_albumArtLabel);   // index 0
    m_displayStack->addWidget(m_videoWidget);     // index 1
    m_displayStack->addWidget(m_lyricsDisplay);   // index 2
    m_displayStack->setCurrentIndex(0);

    m_trackTitleLabel = new QLabel("Audio Name", this);
    m_trackTitleLabel->setAlignment(Qt::AlignCenter);
    m_trackTitleLabel->setStyleSheet("color: #ffffff; font-size: 18px; font-weight: bold;");

    m_trackArtistLabel = new QLabel("Unknown Artist", this);
    m_trackArtistLabel->setAlignment(Qt::AlignCenter);
    m_trackArtistLabel->setStyleSheet("color: #a0a0a0; font-size: 14px;");

    centerLayout->addWidget(m_displayStack, 0, Qt::AlignCenter);
    centerLayout->addSpacing(8);
    centerLayout->addWidget(m_trackTitleLabel);
    centerLayout->addWidget(m_trackArtistLabel);

    // 3. Right Radio Panel
    m_rightRadioPanel = new QWidget(this);
    m_rightRadioPanel->setFixedWidth(220);
    QVBoxLayout *radioLayout = new QVBoxLayout(m_rightRadioPanel);
    radioLayout->setContentsMargins(0, 0, 0, 0);

    // Create a horizontal row for the title and the add button
    QHBoxLayout *radioHeaderLayout = new QHBoxLayout();
    radioHeaderLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *radioTitle = new QLabel("Radio Channels", m_rightRadioPanel);
    radioTitle->setStyleSheet("color: #ffffff; font-weight: bold; font-size: 14px;");

    m_addRadioBtn = new QPushButton("+ Add URL", m_rightRadioPanel);
    m_addRadioBtn->setCursor(Qt::PointingHandCursor);
    m_addRadioBtn->setStyleSheet("color: #e63946; font-size: 12px; font-weight: bold; text-align: right; background: transparent;");

    radioHeaderLayout->addWidget(radioTitle);
    radioHeaderLayout->addStretch();
    radioHeaderLayout->addWidget(m_addRadioBtn);

    m_radioList = new QListWidget(m_rightRadioPanel);

    // Default hardcoded stations
    QListWidgetItem *radio1 = new QListWidgetItem("Radio Quran", m_radioList);
    radio1->setData(Qt::UserRole, "https://stream.radiojar.com/8s5u5tpdtwzuv");



    radioLayout->addLayout(radioHeaderLayout);
    radioLayout->addWidget(m_radioList);
    m_rightRadioPanel->setVisible(false);

    mainAreaLayout->addWidget(m_leftQueuePanel);
    mainAreaLayout->addLayout(centerLayout, 1);
    mainAreaLayout->addWidget(m_rightRadioPanel);

    rootLayout->addLayout(mainAreaLayout, 1);

    // --- BOTTOM CONTROLS SECTION ---
    QVBoxLayout *bottomSection = new QVBoxLayout();
    bottomSection->setSpacing(12);

    QHBoxLayout *importRow = new QHBoxLayout();

    // --> REPEAT BUTTON ON THE LEFT <--
    m_repeatBtn = new QPushButton(this);
    m_repeatBtn->setIcon(makeWhiteIcon(":/icons/repeat.png"));
    m_repeatBtn->setIconSize(QSize(24, 24));
    m_repeatBtn->setFixedSize(36, 36);
    m_repeatBtn->setCursor(Qt::PointingHandCursor);
    m_repeatBtn->setCheckable(true); // Makes it act like a toggle
    m_repeatBtn->setToolTip("Repeat Current Audio");
    m_repeatBtn->setStyleSheet("QPushButton:checked { background-color: #3b3b4a; border: 1px solid #e63946; border-radius: 6px; }");

    importRow->addWidget(m_repeatBtn);
    importRow->addStretch(); // Pushes import button to the right

    // Original Import Button
    m_importBtn = new QPushButton(this);
    m_importBtn->setIcon(makeWhiteIcon(":/icons/import.png"));
    m_importBtn->setIconSize(QSize(36, 36));
    m_importBtn->setFixedSize(48, 48);
    m_importBtn->setCursor(Qt::PointingHandCursor);
    m_importBtn->setToolTip("Import Audio");

    importRow->addWidget(m_importBtn);
    importRow->addSpacing(5);

    bottomSection->addLayout(importRow);

    // Timeline Slider
    m_timeSlider = new QSlider(Qt::Horizontal, this);
    m_timeSlider->setCursor(Qt::PointingHandCursor);
    bottomSection->addWidget(m_timeSlider);

    // Start / End Time Labels
    QHBoxLayout *timeLabelsLayout = new QHBoxLayout();
    m_elapsedTimeLabel = new QLabel("00:00", this);
    m_elapsedTimeLabel->setStyleSheet("color: #b0b0b0; font-size: 13px;");

    m_totalTimeLabel = new QLabel("-00:00", this);
    m_totalTimeLabel->setStyleSheet("color: #b0b0b0; font-size: 13px;");

    timeLabelsLayout->addWidget(m_elapsedTimeLabel, 0, Qt::AlignLeft);
    timeLabelsLayout->addStretch(1);
    timeLabelsLayout->addWidget(m_totalTimeLabel, 0, Qt::AlignRight);

    bottomSection->addLayout(timeLabelsLayout);

    // Middle Playback Row
    QHBoxLayout *middleControlsRow = new QHBoxLayout();
    middleControlsRow->setAlignment(Qt::AlignCenter);
    middleControlsRow->setSpacing(35);

    m_favBtn = new QPushButton(this);
    m_favBtn->setIcon(makeWhiteIcon(":/icons/unlike.png"));
    m_favBtn->setIconSize(QSize(32, 32));
    m_favBtn->setFixedSize(44, 44);
    m_favBtn->setCursor(Qt::PointingHandCursor);

    m_prevBtn = new QPushButton(this);
    m_prevBtn->setIcon(makeWhiteIcon(":/icons/prev.png"));
    m_prevBtn->setIconSize(QSize(48, 48));
    m_prevBtn->setFixedSize(56, 56);
    m_prevBtn->setCursor(Qt::PointingHandCursor);

    m_playBtn = new QPushButton(this);
    m_playBtn->setIcon(makeWhiteIcon(":/icons/play.png"));
    m_playBtn->setIconSize(QSize(48, 48));
    m_playBtn->setFixedSize(70, 70);
    m_playBtn->setCursor(Qt::PointingHandCursor);
    m_playBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2a2a35;
            border: 2px solid #ffffff;
            border-radius: 35px;
        }
        QPushButton:hover {
            background-color: #3b3b4a;
        }
    )");

    m_nextBtn = new QPushButton(this);
    m_nextBtn->setIcon(makeWhiteIcon(":/icons/next.png"));
    m_nextBtn->setIconSize(QSize(48, 48));
    m_nextBtn->setFixedSize(56, 56);
    m_nextBtn->setCursor(Qt::PointingHandCursor);

    m_downloadBtn = new QPushButton(this);
    m_downloadBtn->setIcon(makeWhiteIcon(":/icons/undownload.png"));
    m_downloadBtn->setIconSize(QSize(32, 32));
    m_downloadBtn->setFixedSize(44, 44);
    m_downloadBtn->setCursor(Qt::PointingHandCursor);

    middleControlsRow->addWidget(m_favBtn);
    middleControlsRow->addWidget(m_prevBtn);
    middleControlsRow->addWidget(m_playBtn);
    middleControlsRow->addWidget(m_nextBtn);
    middleControlsRow->addWidget(m_downloadBtn);

    bottomSection->addLayout(middleControlsRow);
    bottomSection->addSpacing(15);

    // --- STRICTLY ALIGNED GRID ROW ---
    QGridLayout *bottomRowLayout = new QGridLayout();
    bottomRowLayout->setContentsMargins(0, 0, 0, 0);

    // Audio Menu Group
    QWidget *audioSelectWidget = new QWidget(this);
    QVBoxLayout *audioSelectLayout = new QVBoxLayout(audioSelectWidget);
    audioSelectLayout->setContentsMargins(0, 0, 0, 0);
    audioSelectLayout->setSpacing(4);
    audioSelectLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_audioSourceBtn = new QPushButton("AUDIO ⚙", this);
    m_audioSourceBtn->setCursor(Qt::PointingHandCursor);

    QMenu *audioMenu = new QMenu(this);
    audioMenu->addAction("Download");
    audioMenu->addAction("Radio");
    audioMenu->addAction("Bluetooth");
    audioMenu->addAction("CDRom");
    audioMenu->addAction("Favourites");
    m_audioSourceBtn->setMenu(audioMenu);

    m_selectedAudioLabel = new QLabel("Download", this);
    m_selectedAudioLabel->setStyleSheet("color: #e63946; font-size: 11px; font-weight: bold;");
    m_selectedAudioLabel->setAlignment(Qt::AlignCenter);

    audioSelectLayout->addWidget(m_audioSourceBtn);
    audioSelectLayout->addWidget(m_selectedAudioLabel);

    connect(audioMenu, &QMenu::triggered, this, [this](QAction *action) {
        QString mode = action->text();
        m_selectedAudioLabel->setText(mode);
        m_rightRadioPanel->setVisible(mode == "Radio");

        // --- Toggle Timeline vs LIVE text ---
        bool isRadio = (mode == "Radio");
        m_timeSlider->setVisible(!isRadio); // Hide slider if radio

        if (isRadio) {
            m_elapsedTimeLabel->setText("🔴 LIVE");
            m_elapsedTimeLabel->setStyleSheet("color: #e63946; font-size: 13px; font-weight: bold;");
            m_totalTimeLabel->setText(""); // Clear the right label
        } else {
            // Restore default look for local audio
            m_elapsedTimeLabel->setText("00:00");
            m_elapsedTimeLabel->setStyleSheet("color: #b0b0b0; font-size: 13px;");
            m_totalTimeLabel->setText("-00:00");
        }
    });

    m_queueToggleBtn = new QPushButton("QUEUE", this);
    m_queueToggleBtn->setCursor(Qt::PointingHandCursor);

    // Audio & Volume Group
    QWidget *audioSoundWidget = new QWidget(this);
    QHBoxLayout *audioSoundGroup = new QHBoxLayout(audioSoundWidget);
    audioSoundGroup->setContentsMargins(0, 0, 0, 0);
    audioSoundGroup->setSpacing(8);

    m_volumeBtn = new QPushButton(this);
    m_volumeBtn->setIcon(makeWhiteIcon(":/icons/Sound.png"));
    m_volumeBtn->setIconSize(QSize(28, 28));
    m_volumeBtn->setFixedSize(36, 36);
    m_volumeBtn->setCursor(Qt::PointingHandCursor);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(70);
    m_volumeSlider->setFixedWidth(90);
    m_volumeSlider->setCursor(Qt::PointingHandCursor);

    audioSoundGroup->addWidget(m_volumeBtn);
    audioSoundGroup->addWidget(m_volumeSlider);

    bottomRowLayout->addWidget(audioSelectWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    bottomRowLayout->addWidget(m_queueToggleBtn, 0, 1, Qt::AlignCenter | Qt::AlignVCenter);
    bottomRowLayout->addWidget(audioSoundWidget, 0, 2, Qt::AlignRight | Qt::AlignVCenter);

    bottomRowLayout->setColumnStretch(0, 1);
    bottomRowLayout->setColumnStretch(1, 1);
    bottomRowLayout->setColumnStretch(2, 1);

    bottomSection->addLayout(bottomRowLayout);
    rootLayout->addLayout(bottomSection);

    centralWidget->setStyleSheet(R"(
        QPushButton { background: transparent; color: #ffffff; border: none; font-size: 14px; font-weight: bold; }
        QPushButton:hover { opacity: 0.8; }
        QPushButton::menu-indicator { image: none; }
        QListWidget { background-color: #1a1a1e; color: #ffffff; border: 1px solid #2a2a30; border-radius: 8px; padding: 5px; }
        QListWidget::item:selected { background-color: #e63946; border-radius: 4px; }
        QSlider::groove:horizontal { height: 4px; background: #2a2a30; border-radius: 2px; }
        QSlider::sub-page:horizontal { background: #ffffff; border-radius: 2px; }
        QSlider::handle:horizontal { background: #ffffff; width: 14px; height: 14px; margin: -5px 0; border-radius: 7px; }
        QMenu { background-color: #1e1e24; color: #ffffff; border: 1px solid #33333d; padding: 5px; border-radius: 8px; }
        QMenu::item { padding: 8px 25px; }
        QMenu::item:selected { background-color: #e63946; border-radius: 4px; }
    )");
}

void MainWindow::updateListWidget()
{
    m_queueList->clear();
    QStringList currentList;

    if (m_currentView == VIEW_FAVORITES) {
        currentList = m_favoritesFiles;
        m_favTabBtn->setStyleSheet("color: #e63946;");
        m_dlTabBtn->setStyleSheet("color: #ffffff;");
    } else if (m_currentView == VIEW_DOWNLOADS) {
        currentList = m_downloadsFiles;
        m_favTabBtn->setStyleSheet("color: #ffffff;");
        m_dlTabBtn->setStyleSheet("color: #e63946;");
    } else {
        currentList = m_queueFiles;
        m_favTabBtn->setStyleSheet("color: #ffffff;");
        m_dlTabBtn->setStyleSheet("color: #ffffff;");
    }

    for (const QString &filePath : currentList) {
        QFileInfo fi(filePath);
        QListWidgetItem *item = new QListWidgetItem(fi.fileName(), m_queueList);
        item->setData(Qt::UserRole, filePath);
    }
}

void MainWindow::playTrack(const QString &filePath)
{
    if (filePath.isEmpty()) return;

    m_currentlyPlayingFile = filePath;
    QFileInfo fi(filePath);

    m_trackTitleLabel->setText(fi.fileName());
    m_trackArtistLabel->setText("Loading...");

    m_favBtn->setIcon(makeWhiteIcon(m_favoritesFiles.contains(filePath) ? ":/icons/liked.png" : ":/icons/unlike.png"));
    m_downloadBtn->setIcon(makeWhiteIcon(m_downloadsFiles.contains(filePath) ? ":/icons/downloaded.png" : ":/icons/undownload.png"));

    m_backend->playMedia(QUrl::fromLocalFile(filePath));
}

void MainWindow::playNextTrack()
{
    QStringList currentList = (m_currentView == VIEW_FAVORITES) ? m_favoritesFiles :
                                  (m_currentView == VIEW_DOWNLOADS) ? m_downloadsFiles : m_queueFiles;

    if (currentList.isEmpty()) return;
    int idx = currentList.indexOf(m_currentlyPlayingFile);

    if (idx >= 0 && idx < currentList.size() - 1) {
        playTrack(currentList[idx + 1]);
    } else {
        playTrack(currentList.first()); // Loop to start
    }
}

void MainWindow::playPrevTrack()
{
    QStringList currentList = (m_currentView == VIEW_FAVORITES) ? m_favoritesFiles :
                                  (m_currentView == VIEW_DOWNLOADS) ? m_downloadsFiles : m_queueFiles;

    if (currentList.isEmpty()) return;
    int idx = currentList.indexOf(m_currentlyPlayingFile);

    if (idx > 0) {
        playTrack(currentList[idx - 1]);
    } else {
        playTrack(currentList.last()); // Loop to end
    }
}

void MainWindow::setupConnections()
{
    // --- AUTOPLAY / REPEAT LOGIC ---
    connect(m_backend->player(), &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_isRepeatMode && !m_currentlyPlayingFile.isEmpty()) {
                // Instantly replay the same track
                m_backend->setPosition(0);
                m_backend->player()->play();
            } else {
                // Otherwise move to the next track
                playNextTrack();
            }
        }
    });

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) // For Qt 5
    connect(m_backend->player(), QOverload<>::of(&QMediaPlayer::metaDataChanged), this, [this]() {
        QString title = m_backend->player()->metaData(QMediaMetaData::Title).toString();
        QString artist = m_backend->player()->metaData(QMediaMetaData::ContributingArtist).toString();
        if(artist.isEmpty()) artist = m_backend->player()->metaData(QMediaMetaData::Author).toString();

        if (!title.isEmpty()) m_trackTitleLabel->setText(title);
        if (!artist.isEmpty()) m_trackArtistLabel->setText(artist);
        else m_trackArtistLabel->setText("Unknown Artist");
    });
#else // For Qt 6
    connect(m_backend->player(), &QMediaPlayer::metaDataChanged, this, [this]() {
        QMediaMetaData meta = m_backend->player()->metaData();
        QString title = meta.stringValue(QMediaMetaData::Title);
        QString artist = meta.stringValue(QMediaMetaData::ContributingArtist);
        if(artist.isEmpty()) artist = meta.stringValue(QMediaMetaData::Author);

        if (!title.isEmpty()) m_trackTitleLabel->setText(title);
        if (!artist.isEmpty()) m_trackArtistLabel->setText(artist);
        else m_trackArtistLabel->setText("Unknown Artist");
    });
#endif

    // --- REPEAT BUTTON TOGGLE ---
    connect(m_repeatBtn, &QPushButton::toggled, this, [this](bool checked) {
        m_isRepeatMode = checked;
    });

    // --- IMPORT BUTTON ---
    connect(m_importBtn, &QPushButton::clicked, this, [this]() {
        QStringList files = QFileDialog::getOpenFileNames(this, "Import Audio Files", QString(), "Audio Files (*.mp3 *.wav *.m4a *.flac *.aac *.mp4)");
        if (files.isEmpty()) return;

        for (const QString &file : files) {
            if (!m_queueFiles.contains(file)) m_queueFiles.append(file);
        }

        m_currentView = VIEW_QUEUE;
        m_leftQueuePanel->setVisible(true);
        updateListWidget();
        saveData();
    });

    // --- PLAY QUEUE ITEM ---
    connect(m_queueList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        QString filePath = item->data(Qt::UserRole).toString();
        playTrack(filePath);
    });

    // --- NEW: Add Custom Radio URL ---
    connect(m_addRadioBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString name = QInputDialog::getText(this, "Add Custom Radio",
                                             "Enter Station Name:",
                                             QLineEdit::Normal, "", &ok);

        if (ok && !name.isEmpty()) {
            QString url = QInputDialog::getText(this, "Add Custom Radio",
                                                "Enter Direct Stream URL:",
                                                QLineEdit::Normal, "http://", &ok);

            if (ok && !url.isEmpty()) {
                QListWidgetItem *newItem = new QListWidgetItem(name, m_radioList);
                newItem->setData(Qt::UserRole, url);
            }
        }
    });

    // --- NEW: Play Radio Stream ---
    connect(m_radioList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        QString streamUrl = item->data(Qt::UserRole).toString();
        m_currentlyPlayingFile = streamUrl;

        m_trackTitleLabel->setText(item->text());
        m_trackArtistLabel->setText("Live Radio Broadcast");

        // Play as a network stream (QUrl), not a local file!
        m_backend->playMedia(QUrl(streamUrl));
    });

    connect(m_nextBtn, &QPushButton::clicked, this, &MainWindow::playNextTrack);
    connect(m_prevBtn, &QPushButton::clicked, this, &MainWindow::playPrevTrack);

    connect(m_favBtn, &QPushButton::clicked, this, [this]() {
        if (m_currentlyPlayingFile.isEmpty()) return;
        if (!m_favoritesFiles.contains(m_currentlyPlayingFile)) {
            m_favoritesFiles.append(m_currentlyPlayingFile);
            m_favBtn->setIcon(makeWhiteIcon(":/icons/liked.png"));
        } else {
            m_favoritesFiles.removeAll(m_currentlyPlayingFile);
            m_favBtn->setIcon(makeWhiteIcon(":/icons/unlike.png"));
        }
        saveData();
        if (m_currentView == VIEW_FAVORITES) updateListWidget();
    });

    connect(m_downloadBtn, &QPushButton::clicked, this, [this]() {
        if (m_currentlyPlayingFile.isEmpty()) return;
        if (!m_downloadsFiles.contains(m_currentlyPlayingFile)) {
            m_downloadsFiles.append(m_currentlyPlayingFile);
            m_downloadBtn->setIcon(makeWhiteIcon(":/icons/downloaded.png"));
        } else {
            m_downloadsFiles.removeAll(m_currentlyPlayingFile);
            m_downloadBtn->setIcon(makeWhiteIcon(":/icons/undownload.png"));
        }
        saveData();
        if (m_currentView == VIEW_DOWNLOADS) updateListWidget();
    });

    connect(m_favTabBtn, &QPushButton::clicked, this, [this]() { m_currentView = VIEW_FAVORITES; updateListWidget(); });
    connect(m_dlTabBtn, &QPushButton::clicked, this, [this]() { m_currentView = VIEW_DOWNLOADS; updateListWidget(); });

    connect(m_queueToggleBtn, &QPushButton::clicked, this, [this]() {
        bool visible = !m_leftQueuePanel->isVisible();
        m_leftQueuePanel->setVisible(visible);

        if (visible) {
            m_currentView = VIEW_QUEUE;
            updateListWidget();
            m_queueToggleBtn->setStyleSheet("background-color: #ffffff; color: #121214; border-radius: 12px; padding: 4px 16px;");
        } else {
            m_queueToggleBtn->setStyleSheet("");
        }
    });

    connect(m_playBtn, &QPushButton::clicked, this, [this]() { m_backend->togglePlayPause(); });

    // --- PLAY/PAUSE & GIF SYNC LOGIC ---
    connect(m_backend, &MediaPlayerBackend::playbackStateChanged, this, [this](bool isPlaying) {
        m_playBtn->setIcon(makeWhiteIcon(isPlaying ? ":/icons/pause.png" : ":/icons/play.png"));
        m_playBtn->setIconSize(QSize(48, 48));

        if (m_albumArtMovie->isValid()) {
            if (isPlaying) {
                if (m_albumArtMovie->state() == QMovie::NotRunning) {
                    m_albumArtMovie->start();
                } else {
                    m_albumArtMovie->setPaused(false);
                }
            } else {
                m_albumArtMovie->setPaused(true);
            }
        }
    });

    // --- VOLUME CONTROLS ---
    connect(m_volumeSlider, &QSlider::valueChanged, this, [this](int val) {
        float vol = val / 100.0f;
        m_backend->setVolume(vol);
        m_volumeBtn->setIcon(makeWhiteIcon(val == 0 ? ":/icons/mute.png" : ":/icons/Sound.png"));
    });

    connect(m_volumeBtn, &QPushButton::clicked, this, [this]() {
        bool muted = !m_backend->isMuted();
        m_backend->setMuted(muted);
        m_volumeBtn->setIcon(makeWhiteIcon(muted ? ":/icons/mute.png" : ":/icons/Sound.png"));
    });

    // --- TIMELINE POSITIONS ---
    connect(m_backend, &MediaPlayerBackend::durationChanged, this, [this](qint64 duration) {
        // Stop updating the timeline duration if we are in Radio mode
        if (m_selectedAudioLabel->text() == "Radio") return;

        m_timeSlider->setRange(0, duration);
        m_totalTimeLabel->setText("-" + formatTime(duration));
    });

    connect(m_backend, &MediaPlayerBackend::positionChanged, this, [this](qint64 position) {
        // Stop updating the ticking numbers if we are in Radio mode
        if (m_selectedAudioLabel->text() == "Radio") {
            m_elapsedTimeLabel->setText("🔴 LIVE");
            m_totalTimeLabel->setText("");
            return;
        }

        // Normal local audio behavior
        if (!m_timeSlider->isSliderDown()) m_timeSlider->setValue(position);

        m_elapsedTimeLabel->setText(formatTime(position));
        qint64 remaining = m_timeSlider->maximum() - position;
        m_totalTimeLabel->setText("-" + formatTime(remaining > 0 ? remaining : 0));
    });

    // --- RESTORED: SLIDER DRAG LOGIC ---
    connect(m_timeSlider, &QSlider::sliderMoved, this, [this](int position) {
        m_backend->setPosition(position);
    });
}

QString MainWindow::formatTime(qint64 ms)
{
    qint64 seconds = (ms / 1000) % 60;
    qint64 minutes = (ms / (1000 * 60)) % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
