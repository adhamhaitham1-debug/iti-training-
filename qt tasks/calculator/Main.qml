import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import calculator 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 380
    height: 680
    minimumWidth: 320
    minimumHeight: 520
    title: "DOMZ Calculator"
    color: "#000000"

    CalculatorBackend {
        id: backend
    }


    Item {
        id: container
        width: Math.min(parent.width - 32, 420)
        height: Math.min(parent.height - 32, 720)
        anchors.centerIn: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 16


            RowLayout {
                Layout.fillWidth: true

                Button {
                    flat: true
                    contentItem: Text { text: "≡"; color: "#888888"; font.pixelSize: 22 }
                    background: Rectangle { color: "transparent" }
                }

                Item { Layout.fillWidth: true }

                Button {
                    flat: true
                    contentItem: Text { text: "⊟"; color: "#888888"; font.pixelSize: 18 }
                    background: Rectangle { color: "transparent" }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom
                spacing: 4

                Text {
                    text: backend.expression
                    color: "#888888"
                    font.pixelSize: 24
                    font.weight: Font.Light
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    elide: Text.ElideLeft
                }

                Text {
                    text: backend.display
                    color: "#FFFFFF"
                    font.pixelSize: Math.min(container.width * 0.2, 72)
                    font.weight: Font.Light
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 32
                }
            }


            GridLayout {
                id: grid
                columns: 4
                rows: 5
                rowSpacing: 12
                columnSpacing: 12
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter


                property real btnSize: Math.floor((container.width - (columnSpacing * (columns - 1))) / columns)


                component CalcButton: Button {
                    id: btn
                    property string btnColor: "#333333"
                    property string textColor: "#FFFFFF"


                    Layout.preferredWidth: grid.btnSize
                    Layout.preferredHeight: grid.btnSize

                    background: Rectangle {
                        color: btn.pressed ? Qt.lighter(btn.btnColor, 1.4) : btn.btnColor
                        radius: width / 2 // Guarantees a perfect circle
                    }

                    contentItem: Text {
                        text: btn.text
                        color: btn.textColor
                        font.pixelSize: btn.height * 0.35
                        font.weight: Font.Medium
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }


                CalcButton { text: "⌫"; btnColor: "#A5A5A5"; textColor: "#000000"; onClicked: backend.backspacePressed() }
                CalcButton { text: "AC"; btnColor: "#A5A5A5"; textColor: "#000000"; onClicked: backend.clearPressed() }
                CalcButton { text: "%"; btnColor: "#A5A5A5"; textColor: "#000000"; onClicked: backend.percentPressed() }
                CalcButton { text: "÷"; btnColor: "#FF9F0A"; textColor: "#FFFFFF"; onClicked: backend.operatorPressed("÷") }


                CalcButton { text: "7"; onClicked: backend.digitPressed("7") }
                CalcButton { text: "8"; onClicked: backend.digitPressed("8") }
                CalcButton { text: "9"; onClicked: backend.digitPressed("9") }
                CalcButton { text: "×"; btnColor: "#FF9F0A"; textColor: "#FFFFFF"; onClicked: backend.operatorPressed("×") }


                CalcButton { text: "4"; onClicked: backend.digitPressed("4") }
                CalcButton { text: "5"; onClicked: backend.digitPressed("5") }
                CalcButton { text: "6"; onClicked: backend.digitPressed("6") }
                CalcButton { text: "−"; btnColor: "#FF9F0A"; textColor: "#FFFFFF"; onClicked: backend.operatorPressed("-") }


                CalcButton { text: "1"; onClicked: backend.digitPressed("1") }
                CalcButton { text: "2"; onClicked: backend.digitPressed("2") }
                CalcButton { text: "3"; onClicked: backend.digitPressed("3") }
                CalcButton { text: "+"; btnColor: "#FF9F0A"; textColor: "#FFFFFF"; onClicked: backend.operatorPressed("+") }


                CalcButton { text: "+/-"; onClicked: backend.toggleSignPressed() }
                CalcButton { text: "0"; onClicked: backend.digitPressed("0") }
                CalcButton { text: "."; onClicked: backend.digitPressed(".") }
                CalcButton { text: "="; btnColor: "#FF9F0A"; textColor: "#FFFFFF"; onClicked: backend.equalsPressed() }
            }
        }
    }
}