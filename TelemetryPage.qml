import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import QtLocation
import QtPositioning

Item {
    id: telemetry
    anchors.fill: parent
    property real payloadLatitude: 31.0443
    property real payloadLongitude: -103.53507

    property real stationLatitude: 31.043
    property real stationLongitude: -103.5350

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: "Telem"
        }

        MapViewer {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ColumnLayout {

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
}
