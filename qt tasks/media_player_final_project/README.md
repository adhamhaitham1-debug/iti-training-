### 🏛️ System Architecture

```mermaid
graph TD
    subgraph "1. UI Layer (QML / Front-End)"
        UI_Controls[Media Controls & VideoOutput]
        UI_Menus[Source Menu & BT Dropdown]
        UI_Lists[Queue, Favorites, Downloads]
    end

    subgraph "2. Bridge Layer (C++ / QML Integration)"
        Backend[MediaPlayerBackend]
    end

    subgraph "3. Core Backend Layer (C++ & Qt 6)"
        Core_Media[QMediaPlayer & QVideoSink]
        Core_Audio[QAudioOutput & QMediaDevices]
        Core_Storage[QStorageInfo & QDirIterator]
        Core_Settings[QSettings]
    end

    subgraph "4. OS & Hardware Layer (Linux / Ubuntu)"
        OS_Audio[PulseAudio / PipeWire / BlueZ]
        OS_Hardware[USB / CD-ROM / Local Disks]
    end

    %% Flow of data
    UI_Controls -->|Q_INVOKABLE| Backend
    UI_Menus -->|Q_INVOKABLE| Backend
    Backend -.->|Signals / Q_PROPERTY| UI_Controls
    Backend -.->|Signals| UI_Menus

    Backend --- Core_Media
    Backend --- Core_Audio
    Backend --- Core_Storage
    Backend --- Core_Settings

    Core_Media --> OS_Audio
    Core_Audio <--> OS_Audio
    Core_Storage --> OS_Hardware
