import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Window {
    id: root
    width: 1024
    height: 768
    visible: true
    title: qsTr("Domz Cars - Luxury Dealership")

    LayoutMirroring.enabled: Qt.uiLanguage === "ar"
    LayoutMirroring.childrenInherit: true

    property int currentTheme: 0

    property var themes: [
        { name: "Beige", bg: "#F5F2EB", bar: "#FFFFFF", text: "#1A1A1A", subText: "#4A4A4A", card: "#FFFFFF", border: "#E2E8F0", accent: "#630015" },
        { name: "Navy", bg: "#0B132B", bar: "#111A3A", text: "#F8FAFC", subText: "#94A3B8", card: "#1C2541", border: "#2E3A59", accent: "#D4AF37" },
        { name: "Burgundy", bg: "#4A0E17", bar: "#33080F", text: "#F8FAFC", subText: "#E0B0B6", card: "#5A111C", border: "#7A1726", accent: "#111A3A" }
    ]

    property var activeTheme: themes[currentTheme]
    color: activeTheme.bg
    Behavior on color { ColorAnimation { duration: 400 } }

    property real cartTotal: 0.0

    property var allCarsData: [
        { name: "Rolls-Royce Phantom", price: 450000, img: "images/car1.jpg", desc: "The ultimate luxury sedan with unparalleled comfort.", engine: "6.75L Twin-Turbo V12", hp: "563 hp", zeroToSixty: "5.1s", topSpeed: "155 mph" },
        { name: "Bentley Continental", price: 235000, img: "images/car2.jpg", desc: "Grand touring at its absolute finest.", engine: "6.0L Twin-Turbo W12", hp: "650 hp", zeroToSixty: "3.5s", topSpeed: "208 mph" },
        { name: "Mercedes-Maybach", price: 185000, img: "images/car3.jpg", desc: "First-class travel on the road.", engine: "4.0L V8 Biturbo", hp: "496 hp", zeroToSixty: "4.7s", topSpeed: "155 mph" },
        { name: "Aston Martin DB11", price: 205000, img: "images/car4.jpg", desc: "Beautifully powerful and exquisitely crafted.", engine: "5.2L Twin-Turbo V12", hp: "630 hp", zeroToSixty: "3.7s", topSpeed: "208 mph" }
    ]

    ListModel { id: displayCarsModel }
    ListModel { id: cartModel }

    Component.onCompleted: {
        for(var i = 0; i < allCarsData.length; i++) {
            displayCarsModel.append(allCarsData[i])
        }
    }

    Rectangle {
        id: topBar
        width: parent.width
        height: 75
        color: activeTheme.bar
        border.color: activeTheme.border
        border.width: 1
        z: 2
        Behavior on color { ColorAnimation { duration: 400 } }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 20

            RowLayout {
                spacing: 12
                Rectangle {
                    width: 65; height: 42; radius: 8; clip: true; color: activeTheme.border
                    Image { source: "images/domz_logo.png"; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
                }
                Text { text: qsTr("Domz Cars"); font.pixelSize: 26; font.bold: true; color: activeTheme.text }
            }

            Item { Layout.preferredWidth: 20 }

            TextField {
                Layout.fillWidth: true
                Layout.maximumWidth: 350
                placeholderText: qsTr("Search luxury cars...")
                font.pixelSize: 14
                color: currentTheme === 0 ? "#000000" : "#FFFFFF"
                background: Rectangle {
                    radius: 5
                    border.color: activeTheme.border
                    color: currentTheme === 0 ? "#F8FAFC" : "#1E293B"
                }
                onTextEdited: {
                    displayCarsModel.clear()
                    var query = text.toLowerCase()
                    for(var i = 0; i < allCarsData.length; i++) {
                        if(allCarsData[i].name.toLowerCase().indexOf(query) !== -1 ||
                           allCarsData[i].desc.toLowerCase().indexOf(query) !== -1) {
                            displayCarsModel.append(allCarsData[i])
                        }
                    }
                }
            }

            Item { Layout.fillWidth: true }

            Row {
                spacing: 10
                Repeater {
                    model: 3
                    delegate: Rectangle {
                        width: 24; height: 24; radius: 12
                        color: index === 0 ? "#F5F2EB" : (index === 1 ? "#0B132B" : "#4A0E17")
                        border.color: currentTheme === index ? "#10B981" : "#A0A0A0"
                        border.width: currentTheme === index ? 2 : 1
                        MouseArea { anchors.fill: parent; onClicked: currentTheme = index }
                    }
                }
            }

            Item { Layout.preferredWidth: 10 }

            Rectangle {
                Layout.preferredWidth: 120
                Layout.preferredHeight: 45
                color: cartBtnArea.containsMouse ? activeTheme.card : "transparent"
                border.color: cartBtnArea.containsMouse ? activeTheme.border : "transparent"
                radius: 8

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 5
                    Text { text: "🛒 " + qsTr("Cart:"); color: activeTheme.text }
                    Text { text: "$" + cartTotal.toLocaleString(Qt.locale(), 'f', 0); font.bold: true; color: "#10b981" }
                }

                MouseArea {
                    id: cartBtnArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: cartPopup.open()
                }
            }

            ComboBox {
                id: langCombo
                model: [ { text: "English", code: "en" }, { text: "العربية", code: "ar" }, { text: "Türkçe", code: "tr" } ]
                textRole: "text"
                valueRole: "code"
                Component.onCompleted: {
                    for (var i = 0; i < count; i++) {
                        if (model[i].code === Qt.uiLanguage) { currentIndex = i; break }
                    }
                }
                onActivated: (index) => {
                    Qt.uiLanguage = model[index].code
                }
            }
        }
    }

    ScrollView {
        id: mainScroll
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true
        contentWidth: availableWidth

        ColumnLayout {
            width: mainScroll.availableWidth - 40
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Item { Layout.preferredHeight: 10 }

            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: 280; radius: 12; color: "#1A1A1A"; clip: true
                Image { source: "images/banner.jpg"; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#44000000" }
                        GradientStop { position: 1.0; color: "#dd000000" }
                    }
                }
                ColumnLayout {
                    anchors.centerIn: parent; spacing: 10
                    Text { text: qsTr("L U X U R Y   A W A I T S"); font.pixelSize: 18; font.letterSpacing: 4; color: "#E2E8F0"; Layout.alignment: Qt.AlignHCenter }
                    Text { text: qsTr("Find Your Dream Car"); font.pixelSize: 42; font.bold: true; color: "#FFFFFF"; Layout.alignment: Qt.AlignHCenter }
                }
            }

            Text {
                text: qsTr("Featured Luxury Vehicles")
                font.pixelSize: 22; font.bold: true; color: activeTheme.text; Layout.topMargin: 15
            }

            GridLayout {
                columns: mainScroll.availableWidth > 1050 ? 4 : (mainScroll.availableWidth > 780 ? 3 : (mainScroll.availableWidth > 500 ? 2 : 1))
                columnSpacing: 15; rowSpacing: 20
                Layout.fillWidth: true; Layout.alignment: Qt.AlignHCenter

                Repeater {
                    model: displayCarsModel

                    delegate: Component {
                        Rectangle {
                            width: 230; height: 330; radius: 10
                            color: activeTheme.card; border.color: activeTheme.border; border.width: 1
                            Behavior on color { ColorAnimation { duration: 400 } }

                            ColumnLayout {
                                anchors.fill: parent; spacing: 0

                                Rectangle {
                                    Layout.fillWidth: true; Layout.preferredHeight: 160
                                    color: activeTheme.border; radius: 10; clip: true

                                    Image { source: model.img; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            carDetailPopup.carName = qsTr(model.name)
                                            carDetailPopup.carPrice = model.price
                                            carDetailPopup.carDesc = qsTr(model.desc)
                                            carDetailPopup.carImg = model.img
                                            carDetailPopup.carEngine = model.engine
                                            carDetailPopup.carHp = model.hp
                                            carDetailPopup.carZeroToSixty = model.zeroToSixty
                                            carDetailPopup.carTopSpeed = model.topSpeed
                                            carDetailPopup.open()
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true; Layout.fillHeight: true; Layout.margins: 14; spacing: 6
                                    Text { text: qsTr(model.name); font.pixelSize: 16; font.bold: true; color: activeTheme.text; Layout.fillWidth: true; wrapMode: Text.WordWrap }
                                    Text { text: "$" + model.price.toLocaleString(Qt.locale(), 'f', 0); font.pixelSize: 16; color: "#10b981" }
                                    Item { Layout.fillHeight: true }

                                    Button {
                                        text: qsTr("Add to Cart")
                                        Layout.fillWidth: true
                                        onClicked: {
                                            root.cartTotal += model.price
                                            cartModel.append({ "cName": model.name, "cPrice": model.price, "cImg": model.img })
                                        }
                                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                                        background: Rectangle { color: parent.down ? "#059669" : activeTheme.accent; radius: 6 }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item { Layout.preferredHeight: 40 }
        }
    }

    Popup {
        id: carDetailPopup
        width: 650; height: 450; anchors.centerIn: parent; modal: true; focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property string carName: ""; property real carPrice: 0.0; property string carDesc: ""; property string carImg: ""
        property string carEngine: ""; property string carHp: ""; property string carZeroToSixty: ""; property string carTopSpeed: ""

        background: Rectangle { radius: 12; color: activeTheme.card; border.color: activeTheme.border }
        enter: Transition { NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200 } }
        exit: Transition { NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 200 } }

        RowLayout {
            anchors.fill: parent; anchors.margins: 20; spacing: 25
            Rectangle {
                Layout.preferredWidth: 300; Layout.fillHeight: true; color: activeTheme.border; radius: 8; clip: true
                Image { source: carDetailPopup.carImg; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
            }
            ColumnLayout {
                Layout.fillWidth: true; Layout.fillHeight: true; spacing: 12
                Text { text: carDetailPopup.carName; font.pixelSize: 26; font.bold: true; color: activeTheme.text; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                Text { text: "$" + carDetailPopup.carPrice.toLocaleString(Qt.locale(), 'f', 0); font.pixelSize: 22; color: "#10b981"; font.bold: true }
                Text { text: carDetailPopup.carDesc; font.pixelSize: 15; color: activeTheme.subText; wrapMode: Text.WordWrap; Layout.fillWidth: true }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: activeTheme.border; Layout.topMargin: 5; Layout.bottomMargin: 5 }

                GridLayout {
                    columns: 2; rowSpacing: 10; columnSpacing: 15; Layout.fillWidth: true
                    Text { text: qsTr("Engine:"); font.bold: true; color: activeTheme.subText }
                    Text { text: carDetailPopup.carEngine; color: activeTheme.text }
                    Text { text: qsTr("Horsepower:"); font.bold: true; color: activeTheme.subText }
                    Text { text: carDetailPopup.carHp; color: activeTheme.text }
                    Text { text: qsTr("0-60 mph:"); font.bold: true; color: activeTheme.subText }
                    Text { text: carDetailPopup.carZeroToSixty; color: activeTheme.text }
                    Text { text: qsTr("Top Speed:"); font.bold: true; color: activeTheme.subText }
                    Text { text: carDetailPopup.carTopSpeed; color: activeTheme.text }
                }
                Item { Layout.fillHeight: true }
                RowLayout {
                    Layout.fillWidth: true; spacing: 10
                    Button { text: qsTr("Close"); Layout.fillWidth: true; onClicked: carDetailPopup.close() }
                    Button {
                        text: qsTr("Add to Cart")
                        Layout.fillWidth: true
                        onClicked: {
                            root.cartTotal += carDetailPopup.carPrice
                            cartModel.append({ "cName": carDetailPopup.carName, "cPrice": carDetailPopup.carPrice, "cImg": carDetailPopup.carImg })
                            carDetailPopup.close()
                        }
                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                        background: Rectangle { color: activeTheme.accent; radius: 6 }
                    }
                }
            }
        }
    }

    Popup {
        id: cartPopup
        width: 450; height: 550; anchors.centerIn: parent; modal: true; focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        background: Rectangle { radius: 12; color: activeTheme.card; border.color: activeTheme.border }
        enter: Transition { NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200 } }
        exit: Transition { NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 200 } }

        ColumnLayout {
            anchors.fill: parent; anchors.margins: 20; spacing: 15

            Text { text: qsTr("Your Cart"); font.pixelSize: 24; font.bold: true; color: activeTheme.text }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: activeTheme.border }

            Text {
                text: qsTr("Your cart is empty.")
                font.pixelSize: 16; color: activeTheme.subText
                visible: cartModel.count === 0
                Layout.alignment: Qt.AlignHCenter; Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
            }

            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                clip: true
                model: cartModel
                spacing: 10
                visible: cartModel.count > 0

                delegate: Rectangle {
                    width: ListView.view.width; height: 80
                    color: "transparent"; border.color: activeTheme.border; radius: 8

                    RowLayout {
                        anchors.fill: parent; anchors.margins: 10; spacing: 15

                        Rectangle {
                            Layout.preferredWidth: 90; Layout.fillHeight: true; radius: 6; clip: true; color: activeTheme.border
                            Image { source: model.cImg; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true; spacing: 4
                            Text { text: model.cName; font.bold: true; font.pixelSize: 16; color: activeTheme.text; Layout.fillWidth: true; elide: Text.ElideRight }
                            Text { text: "$" + model.cPrice.toLocaleString(Qt.locale(), 'f', 0); color: "#10b981"; font.pixelSize: 14 }
                        }

                        Button {
                            text: "X"
                            Layout.preferredWidth: 35; Layout.preferredHeight: 35
                            background: Rectangle { color: "#EF4444"; radius: 4 }
                            contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                            onClicked: {
                                root.cartTotal -= model.cPrice
                                cartModel.remove(index)
                            }
                        }
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: activeTheme.border }

            RowLayout {
                Layout.fillWidth: true
                Text { text: qsTr("Total:"); font.pixelSize: 20; font.bold: true; color: activeTheme.text }
                Item { Layout.fillWidth: true }
                Text { text: "$" + root.cartTotal.toLocaleString(Qt.locale(), 'f', 0); font.pixelSize: 22; font.bold: true; color: "#10b981" }
            }

            RowLayout {
                Layout.fillWidth: true; spacing: 10; Layout.topMargin: 10
                Button {
                    text: qsTr("Keep Shopping")
                    Layout.fillWidth: true; onClicked: cartPopup.close()
                }
                Button {
                    text: qsTr("Checkout")
                    Layout.fillWidth: true
                    enabled: cartModel.count > 0
                    background: Rectangle { color: enabled ? activeTheme.accent : activeTheme.border; radius: 6 }
                    contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                    onClicked: {
                        cartModel.clear()
                        root.cartTotal = 0
                        cartPopup.close()
                    }
                }
            }
        }
    }

    Rectangle {
        id: splashScreen
        anchors.fill: parent; color: activeTheme.bg; z: 100
        ColumnLayout {
            anchors.centerIn: parent; spacing: 20
            Rectangle {
                Layout.preferredWidth: 220; Layout.preferredHeight: 130; Layout.alignment: Qt.AlignHCenter
                radius: 12; clip: true; color: "transparent"
                Image { source: "images/domz_logo.png"; anchors.fill: parent; fillMode: Image.PreserveAspectCrop }
                transform: Translate { id: carBounce }
                SequentialAnimation {
                    running: true; loops: Animation.Infinite
                    NumberAnimation { target: carBounce; property: "y"; from: 0; to: -15; duration: 600; easing.type: Easing.OutQuad }
                    NumberAnimation { target: carBounce; property: "y"; from: -15; to: 0; duration: 600; easing.type: Easing.InQuad }
                }
            }
            Text { text: qsTr("Domz Cars"); font.pixelSize: 48; font.bold: true; color: activeTheme.text; Layout.alignment: Qt.AlignHCenter }
            Text { text: qsTr("Preparing your luxury experience..."); font.pixelSize: 18; color: activeTheme.subText; Layout.alignment: Qt.AlignHCenter }
        }
        Timer { interval: 3000; running: true; repeat: false; onTriggered: hideSplash.start() }
        ParallelAnimation {
            id: hideSplash
            NumberAnimation { target: splashScreen; property: "opacity"; to: 0; duration: 800; easing.type: Easing.InOutQuad }
            NumberAnimation { target: splashScreen; property: "scale"; to: 1.1; duration: 800 }
            onFinished: splashScreen.visible = false
        }
    }
}