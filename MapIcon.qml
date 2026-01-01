import QtQuick
import QtPositioning
import QtLocation

MapQuickItem {
    id: icon

    required property geoCoordinate coordinates
    required property string path

    property real iconSize: 50

    anchorPoint.x: iconImage.width / 2
    anchorPoint.y: iconImage.height / 2
    coordinate: icon.coordinates
    visible: true

    sourceItem: Image {
        id: iconImage
        width: icon.iconSize
        height: icon.iconSize
        fillMode: Image.PreserveAspectFit

        source: icon.path
    }
}
