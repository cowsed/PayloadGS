import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Material.theme: Material.Light

    id: telemetry
    anchors.fill: parent
    property real payloadLatitude: 31.0443
    property real payloadLongitude: -103.53507

    property real stationLatitude: 31.043
    property real stationLongitude: -103.5350

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            Layout.horizontalStretchFactor: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            MapViewer {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                RowLayout {
                    Label {
                        text: "Latitude"
                    }
                    TextArea {
                        text: telemetry.payloadLatitude
                        readOnly: true
                    }
                    Label {
                        text: "Longitude"
                    }
                    TextArea {
                        text: telemetry.payloadLongitude
                        readOnly: true
                    }
                    Button {
                        text: "Copy"
                        onClicked: function () {
                            print("copy paste")
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.horizontalStretchFactor: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                Layout.fillHeight: true
                Layout.fillWidth: true

                text: "70cm Radio"
            }
        }
    }
}
