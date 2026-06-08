import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: control
    Material.theme: Material.Light

    required property variant window
    required property int flightNumber
    required property string stateString

    property bool forceBoostMenu: false
    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Rectangle {
                color: Material.primary
                Layout.margins: 4
                radius: 4
                border.width: 2
                border.color: Qt.rgba(.95, .95, .95)

                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: "State: "
                    padding: 4
                }
                Label {
                    anchors.centerIn: parent
                    text: control.stateString
                    color: "purple"

                    font.bold: true
                    font.pointSize: 40

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

            ColumnLayout {

                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: "Status: "
                    padding: 4
                }
                Label {
                    text: "GPS Has Fix: ?"
                }
                Label {
                    text: "GPS Has Fix: ?"
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            // Layout.verticalStretchFactor: 1
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Expect Launch"
                visible: !control.forceBoostMenu
                onClicked: RadioPacketParser.sendToPhase(RadioPacketParser.Expecting)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Boost"
                visible: !control.forceBoostMenu
                onClicked: control.forceBoostMenu = true
                Material.background: Material.Red
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Cancel Boost"
                visible: !control.forceBoostMenu
                onClicked: RadioPacketParser.sendToPhase(RadioPacketParser.Pad)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Manual"
                visible: !control.forceBoostMenu
                onClicked: RadioPacketParser.sendToPhase(RadioPacketParser.LandedManual)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Back"
                visible: control.forceBoostMenu
                onClicked: control.forceBoostMenu = false
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Really Force"
                Material.background: Material.Red
                visible: control.forceBoostMenu
                onClicked: RadioPacketParser.sendToPhase(RadioPacketParser.Flight)
            }
        }
    }
}
