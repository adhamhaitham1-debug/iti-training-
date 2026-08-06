import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 420
    height: 680
    visible: true
    title: qsTr("Smart Home Control")

    LayoutMirroring.enabled: Qt.uiLanguage === "ar"
    LayoutMirroring.childrenInherit: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginPage
    }

    Component {
        id: loginPage
        Page {
            title: qsTr("Lorgin")

            Column {
                anchors.centerIn: parent
                spacing: 15
                width: parent.width * 0.85


                Item {
                    width: 240
                    height: 150
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        text: "🏠"
                        font.pixelSize: 90
                        anchors.centerIn: parent
                        visible: logoImg.status !== Image.Ready
                    }

                    Image {
                        id: logoImg
                        source: "images/user_logo.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }
                }

                Label {
                    text: qsTr("Smart Home Control")
                    font.pixelSize: 22
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                TextField {
                    id: usernameField
                    placeholderText: qsTr("Username")
                    width: parent.width
                }

                TextField {
                    id: passwordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    width: parent.width
                }

                Label {
                    id: errorLabel
                    text: qsTr("Error: Both fields are required.")
                    color: "red"
                    visible: false
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Button {
                    text: qsTr("Login")
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        if (usernameField.text === "" || passwordField.text === "") {
                            errorLabel.visible = true
                        } else {
                            errorLabel.visible = false
                            busyIndicator.running = true
                            loginTimer.start()
                        }
                    }
                }

                BusyIndicator {
                    id: busyIndicator
                    running: false
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Timer {
                    id: loginTimer
                    interval: 2000
                    repeat: false
                    onTriggered: {
                        busyIndicator.running = false
                        stackView.push(dashboardPage)
                    }
                }
            }
        }
    }


    Component {
        id: dashboardPage
        Page {
            id: dashPage
            title: qsTr("My Devices")

            header: ToolBar {
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Label {
                        text: qsTr("My Devices")
                        font.pixelSize: 18
                        font.bold: true
                        Layout.fillWidth: true
                    }
                    Button {
                        text: qsTr("⚙ Settings")
                        onClicked: stackView.push(settingsPage)
                    }
                }
            }

            property var deviceList: [
                { name: qsTr("Living Room Light"), iconPath: "images/light.png", iconText: "💡", energy: 0.3 },
                { name: qsTr("Bedroom Light"), iconPath: "images/light.png", iconText: "💡", energy: 0.1 },
                { name: qsTr("Air Conditioner"), iconPath: "images/ac.png", iconText: "❄️", energy: 0.8 },
                { name: qsTr("Fan"), iconPath: "images/fan.png", iconText: "🌀", energy: 0.4 },
                { name: qsTr("Garage Door"), iconPath: "images/garage.png", iconText: "🚪", energy: 0.05 }
            ]

            ListView {
                anchors.fill: parent
                anchors.margins: 10
                model: dashPage.deviceList // Fixed: Safely targets the page's list
                spacing: 15
                clip: true

                delegate: ItemDelegate {
                    width: ListView.view.width
                    height: 70

                    RowLayout {
                        anchors.fill: parent
                        spacing: 15

                        Item {
                            width: 40
                            height: 40
                            Layout.alignment: Qt.AlignVCenter

                            Text {
                                text: modelData.iconText
                                font.pixelSize: 26
                                anchors.centerIn: parent
                                visible: deviceImg.status !== Image.Ready
                            }

                            Image {
                                id: deviceImg
                                source: modelData.iconPath
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5

                            Label {
                                text: modelData.name
                                font.bold: true
                                font.pixelSize: 15
                            }


                            ProgressBar {
                                value: modelData.energy
                                Layout.fillWidth: true
                            }
                        }

                        Switch {
                            Layout.alignment: Qt.AlignVCenter
                        }
                    }
                }
            }
        }
    }


    Component {
        id: settingsPage
        Page {
            title: qsTr("Settings")

            header: ToolBar {
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Button {
                        text: qsTr("◀ Back")
                        onClicked: stackView.pop()
                    }
                    Label {
                        text: qsTr("Settings")
                        font.pixelSize: 18
                        font.bold: true
                        Layout.fillWidth: true
                    }
                }
            }

            ScrollView {
                anchors.fill: parent
                contentWidth: availableWidth
                contentHeight: settingsColumn.implicitHeight + 40
                clip: true

                Column {
                    id: settingsColumn
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width * 0.85
                    spacing: 20
                    topPadding: 15

                    Column {
                        width: parent.width
                        spacing: 5
                        Label { text: qsTr("Language:"); font.bold: true }

                        ComboBox {
                            id: langCombo
                            width: parent.width
                            textRole: "text"
                            valueRole: "code"
                            model: [
                                { text: "English", code: "en" },
                                { text: "العربية (Arabic)", code: "ar" },
                                { text: "Español (Spanish)", code: "es" },
                                { text: "Français (French)", code: "fr" },
                                { text: "Deutsch (German)", code: "de" }
                            ]

                            Component.onCompleted: {
                                for (var i = 0; i < count; i++) {
                                    if (model[i].code === Qt.uiLanguage) {
                                        currentIndex = i;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    Column {
                        width: parent.width
                        spacing: 5
                        Label { text: qsTr("Screen Brightness") + " (" + Math.round(brightnessSlider.value) + "%):"; font.bold: true }
                        Slider {
                            id: brightnessSlider
                            width: parent.width
                            from: 0
                            to: 100
                            value: 75
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 10
                        CheckBox {
                            id: notifCheck
                            checked: true
                        }
                        Label {
                            text: qsTr("Enable Push Notifications")
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                        }
                    }

                    Button {
                        text: qsTr("Save Settings")
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {

                            Qt.uiLanguage = langCombo.currentValue
                            console.log("Language set to:", Qt.uiLanguage)

                            savedNotification.visible = true
                            hideNotifyTimer.restart()
                        }
                    }

                    Label {
                        id: savedNotification
                        text: qsTr("✓ Settings Saved Successfully!")
                        color: "green"
                        font.bold: true
                        visible: false
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Timer {
                        id: hideNotifyTimer
                        interval: 2500
                        repeat: false
                        onTriggered: savedNotification.visible = false
                    }
                }
            }
        }
    }
}