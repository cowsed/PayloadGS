import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: control
    Material.theme: Material.Light

    required property variant window
    required property string stateString
    required property payloadFlags pflags

    property variant goingTo: RadioPacketParser.Starting
    function startPhaseShift(toPhase) {
        goingTo = toPhase;

        shiftConfirmation.visible = true;
    }

    ColumnLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
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
        }
        RowLayout {
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Runcam On"
                onClicked: RadioPacketParser.askForRuncamOn(true)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Runcam Off"
                onClicked: RadioPacketParser.askForRuncamOn(false)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Restart"
                onClicked: RadioPacketParser.askForRestart()
            }
        }

        ColumnLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true
            Label {
                text: "Status: "
                padding: 4
            }
            GridLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                // spacing: 10
                rowSpacing: 20
                columnSpacing: 10
                columns: 3
                Label {
                    Layout.fillWidth: true
                    text: "GPS Has Fix: " + control.pflags.GPSHasFix
                }
                Label {
                    Layout.fillWidth: true
                    text: "LastArmMoveStalled: " + control.pflags.LastArmMovedStalled
                }
                Label {
                    Layout.fillWidth: true
                    text: "LastServoMoveStalled: " + control.pflags.LastServoMoveStalled
                }
                Label {
                    Layout.fillWidth: true
                    text: "Runcam: " + control.pflags.RuncamOn
                }
                Label {
                    Layout.fillWidth: true
                    text: "ArmMoving: " + control.pflags.ArmMoving
                }
                Label {
                    Layout.fillWidth: true
                    text: "ServoMoving: " + control.pflags.ServoMoving
                }
                Label {
                    Layout.fillWidth: true
                    text: "StmOn: " + control.pflags.StmBooted
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Start Flipping"
                onClicked: control.startPhaseShift(RadioPacketParser.LandedFlipping)
                Material.background: Material.Red
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Unfold"
                onClicked: control.startPhaseShift(RadioPacketParser.LandedUnfolding)
                Material.background: Material.Red
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Panorama"
                onClicked: control.startPhaseShift(RadioPacketParser.LandedAutomaticCamera)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Manual"
                onClicked: control.startPhaseShift(RadioPacketParser.LandedManual)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Expect Launch"
                onClicked: control.startPhaseShift(RadioPacketParser.Expecting)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Boost"
                onClicked: control.startPhaseShift(RadioPacketParser.Flight)
                Material.background: Material.Red
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Pad"
                onClicked: control.startPhaseShift(RadioPacketParser.Pad)
            }
            Button {
                font.pointSize: 20
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Manual"
                onClicked: control.startPhaseShift(RadioPacketParser.LandedManual)
            }
        }
    }

    MessageDialog {
        id: shiftConfirmation
        title: "Send to Phase"
        text: " Proceed to send to phase? think carefully!.Going to \n  " + RadioPacketParser.phaseToShortString(control.goingTo)
        visible: false
        buttons: Dialog.Yes | Dialog.No

        onAccepted: RadioPacketParser.sendToPhase(control.goingTo)
        onRejected: console.log("aborted")
    }
}
