import QtQuick
import QtQuick.Controls

Window {
    width: 500
    height: 600
    visible: true
    title: "Info App"
    color: "#101923"

    property bool isCardVisible: false
    Button {
        id: showInfoBtn
        text: "Show My Info"
        font.pixelSize: 18
        font.bold: true
        width: 180
        height: 50
        anchors.centerIn: parent
        visible: !isCardVisible

        onClicked: {
            isCardVisible = true
            cardRevealAnim.start()
        }
    }

    Rectangle {
        id: infoCard
        width: 400
        height: 450
        anchors.centerIn: parent
        radius: 18
        color: "#ffffff"
        border.color: "#d0d7de"
        border.width: 1

        visible: isCardVisible
        opacity: 0
        scale: 0.8

        ParallelAnimation {
            id: cardRevealAnim
            NumberAnimation { target: infoCard; property: "opacity"; to: 1; duration: 400 }
            NumberAnimation { target: infoCard; property: "scale"; to: 1; duration: 500; easing.type: Easing.OutBack }
        }

        Column {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 20

            Text {
                text: "Hello, I am"
                font.pixelSize: 28
                font.bold: true
                color: "#24292f"
                opacity: 0

                Timer {
                    running: isCardVisible
                    interval: 400
                    onTriggered: parent.opacity = 1
                }
                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            Repeater {
                model: ListModel {
                    ListElement { label: "Name:"; value: "Adham Haitham" }
                    ListElement { label: "Role:"; value: "Student" }
                    ListElement { label: "University:"; value: "MSA University" }
                    ListElement { label: "City:"; value: "Cairo" }
                    ListElement { label: "Address:"; value: "Madinty" }
                }

                delegate: Component {
                    Row {
                        id: rowDelegate
                        spacing: 10
                        opacity: 0

                        transform: Translate {
                            id: yTransform
                            y: -15

                            Behavior on y {
                                NumberAnimation { duration: 400; easing.type: Easing.OutBack }
                            }
                        }

                        Text {
                            text: model.label
                            font.pixelSize: 17
                            color: "#57606a"
                            width: 110
                        }

                        Text {
                            text: model.value
                            font.pixelSize: 18
                            font.bold: true
                            color: "#0969da"
                        }

                        Timer {
                            running: isCardVisible
                            // Calculates delay based on the row's index
                            interval: 600 + (index * 200)
                            onTriggered: {
                                rowDelegate.opacity = 1
                                yTransform.y = 0
                            }
                        }

                        Behavior on opacity {
                            NumberAnimation { duration: 400 }
                        }
                    }
                }
            }
        }
    }
}