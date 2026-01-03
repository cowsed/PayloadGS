import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtPositioning

Item {
    Material.theme: Material.Light

    id: telemetry
    required property geoCoordinate payloadCoordinate
    required property date payloadCoordinateUpdateTime

    required property geoCoordinate stationCoordinate
    required property date stationCoordinateUpdateTime

    required property geoCoordinate rocketCoordinate
    required property date rocketCoordinateUpdateTime

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            Layout.horizontalStretchFactor: 5
            Layout.fillHeight: true

            // Layout.fillWidth: true
            MapViewer {
                Layout.fillHeight: true
                Layout.fillWidth: true

                payloadCoordinate: telemetry.payloadCoordinate
                stationCoordinate: telemetry.stationCoordinate
                rocketCoordinate: telemetry.rocketCoordinate
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                RowLayout {

                    TextEdit {
                        id: copierHack
                        Layout.maximumWidth: 0
                        readOnly: true
                        visible: false
                    }

                    RoundButton {
                        id: payloadPosition
                        text: "P: " + telemetry.payloadCoordinate.latitude.toFixed(
                                  5) + ", " + telemetry.payloadCoordinate.longitude.toFixed(
                                  5)
                        radius: 1
                        // readOnly: true
                        onClicked: {
                            copierHack.text = telemetry.payloadCoordinate.latitude.toFixed(
                                        7) + ", " + telemetry.payloadCoordinate.longitude.toFixed(
                                        7)
                            copierHack.selectAll()
                            copierHack.copy()
                        }
                    }
                    RoundButton {
                        id: stationPosition
                        text: "G: " + telemetry.stationCoordinate.latitude.toFixed(
                                  5) + ", " + telemetry.stationCoordinate.longitude.toFixed(
                                  5)
                        radius: 1
                        onClicked: {
                            copierHack.text = "" + telemetry.stationCoordinate.latitude.toFixed(
                                        7) + ", " + telemetry.stationCoordinate.longitude.toFixed(
                                        7)
                            copierHack.selectAll()
                            copierHack.copy()
                        }
                    }
                    RoundButton {
                        id: rocketPosition
                        text: "R: " + telemetry.rocketCoordinate.latitude.toFixed(
                                  5) + ", " + telemetry.rocketCoordinate.longitude.toFixed(
                                  5)
                        radius: 1
                        onClicked: {
                            copierHack.text = "" + telemetry.rocketCoordinate.latitude.toFixed(
                                        7) + ", " + telemetry.rocketCoordinate.longitude.toFixed(
                                        7)
                            copierHack.selectAll()
                            copierHack.copy()
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.horizontalStretchFactor: 14
            // Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            CompassUI {
                Layout.fillWidth: true
                payloadCoordinate: telemetry.payloadCoordinate
                stationCoordinate: telemetry.stationCoordinate
                rocketCoordinate: telemetry.rocketCoordinate
            }
            TimeSinceThing {
                desc: "Last Updated: "
                event_time: new Date()
            }
        }
    }
}
