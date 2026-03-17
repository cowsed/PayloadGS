pragma ComponentBehavior: Bound

import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: page

    required property int flightNumber
    property int activeImageID: 0

    property imageMetadataHolder activeImageMetadata: ImageDataHolder.metadataForImageId(
                                                          activeImageID)

    property real numPackets: ImageDataHolder.numDownloadedPackets(
                                  activeImageID)

    Connections {
        target: ImageDataHolder
        function onImageUpdated(image_id) {
            if (image_id == page.activeImageID) {
                activeImage.reload()
            }
            if (previews.itemAt(image_id)) {
                previews.itemAt(image_id).reload()
            }
            page.activeImageMetadata = ImageDataHolder.metadataForImageId(
                        page.activeImageID)
        }
    }

    Material.theme: Material.Light
    RowLayout {
        anchors.fill: parent
        Image {
            id: activeImage
            cache: false
            Layout.preferredWidth: 512 - 60
            Layout.preferredHeight: 512 - 60
            fillMode: Image.PreserveAspectFit

            source: "file:" + ImageDataHolder.pathForImage(page.activeImageID)

            function reload() {
                const oldSource = activeImage.source
                activeImage.source = ""
                activeImage.source = oldSource
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            // Layout.alignment: Qt.AlignLeft
            ColumnLayout {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                // Layout.alignment: Qt.AlignTop
                spacing: 1

                Label {
                    text: "Images: " + ImageDataHolder.numImages
                }

                ScrollView {
                    Layout.fillHeight: true
                    // Layout.fillWidth: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.interactive: false
                    padding: 5

                    ColumnLayout {
                        spacing: 0
                        Repeater {
                            id: previews
                            model: ImageDataHolder.numImages
                            ImagePreview {
                                required property int index
                                activeHolder: page
                                activeImageID: page.activeImageID
                                imageID: index
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Label {
                    text: "Image #" + page.activeImageID
                }

                Label {
                    text: "Blocks total: " + page.activeImageMetadata.numBlocks
                }
                Label {
                    text: "Blocks downloaded: " + page.numPackets
                }
                Label {
                    text: `Remaining: ${page.activeImageMetadata.numBlocks - page.numPackets}`
                }

                Layout.horizontalStretchFactor: 4
                Layout.fillHeight: true
                Layout.fillWidth: true

                CropVisualizer {
                    crop: page.activeImageMetadata.photoTransform()
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                }

                Label {
                    text: `H: ` + page.activeImageMetadata.left
                          + ` - ${page.activeImageMetadata.right}`
                }
                Label {
                    text: `V: ${page.activeImageMetadata.bottom} - ${page.activeImageMetadata.top}`
                }
                Label {
                    text: `E: ${page.activeImageMetadata.encodedWidth}x${page.activeImageMetadata.encodedHeight} Q: ${page.activeImageMetadata.encodedQuality}`
                }

                // check if librarian is asking
                RowLayout {
                    property bool looking: Librarian.activelyAskingForImage(
                                               page.activeImageID)
                    Label {
                        // should be 3 state: pause, start, finished
                        text: parent.looking ? "Downloading" : "Stopped"
                    }

                    Button {
                        // really this should be 3 state: pause, start, finished
                        text: parent.looking ? "Pause" : "Start"
                    }
                }
                Button {
                    text: "force reload"
                    onClicked: {
                        ImageDataHolder.rescanCount()
                        activeImage.reload()
                    }
                }
            }
        }
    }
}
