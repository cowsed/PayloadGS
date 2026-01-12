import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: control
    Material.theme: Material.Light

    required property int flightNumber
    required property date lastFlightNumberUpdate
    required property string stateString

    property bool forceBoostMenu: false
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            // Layout.verticalStretchFactor: 1
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Expect Launch"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Boost"
                visible: !control.forceBoostMenu
                onClicked: control.forceBoostMenu = true
                Material.background: Material.Red
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Cancel Boost"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Manual"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Back"
                visible: control.forceBoostMenu
                onClicked: control.forceBoostMenu = false
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Really Force"
                Material.background: Material.Red
                visible: control.forceBoostMenu
            }
        }
        RowLayout {
            Label {
                text: "State: "
            }
            Label {
                color: "purple"
                font.bold: true
                text: control.stateString
            }
        }

        RowLayout {
            Button {
                text: "Next Flight"
            }
            Label {
                text: "Flight #" + control.flightNumber
            }
            Button {
                text: "⟳"
            }
            TimeSinceThing {
                desc: "Last Updated: "
                event_time: control.lastFlightNumberUpdate
                suffix: " ago"
            }
        }
    }
}
