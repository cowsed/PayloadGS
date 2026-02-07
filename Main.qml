import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ApplicationWindow {
    id: mainwindow
    width: 1920 / 2
    height: 1080 / 2

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

    LoraSettings {
        id: defaultRadioSettings
        spreadingFactor: LoraSettings.SF7
        bandwidth: LoraSettings.BW125
        codingRate: LoraSettings.CR4_8
        frequency: 433000000
    }

    Component.onCompleted: {
        console.log("Loaded")
        console.log("Latest geopos", TelemetryLogHolder.latestPayloadPosition
                    + " " + TelemetryLogHolder.latestPayloadPositionUpdateTime)
    }

    property LoraSettings currentRadioSettings: defaultRadioSettings

    property int flightNumber: 0
    property date lastFlightNumberUpdate: new Date()

    property string stateString: "Pad"

    function toggleFullscreen() {
        if (mainwindow.visibility === Window.FullScreen) {
            mainwindow.visibility = Window.Windowed
        } else {
            mainwindow.visibility = Window.FullScreen
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
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: controlTabButton
            text: "Control"
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: telemTabButton
            text: "Telemetry"
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: armTabButton
            text: "Arm"
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: imageTabButton
            text: "Image"
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: radioTabButton
            text: "Radio"
            onClicked: metaview.currentIndex = 0
        }
        TabButton {
            id: shellTabButton
            text: "Shell"
            onClicked: metaview.currentIndex = 0
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
        Shortcut {
            sequences: ["Alt+g"]
            onActivated: metaview.currentIndex = 1
        }

        onCurrentIndexChanged: {
            mainview.currentIndex = currentIndex
        }
    }

    SwipeView {
        id: metaview
        anchors.fill: parent
        orientation: Qt.Vertical

        SwipeView {
            id: mainview
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: mainwindow.initTab

            // Layout.fillWidth: true
            // Layout.fillHeight: true
            onCurrentIndexChanged: {
                tabbar.currentIndex = currentIndex
            }

            MapPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ControlPage {
                id: controlPage
                Layout.fillWidth: true
                Layout.fillHeight: true

                window: mainwindow
                flightNumber: mainwindow.flightNumber
                lastFlightNumberUpdate: mainwindow.lastFlightNumberUpdate
                stateString: mainwindow.stateString
            }

            TelemetryPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
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

                currentSettings: defaultRadioSettings
            }

            ShellPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        GSPage {
            Layout.fillWidth: true
            Layout.fillHeight: true

            fullscreenToggle: mainwindow.toggleFullscreen
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
                text: mainwindow.currentRadioSettings.spreadingFactorString(
                          mainwindow.currentRadioSettings.spreadingFactor)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: mainwindow.currentRadioSettings.bandwidthString(
                          mainwindow.currentRadioSettings.bandwidth)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: mainwindow.currentRadioSettings.codingRateString(
                          mainwindow.currentRadioSettings.codingRate)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "F " + (mainwindow.currentRadioSettings.frequency / 1000000).toFixed(
                          3) + " MHz"
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
