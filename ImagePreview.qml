import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

// preview of image in bottom slider to see and
Rectangle {
    id: preview

    required property int imageID
    property int activeImageID
    required property real transmissionPercent
    required property string dir
    required property Item activeHolder

    Layout.preferredWidth: 180
    Layout.preferredHeight: 135
    Layout.margins: 5

    color: (preview.activeImageID == preview.imageID) ? Qt.rgba(
                                                            0, 0, 0,
                                                            .1) : Material.background
    MouseArea {
        anchors.fill: parent

        onClicked: {
            console.log("Rectangle clicked!",
                        preview.imageID) // Optional console output
            preview.activeHolder.activeImageID = preview.imageID
        }
        RowLayout {
            anchors.fill: parent

            Image {
                source: preview.dir + "thumbnail.png"
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
                    text: preview.transmissionPercent + "%"
                }
            }
        }
    }
}
