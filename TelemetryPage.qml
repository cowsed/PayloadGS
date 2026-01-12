import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "Pages/Telemetry/"

Item {
    id: telem
    Material.theme: Material.Light

    required property real ramUsed
    required property real ramAvail
    required property real fsUsed
    required property real fsAvail

    required property real batteryVoltage
    required property real batteryCurrent

    ColumnLayout {
        anchors.fill: parent

        StorageGauges {
            ramUsed: telem.ramUsed
            ramAvail: telem.ramAvail
            fsUsed: telem.fsUsed
            fsAvail: telem.fsAvail

            batteryVoltage: telem.batteryVoltage
            batteryCurrent: telem.batteryCurrent

            Layout.verticalStretchFactor: 2
        }
        TempPlot {
            Layout.verticalStretchFactor: 2
        }
    }
}
