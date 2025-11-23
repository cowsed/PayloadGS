import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainwindow
    width: 1024
    height: 600
    visible: true
    title: qsTr("Payload Groundstation")

    property real initTab: 2

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tabbar
            Layout.fillWidth: true
            currentIndex: mainwindow.initTab
            TabButton {
                text: "Arm"
            }
            TabButton {
                text: "Image"
            }
            TabButton {
                text: "Telemetry"
            }
            onCurrentIndexChanged: {
                mainview.currentIndex = currentIndex
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
                id: firstPage
                Label {
                    text: "Arm page"
                }
            }
            Item {
                ScrollView {
                    id: secondPage
                    ColumnLayout {
                        Label {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "Image page"
                        }
                        Image {
                            source: "assets/maps/textureData.png"
                        }
                    }
                }
            }
            Item {
                id: thirdPage
                TelemetryPage {}
            }
        }
    }
}
