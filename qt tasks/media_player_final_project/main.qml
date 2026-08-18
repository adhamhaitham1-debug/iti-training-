import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
import QtMultimedia

ApplicationWindow {
    id: window
    width: 1000
    height: 680
    minimumWidth: 600
    minimumHeight: 500
    visible: true
    title: "Qt Quick Media Player"
    color: "#121214"

    property bool isQueueVisible: false
    property bool isRadioPanelVisible: false

    function formatTime(ms) {
        if (!ms || ms <= 0) return "00:00";
        var seconds = Math.floor((ms / 1000) % 60);
        var minutes = Math.floor((ms / (1000 * 60)) % 60);
        return (minutes < 10 ? "0" : "") + minutes + ":" + (seconds < 10 ? "0" : "") + seconds;
    }

    // --- File Dialog for Audio Import ---
    FileDialog {
        id: fileDialog
        title: "Import Audio Files"
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        nameFilters: ["Audio Files (*.mp3 *.wav *.m4a *.flac *.aac *.mp4)"]
        fileMode: FileDialog.OpenFiles
        onAccepted: {
            backend.importFiles(selectedFiles)
            isQueueVisible = true
        }
    }

    // --- Custom Dialog to Add Radio Station ---
    Dialog {
        id: addRadioDialog
        title: "Add Custom Radio"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            spacing: 10
            TextField {
                id: stationNameField
                placeholderText: "Station Name"
                Layout.fillWidth: true
            }
            TextField {
                id: stationUrlField
                placeholderText: "Stream URL (http://...)"
                Layout.fillWidth: true
            }
        }

        onAccepted: {
            backend.addRadioStation(stationNameField.text, stationUrlField.text)
            stationNameField.text = ""
            stationUrlField.text = ""
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // ==========================================
        // TOP MAIN AREA
        // ==========================================
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 15

            // 1. LEFT QUEUE PANEL
            Rectangle {
                Layout.preferredWidth: 220
                Layout.fillHeight: true
                color: "#1a1a1e"
                border.color: "#2a2a30"
                radius: 8
                visible: window.isQueueVisible

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    RowLayout {
                        Button {
                            text: "Favorites"
                            Layout.fillWidth: true
                            flat: true
                            contentItem: Text {
                                text: parent.text
                                color: backend.currentView === 1 ? "#e63946" : "#ffffff"
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: backend.setCurrentView(1)
                        }
                        Button {
                            text: "Downloads"
                            Layout.fillWidth: true
                            flat: true
                            contentItem: Text {
                                text: parent.text
                                color: backend.currentView === 2 ? "#e63946" : "#ffffff"
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: backend.setCurrentView(2)
                        }
                    }

                    ListView {
                        id: queueListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: backend.activeFileList

                        delegate: Rectangle {
                            width: queueListView.width
                            height: 35
                            color: "transparent"
                            radius: 4

                            Text {
                                text: modelData.split('/').pop()
                                color: "#ffffff"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                elide: Text.ElideRight
                                width: parent.width - 10
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: backend.playTrack(modelData)
                            }
                        }
                    }
                }
            }

            // 2. CENTER DISPLAY AREA

            // 2. CENTER DISPLAY AREA
                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 10

                            // Responsive Album Art / Video Frame
                            Rectangle {
                                Layout.preferredWidth: 320
                                Layout.preferredHeight: 260
                                Layout.minimumWidth: 150
                                Layout.minimumHeight: 150
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "transparent"

                                AnimatedImage {
                                    id: albumArtGif
                                    source: "qrc:/icons/animation.gif"
                                    anchors.fill: parent
                                    playing: backend.isPlaying
                                    fillMode: Image.Stretch
                                    visible: !backend.hasVideo
                                }

                                VideoOutput {
                                    id: videoOutput
                                    anchors.fill: parent
                                    fillMode: VideoOutput.PreserveAspectFit
                                    visible: backend.hasVideo

                                    Component.onCompleted: {
                                        backend.setVideoSink(videoOutput.videoSink)
                                    }
                                }
                            }

                            // Audio Name (Track Title)
                            Text {
                                text: backend.title !== "" ? backend.title : "Audio Name"
                                color: "#ffffff"
                                font.pixelSize: 18
                                font.bold: true
                                Layout.alignment: Qt.AlignCenter
                                elide: Text.ElideRight
                                Layout.maximumWidth: parent.width - 40
                            }

                            // Artist Name
                            Text {
                                text: backend.artist !== "" ? backend.artist : "Unknown Artist"
                                color: "#a0a0a0"
                                font.pixelSize: 14
                                Layout.alignment: Qt.AlignCenter
                                elide: Text.ElideRight
                                Layout.maximumWidth: parent.width - 40
                            }
                        }
            // 3. RIGHT RADIO PANEL
            Rectangle {
                Layout.preferredWidth: 220
                Layout.fillHeight: true
                color: "#1a1a1e"
                border.color: "#2a2a30"
                radius: 8
                visible: window.isRadioPanelVisible

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    RowLayout {
                        Layout.fillWidth: true
                        Text {
                            text: "Radio Channels"
                            color: "#ffffff"
                            font.bold: true
                            font.pixelSize: 14
                        }
                        Item { Layout.fillWidth: true }
                        Text {
                            text: "+ Add URL"
                            color: "#e63946"
                            font.bold: true
                            font.pixelSize: 12
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: addRadioDialog.open()
                            }
                        }
                    }

                    ListView {
                        id: radioListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: backend.radioStations

                        delegate: Rectangle {
                            width: radioListView.width
                            height: 35
                            color: "transparent"
                            radius: 4

                            Text {
                                text: modelData.name
                                color: "#ffffff"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: backend.playRadioStation(modelData.name, modelData.url)
                            }
                        }
                    }
                }
            }
        }

        // ==========================================
        // BOTTOM CONTROLS SECTION
        // ==========================================
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12

            // Import / Repeat Row
            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.preferredWidth: 36
                    Layout.preferredHeight: 36
                    radius: 6
                    color: backend.isRepeatMode ? "#3b3b4a" : "transparent"
                    border.color: backend.isRepeatMode ? "#e63946" : "transparent"

                    Image {
                        source: "qrc:/icons/repeat.png"
                        anchors.centerIn: parent
                        width: 24; height: 24
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.toggleRepeat()
                    }
                }

                Item { Layout.fillWidth: true }

                Image {
                    source: "qrc:/icons/import.png"
                    Layout.preferredWidth: 36
                    Layout.preferredHeight: 36
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: fileDialog.open()
                    }
                }
            }

            // Timeline Slider
            Slider {
                id: timeSlider
                Layout.fillWidth: true
                visible: !backend.isRadioMode
                from: 0
                to: backend.duration > 0 ? backend.duration : 1
                value: backend.position
                onMoved: backend.setPosition(value)
            }

            // Start / End Time Labels
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: backend.isRadioMode ? "🔴 LIVE" : formatTime(backend.position)
                    color: backend.isRadioMode ? "#e63946" : "#b0b0b0"
                    font.pixelSize: 13
                    font.bold: backend.isRadioMode
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: backend.isRadioMode ? "" : "-" + formatTime(backend.duration - backend.position)
                    color: "#b0b0b0"
                    font.pixelSize: 13
                }
            }

            // Middle Playback Row
            RowLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: 35

                Image {
                    source: backend.isFavorite ? "qrc:/icons/liked.png" : "qrc:/icons/unlike.png"
                    Layout.preferredWidth: 32; Layout.preferredHeight: 32
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.toggleFavorite()
                    }
                }

                Image {
                    source: "qrc:/icons/prev.png"
                    Layout.preferredWidth: 48; Layout.preferredHeight: 48
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.playPrevTrack()
                    }
                }

                // Play / Pause Circle Button
                Rectangle {
                    Layout.preferredWidth: 70
                    Layout.preferredHeight: 70
                    radius: 35
                    color: playBtnMouse.containsMouse ? "#3b3b4a" : "#2a2a35"
                    border.color: "#ffffff"
                    border.width: 2

                    Image {
                        source: backend.isPlaying ? "qrc:/icons/pause.png" : "qrc:/icons/play.png"
                        anchors.centerIn: parent
                        width: 32; height: 32 // Size inside the Rectangle
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        id: playBtnMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.togglePlayPause()
                    }
                }

                Image {
                    source: "qrc:/icons/next.png"
                    Layout.preferredWidth: 48; Layout.preferredHeight: 48
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.playNextTrack()
                    }
                }

                Image {
                    source: backend.isDownloaded ? "qrc:/icons/downloaded.png" : "qrc:/icons/undownload.png"
                    Layout.preferredWidth: 32; Layout.preferredHeight: 32
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: backend.toggleDownload()
                    }
                }
            }

            // Bottom Grid Row (Audio Menu, Queue Button, Volume)
            RowLayout {
                Layout.fillWidth: true

                // Audio Select Menu
                ColumnLayout {
                    spacing: 4

                    Button {
                        id: audioSourceBtn
                        text: "AUDIO ⚙"
                        flat: true
                        contentItem: Text { text: audioSourceBtn.text; color: "#ffffff"; font.bold: true }
                        onClicked: audioMenu.open()

                        Menu {
                            id: audioMenu
                            y: audioSourceBtn.height

                            MenuItem {
                                text: "Download"
                                onClicked: {
                                    selectedAudioLabel.text = "Download"
                                    window.isRadioPanelVisible = false
                                    backend.setRadioMode(false)
                                }
                            }
                            MenuItem {
                                text: "Radio"
                                onClicked: {
                                    selectedAudioLabel.text = "Radio"
                                    window.isRadioPanelVisible = true
                                    backend.setRadioMode(true)
                                }
                            }
                            MenuItem {
                                text: "Bluetooth"
                                onClicked: selectedAudioLabel.text = "Bluetooth"
                            }
                            MenuItem {
                                                            text: "CDRom/USB"
                                                            onClicked: {
                                                                selectedAudioLabel.text = "CDRom/USB"
                                                                window.isRadioPanelVisible = false
                                                                window.isQueueVisible = true // Open Queue to show files
                                                                backend.scanExternalStorage() // Scan drives!
                                                            }
                                                        }

                        }
                    }

                    Text {
                        id: selectedAudioLabel
                        text: "Download"
                        color: "#e63946"
                        font.pixelSize: 11
                        font.bold: true
                    }
                }

                Item { Layout.fillWidth: true }

                // Queue Toggle Button
                Rectangle {
                    Layout.preferredWidth: 90
                    Layout.preferredHeight: 30
                    radius: 12
                    color: window.isQueueVisible ? "#ffffff" : "transparent"

                    Text {
                        text: "QUEUE"
                        color: window.isQueueVisible ? "#121214" : "#ffffff"
                        font.bold: true
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            window.isQueueVisible = !window.isQueueVisible
                            if (window.isQueueVisible) {
                                backend.setCurrentView(0)
                            }
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Volume Controls
                RowLayout {
                    spacing: 8

                    Image {
                        source: backend.isMuted ? "qrc:/icons/mute.png" : "qrc:/icons/Sound.png"
                        Layout.preferredWidth: 28; Layout.preferredHeight: 28
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: backend.toggleMute()
                        }
                    }

                    Slider {
                        id: volumeSlider
                        from: 0.0
                        to: 1.0
                        value: backend.volume
                        Layout.preferredWidth: 90
                        onMoved: backend.setVolume(value)
                    }
                }
            }
        }
    }
}
