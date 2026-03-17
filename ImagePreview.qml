import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

Rectangle {
    id: preview

    required property int imageID
    property int activeImageID
    required property Item activeHolder

    Layout.preferredWidth: 180
    Layout.preferredHeight: 135
    Layout.margins: 5

    function reload() {
        const oldSource = image.source
        image.source = ""
        image.source = oldSource
    }

    function updatePercent() {
        transmissionPercent.text = (100 * ImageDataHolder.transmissionPercent(
                                        preview.imageID)).toFixed(2) + "%"
    }

    color: (preview.activeImageID == preview.imageID) ? Qt.rgba(
                                                            0, 0, 0,
                                                            .1) : Material.background
    MouseArea {
        anchors.fill: parent

        onClicked: {
            preview.activeHolder.activeImageID = preview.imageID
        }
        RowLayout {
            anchors.fill: parent

            Image {
                id: image

                source: "file:" + ImageDataHolder.pathForImageThumbnail(
                            preview.imageID)
                Layout.preferredWidth: 128
                Layout.preferredHeight: 128
                fillMode: Image.PreserveAspectFit
            }

            ColumnLayout {

                // z: 2
                Text {
                    text: "ID: " + preview.imageID
                }
                Text {
                    id: transmissionPercent
                    text: (100 * ImageDataHolder.transmissionPercent(
                               preview.imageID)).toFixed(0) + "%"
                }
            }
        }
    }
}
