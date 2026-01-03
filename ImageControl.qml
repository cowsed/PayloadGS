import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: page

    property int activeImageID: 1

    Material.theme: Material.Light
    RowLayout {
        anchors.fill: parent
        Image {
            Layout.preferredWidth: 512
            Layout.preferredHeight: 512
            fillMode: Image.PreserveAspectFit
            source: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/Images/"
                    + page.activeImageID + "/sofar.png"
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            ColumnLayout {
                Layout.horizontalStretchFactor: 1
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                spacing: 1
                // Rectangle {
                // anchors.fill: parent
                // color: "purple"
                // }
                Label {
                    text: "Images"
                }

                ScrollView {
                    Layout.fillHeight: true
                    // Layout.fillWidth: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.interactive: false
                    padding: 5

                    ColumnLayout {
                        spacing: 0
                        ImagePreview {
                            activeHolder: page
                            activeImageID: page.activeImageID
                            imageID: 0
                            transmissionPercent: 12
                            dir: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/Images/0/"
                        }
                        ImagePreview {
                            activeHolder: page
                            activeImageID: page.activeImageID
                            imageID: 1
                            transmissionPercent: 5
                            dir: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/Images/1/"
                        }
                        ImagePreview {
                            activeHolder: page
                            activeImageID: page.activeImageID
                            imageID: 2
                            transmissionPercent: 1
                            dir: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/Images/2/"
                        }
                        ImagePreview {
                            activeHolder: page
                            activeImageID: page.activeImageID
                            imageID: 3
                            transmissionPercent: 2
                            dir: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/WorkingDir/Images/3/"
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.horizontalStretchFactor: 4
                Layout.fillHeight: true
                Layout.fillWidth: true
                Button {
                    Layout.fillWidth: true
                    text: "Download More"
                }
            }
        }
    }
}
