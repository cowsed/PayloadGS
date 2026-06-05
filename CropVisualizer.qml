import QtQuick
import QtQuick.Controls.Material

Item {
    id: root
    required property int oldImageId
    required property photoTransform oldCrop
    property photoTransform crop

    property color bg_color: "#a0a0a0"
    property color old_color: "#000000"
    property color fg_color: Material.accent
    property real original_width: 1280
    property real original_height: 800

    function reload() {
        const oldPath = backgroundImage.source;
        backgroundImage.source = "";
        backgroundImage.source = oldPath;
    }

    width: 400
    height: 400
    readonly property real aspectRatio: original_width / original_height

    Rectangle {
        anchors.centerIn: parent

        // Dynamically calculate width and height to maintain aspect ratio within parent
        // It checks which dimension is more restrictive to fit within the parent's size
        width: parent.width < parent.height * parent.aspectRatio ? parent.width : parent.height * parent.aspectRatio
        height: parent.height < parent.width / parent.aspectRatio ? parent.height : parent.width / parent.aspectRatio

        property real scaleFac: width / root.original_width

        color: root.bg_color

        Rectangle {
            visible: root.oldCrop != null
            anchors.left: parent.left
            anchors.top: parent.top

            anchors.topMargin: root.oldCrop.top * parent.scaleFac
            anchors.leftMargin: root.oldCrop.left * parent.scaleFac

            width: (root.oldCrop.right - root.oldCrop.left) * parent.scaleFac
            height: (root.oldCrop.bottom - root.oldCrop.top) * parent.scaleFac

            color: root.old_color
            Image {
                id: backgroundImage
                visible: root.oldImageId != null
                source: "file:" + ImageDataHolder.pathForImage(root.oldImageId)
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top

            anchors.topMargin: root.crop.top * parent.scaleFac
            anchors.leftMargin: root.crop.left * parent.scaleFac

            width: (root.crop.right - root.crop.left) * parent.scaleFac
            height: (root.crop.bottom - root.crop.top) * parent.scaleFac

            color: root.fg_color
            opacity: .5
        }
    }
}
