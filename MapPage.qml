import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtPositioning

Item {
    Material.theme: Material.Light

    id: telemetry

    property geoCoordinate payloadCoordinate: TelemetryLogHolder.latestPayloadPosition
    property date payloadCoordinateUpdateTime: TelemetryLogHolder.latestPayloadPositionUpdateTime

    property geoCoordinate stationCoordinate: TelemetryLogHolder.latestStationPosition
    property date stationCoordinateUpdateTime: TelemetryLogHolder.latestStationPositionUpdateTime

    property geoCoordinate rocketCoordinate: TelemetryLogHolder.latestRocketPosition
    property date rocketCoordinateUpdateTime: TelemetryLogHolder.latestRocketPositionUpdateTime

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            Layout.horizontalStretchFactor: 20
            Layout.fillHeight: true

            Layout.fillWidth: true
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
                        font.pointSize: 11
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
                        font.pointSize: 11
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
                        font.pointSize: 11
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
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            PositionSource {
                id: src
                name: "nmea"
                PluginParameter {
                    name: "nmea.source"
                    value: "serial:/dev/ttyAMA0"
                }
                PluginParameter {
                    name: "nmea.baudrate"
                    value: 9600
                }
                onPositionChanged: {
                    var coord = src.position.coordinate
                    console.log("Coordinate:", coord.longitude, coord.latitude)
                }
            }

            CompassUI {
                Layout.fillWidth: true
                payloadCoordinate: telemetry.payloadCoordinate
                stationCoordinate: telemetry.stationCoordinate
                rocketCoordinate: telemetry.rocketCoordinate
            }

            RowLayout {
                Layout.fillWidth: true

                TimeSinceThing {
                    desc: "Payload: "
                    event_time: telemetry.payloadCoordinateUpdateTime
                }
                Rectangle {
                    Layout.fillWidth: true
                }
                Label {
                    text: telemetry.payloadCoordinate.altitude + " m ASL"
                }
            }
            RowLayout {
                Layout.fillWidth: true
                TimeSinceThing {
                    desc: "Rocket: "
                    event_time: telemetry.rocketCoordinateUpdateTime
                }
                Rectangle {
                    Layout.fillWidth: true
                }

                Label {
                    text: telemetry.rocketCoordinate.altitude + "m ASL"
                }
            }
            RowLayout {
                Layout.fillWidth: true

                TimeSinceThing {
                    desc: "Station: "
                    event_time: telemetry.stationCoordinateUpdateTime
                }
                Rectangle {
                    Layout.fillWidth: true
                }

                Label {
                    text: telemetry.stationCoordinate.altitude + "m ASL"
                }
            }
        }
    }
}
