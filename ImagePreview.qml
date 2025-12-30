import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// preview of image in bottom slider to see and
Item {
    anchors.fill: parent
    id: preview
    // Layout.fillWidth: true
    // Layout.fillHeight: true
    required property string imageName
    required property real transmissionPercent
    required property string dir

    ColumnLayout {
        Item {
            Image {
                source: preview.dir + "sofar.png"
                Layout.preferredWidth: 512
                Layout.preferredHeight: 512
                fillMode: Image.PreserveAspectFit
            }
        }

        Text {
            text: "Image 0"
        }
    }
}
