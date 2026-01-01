import QtQuick 2.15
import QtPositioning
import QtQuick.Controls

Control {
    id: compass

    padding: 10
    topInset: 10
    bottomInset: 10
    leftInset: 10
    rightInset: 10
    implicitHeight: width

    // anchors.fill: parent
    required property geoCoordinate payloadCoordinate
    required property geoCoordinate stationCoordinate
    required property geoCoordinate rocketCoordinate

    property real stationToPayloadAz: stationCoordinate.azimuthTo(
                                          payloadCoordinate)
    property real stationToRocketAz: stationCoordinate.azimuthTo(
                                         rocketCoordinate)

    Image {
        id: northsouth
        anchors.centerIn: compass

        height: compass.width
        fillMode: Image.PreserveAspectFit
        source: "qrc:/assets/images/compass_northsouth.png"
    }
    Image {
        id: payload
        anchors.centerIn: compass
        height: compass.width
        fillMode: Image.PreserveAspectFit
        rotation: compass.stationToPayloadAz
        source: "qrc:/assets/images/compass_payload.png"
    }
    Image {
        id: rocket
        anchors.centerIn: compass
        height: compass.width
        rotation: compass.stationToRocketAz
        fillMode: Image.PreserveAspectFit
        source: "qrc:/assets/images/compass_rocket.png"
    }

    Label {
        id: rangeLabel
        anchors.centerIn: compass
        font.pointSize: 10
        color: "black"
        text: "── Range ──"
    }

    Label {
        font.pointSize: 22
        color: "#f76902"
        text: compass.stationCoordinate.distanceTo(
                  compass.payloadCoordinate).toFixed(0) + "m"
        anchors.bottom: rangeLabel.top
        anchors.horizontalCenter: rangeLabel.horizontalCenter
    }
    Label {
        font.pointSize: 22
        color: "#7D55C7"
        text: compass.stationCoordinate.distanceTo(
                  compass.rocketCoordinate).toFixed(0) + "m"
        anchors.top: rangeLabel.bottom
        anchors.horizontalCenter: rangeLabel.horizontalCenter
    }
}
