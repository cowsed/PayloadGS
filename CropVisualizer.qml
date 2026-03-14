import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    required property ImageMetadataHolder crop

    property color bg_color: "#a0a0a0"
    property color fg_color: Material.accent
    property real original_width: 6000

    width: 400
    height: 400
    readonly property real aspectRatio: 3 / 2

    Rectangle {
        anchors.centerIn: parent

        // Dynamically calculate width and height to maintain aspect ratio within parent
        // It checks which dimension is more restrictive to fit within the parent's size
        width: parent.width < parent.height
               * parent.aspectRatio ? parent.width : parent.height * parent.aspectRatio
        height: parent.height < parent.width
                / parent.aspectRatio ? parent.height : parent.width / parent.aspectRatio

        property real scaleFac: width / root.original_width

        color: root.bg_color

        Rectangle {
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            anchors.bottomMargin: root.crop.bottom * parent.scaleFac
            anchors.leftMargin: root.crop.left * parent.scaleFac

            width: (root.crop.right - root.crop.left) * parent.scaleFac
            height: (root.crop.top - root.crop.bottom) * parent.scaleFac

            color: root.fg_color
        }
    }
}
