import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtCharts
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

    property bool forceBoostMenu: false
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.verticalStretchFactor: 1
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Expect Launch"
                visible: !telem.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Boost"
                visible: !telem.forceBoostMenu
                onClicked: telem.forceBoostMenu = true
                Material.background: Material.Red
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Cancel Boost"
                visible: !telem.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Manual"
                visible: !telem.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Back"
                visible: telem.forceBoostMenu
                onClicked: telem.forceBoostMenu = false
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Really Force"
                Material.background: Material.Red
                visible: telem.forceBoostMenu
            }
        }

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
