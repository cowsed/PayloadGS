import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

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

    property real initTab: 3

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true

            // width: tabbar.width + 10
            // height: tabbar.height + 10
            TabBar {
                id: tabbar
                Layout.fillWidth: true
                currentIndex: mainwindow.initTab
                TabButton {
                    text: "Arm"
                }
                ToolSeparator {}
                TabButton {
                    text: "Image"
                }
                TabButton {
                    text: "Map"
                }
                TabButton {
                    text: "Telemetry"
                }
                TabButton {
                    text: "Radio"
                }
                TabButton {
                    text: "Shell"
                }
                onCurrentIndexChanged: {
                    mainview.currentIndex = currentIndex
                }
            }
        }

        SwipeView {
            id: mainview

            currentIndex: mainwindow.initTab
            Layout.fillWidth: true
            Layout.fillHeight: true

            onCurrentIndexChanged: {
                tabbar.currentIndex = currentIndex
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ArmPage {}
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ImageControl {}
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                MapPage {}
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TelemetryPage {
                    ramUsed: 12713
                    ramAvail: 18727
                    fsUsed: 273322987520
                    fsAvail: 18075521024
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                RadioPage {}
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ShellPage {}
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
                text: "Status"
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
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
                desc: "RX"
                event_time: new Date()
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            TimeSinceThing {
                desc: "TX"
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
