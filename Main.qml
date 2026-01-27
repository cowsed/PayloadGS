import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtPositioning

ApplicationWindow {
    id: mainwindow
    width: 1024
    height: 600
    visible: true
    title: qsTr("Payload Groundstation")

    Material.theme: Material.Light
    Material.accent: Material.Orange

    Material.primary: Qt.rgba(208, 211, 212)
    Material.foreground: Qt.rgba(0, 0, 0, 1)
    Material.background: Qt.rgba(1, 1, 1, 1)

    font.family: "Roboto"
    font.weight: Font.ExtraBold
    font.pointSize: 13

    property geoCoordinate payloadCoordinate: QtPositioning.coordinate(
                                                  43.082737, -77.669240)

    property geoCoordinate stationCoordinate: QtPositioning.coordinate(
                                                  43.083938, -77.675772)
    property geoCoordinate rocketCoordinate: QtPositioning.coordinate(
                                                 43.085308, -77.679096)

    property date payloadCoordinateUpdateTime: new Date()
    property date stationCoordinateUpdateTime: new Date()
    property date rocketCoordinateUpdateTime: new Date()

    property real ramUsed: 11986268160
    property real ramAvail: 20981673984
    property real fsUsed: 273862541312
    property real fsAvail: 17535967232
    property real batteryVoltage: 12.3
    property real batteryCurrent: 1234

    property int flightNumber: 0
    property date lastFlightNumberUpdate: new Date()

    property string stateString: "Pad"

    Timer {
        interval: 10
        running: true // set to true for fun
        repeat: true
        onTriggered: {
            let t = new Date().getTime() / 400
            mainwindow.rocketCoordinate.latitude = 43.085308 + 0.001 * Math.sin(
                        t)

            mainwindow.payloadCoordinate.longitude = -77.669240 + 0.001 * Math.sin(
                        t)
        }
    }

    property real initTab: 3

    header: TabBar {
        id: tabbar
        Layout.fillWidth: true
        currentIndex: mainwindow.initTab

        TabButton {
            id: mapTabButton
            text: "Map"
        }
        TabButton {
            id: controlTabButton
            text: "Control"
        }
        TabButton {
            id: telemTabButton
            text: "Telemetry"
        }
        TabButton {
            id: armTabButton
            text: "Arm"
        }
        TabButton {
            id: imageTabButton
            text: "Image"
        }
        TabButton {
            id: radioTabButton
            text: "Radio"
        }
        TabButton {
            id: shellTabButton
            text: "Shell"
        }

        Shortcut {
            sequences: ["Alt+m", "Alt+1"]
            onActivated: mapTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+c", "Alt+2"]
            onActivated: controlTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+t", "Alt+3"]
            onActivated: telemTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+a", "Alt+4"]
            onActivated: armTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+i", "Alt+5"]
            onActivated: imageTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+r", "Alt+6"]
            onActivated: radioTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+s", "Alt+7"]
            onActivated: shellTabButton.click()
        }

        onCurrentIndexChanged: {
            mainview.currentIndex = currentIndex
        }
    }

    ColumnLayout {
        anchors.fill: parent

        SwipeView {
            id: mainview

            currentIndex: mainwindow.initTab
            Layout.fillWidth: true
            Layout.fillHeight: true

            onCurrentIndexChanged: {
                tabbar.currentIndex = currentIndex
            }

            MapPage {
                Layout.fillWidth: true
                Layout.fillHeight: true

                payloadCoordinate: mainwindow.payloadCoordinate
                payloadCoordinateUpdateTime: mainwindow.payloadCoordinateUpdateTime

                stationCoordinate: mainwindow.stationCoordinate
                stationCoordinateUpdateTime: mainwindow.payloadCoordinateUpdateTime

                rocketCoordinate: mainwindow.rocketCoordinate
                rocketCoordinateUpdateTime: mainwindow.payloadCoordinateUpdateTime
            }

            ControlPage {
                id: controlPage
                Layout.fillWidth: true
                Layout.fillHeight: true
                flightNumber: mainwindow.flightNumber
                lastFlightNumberUpdate: mainwindow.lastFlightNumberUpdate
                stateString: mainwindow.stateString
            }

            TelemetryPage {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ramUsed: mainwindow.ramUsed
                ramAvail: mainwindow.ramAvail
                fsUsed: mainwindow.fsUsed
                fsAvail: mainwindow.fsAvail

                batteryVoltage: mainwindow.batteryVoltage
                batteryCurrent: mainwindow.batteryCurrent
            }
            ArmPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ImageControl {
                flightNumber: mainwindow.flightNumber
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RadioPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ShellPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
    footer: ToolBar {
        font.weight: Font.Normal
        font.pointSize: 12

        implicitHeight: 25
        RowLayout {
            anchors.fill: parent
            Layout.bottomMargin: 0
            Layout.topMargin: 0
            Layout.fillHeight: true

            Label {
                text: mainwindow.stateString
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
            }
            ToolSeparator {}

            Label {
                text: "Bat: " + mainwindow.batteryVoltage.toFixed(2) + "V"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}

            Label {
                text: "SF7"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "BW 125"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "CR  4/8"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "F 433.125 MHz"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            TimeSinceThing {
                desc: "RX: "
                event_time: new Date()
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            TimeSinceThing {
                desc: "TX: "
                event_time: new Date()
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "Rem: 4"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}

            ToolButton {
                Layout.maximumHeight: 20
                text: qsTr("⏸")
                Layout.alignment: Qt.AlignTop
            }
        }
    }
}
