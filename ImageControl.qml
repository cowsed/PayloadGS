pragma ComponentBehavior: Bound

import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: page

    property int activeImageID: 0

    Connections {
        target: page
        function onActiveImageIDChanged() {
            console.log("OAIIC");
            const activeImageMetadata = ImageDataHolder.metadataForImageId(page.activeImageID);
            controlColumn.reconsiderControlColumn(activeImageMetadata);
            imageId.text = "Image #" + page.activeImageID;
            activeImage.source = "file:" + ImageDataHolder.pathForImage(page.activeImageID);
        }
    }

    Connections {
        target: ImageDataHolder
        function onImageUpdated(image_id) {
            if (image_id == page.activeImageID) {
                activeImage.reload(ImageDataHolder.metadataForImageId(page.activeImageID));
            }
            if (previews.itemAt(image_id)) {
                previews.itemAt(image_id).reload();
            }
            cropvis.reload();
        }
    }

    Material.theme: Material.Light
    RowLayout {
        anchors.fill: parent
        Image {
            id: activeImage
            Layout.fillWidth: true
            Layout.horizontalStretchFactor: 3

            cache: false
            Layout.preferredWidth: 512 - 60
            Layout.preferredHeight: 512 - 60
            fillMode: Image.PreserveAspectFit

            source: "file:" + ImageDataHolder.pathForImage(page.activeImageID)

            function reload(metadata) {
                const oldSource = activeImage.source;
                activeImage.source = "";
                activeImage.source = oldSource;
                activeImage.mirrorVertically = metadata.j4 < 0;
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.horizontalStretchFactor: 1
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
                property bool hasAll: ImageDataHolder.imageComplete(page.activeImageID)

                function reconsiderControlColumn(metadata) {
                    dlButton.text = Librarian.activelyAskingForImage(page.activeImageID) ? "Downloading" : "Paused";

                    const numDownloaded = ImageDataHolder.numDownloadedPackets(page.activeImageID);

                    controlColumn.hasAll = numDownloaded == metadata.numBlocks;
                    numBlocksTotal.text = "Blocks total: " + metadata.numBlocks;
                    numBlocksDownloaded.text = "Blocks downloaded: " + numDownloaded;
                    numBlocksRemaining.text = "Blocks Remaining: " + (metadata.numBlocks - numDownloaded);
                    cropvis.oldCrop = metadata.photoTransform();
                    originalSizeLabel.text = `O Size: ${metadata.right - metadata.left}x${metadata.bottom - metadata.top}`;
                    encodedSizeLabel.text = `E Size: ${metadata.encodedWidth}x${metadata.encodedHeight}`;
                    qualityLabel.text = "Quality: " + metadata.encodedQuality;
                }

                Connections {
                    target: Librarian
                    function onNumRequestsChanged() {
                        controlColumn.reconsiderControlColumn(ImageDataHolder.metadataForImageId(page.activeImageID));
                    }
                }

                Label {
                    id: imageId
                    text: "Image #"
                }

                Label {
                    id: numBlocksTotal
                    text: "Blocks total: "
                }
                Label {
                    id: numBlocksDownloaded
                    text: "Blocks downloaded: "
                }
                Label {
                    id: numBlocksRemaining
                    text: `Remaining: `
                }

                Layout.horizontalStretchFactor: 4
                Layout.fillHeight: true
                Layout.fillWidth: true

                CropVisualizer {
                    id: cropvis
                    oldCrop: undefined
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    oldImageId: page.activeImageID
                }

                Label {
                    id: originalSizeLabel
                    text: `O Size: `
                }
                Label {
                    id: encodedSizeLabel
                    text: `E Size: `
                }
                Label {
                    id: qualityLabel
                    text: `Quality: `
                }

                RowLayout {
                    Button {
                        id: dlButton
                        visible: !controlColumn.hasAll
                        checkable: false

                        text: Librarian.activelyAskingForImage(page.activeImageID) ? "Downloading" : "Paused"

                        onPressed: {
                            if (Librarian.activelyAskingForImage(page.activeImageID)) {
                                Librarian.StopImageDownload(page.activeImageID);
                            } else {
                                Librarian.StartImageDownload(page.activeImageID, ImageDataHolder);
                            }
                        }
                    }

                    Label {
                        visible: controlColumn.hasAll
                        text: "Completed"
                    }
                    Button {
                        text: "Ask Info"
                        onClicked: RadioPacketParser.askForMetadata(page.activeImageIDChanged)
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.maximumHeight: 1
                    Layout.minimumHeight: 1
                    color: "#D3D3D3" // Light gray color line
                }

                Button {
                    text: "Request"
                    // on click, pop off the stack and send it to radio
                    onClicked: Librarian.SubmitRequestToRadio(RadioPacketParser)
                }
            }
        }
    }
}
