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
        target: page
        function onActiveImageIDChanged() {
            console.log("OAIIC")
            controlColumn.reconsiderControlColumn()
        }
    }

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
                id: controlColumn
                property bool hasAll: ImageDataHolder.imageComplete(
                                          page.activeImageID)

                function reconsiderControlColumn() {
                    dlButton.text = Librarian.activelyAskingForImage(
                                page.activeImageID) ? "Downloading" : "Paused"

                    controlColumn.hasAll = ImageDataHolder.imageComplete(
                                page.activeImageID)
                }

                Connections {
                    target: Librarian
                    function onNumRequestsChanged() {
                        console.log("Changed")
                        controlColumn.reconsiderControlColumn()
                    }
                }

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
                    oldCrop: page.activeImageMetadata.photoTransform()
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    oldImageId: page.activeImageID
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

                Button {
                    id: dlButton
                    visible: !parent.hasAll
                    checkable: false

                    text: Librarian.activelyAskingForImage(
                              page.activeImageID) ? "Downloading" : "Paused"

                    onPressed: {
                        if (Librarian.activelyAskingForImage(
                                page.activeImageID)) {
                            Librarian.StopImageDownload(page.activeImageID)
                        } else {

                            Librarian.StartImageDownload(page.activeImageID,
                                                         ImageDataHolder)
                        }
                    }
                }

                Label {
                    visible: parent.hasAll
                    text: "Completed"
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
