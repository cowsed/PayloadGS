import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

RowLayout {
    id: root
    ColumnLayout {
        id: servoColumn
        Label {
            text: "Move Servo"
        }

        RowLayout {
            SpinBox {
                id: servoId
                from: 1
                to: 3
                value: 1
            }
            Label {
                text: "Servo ID"
            }
        }
        RowLayout {
            SpinBox {
                id: openTo
                from: 0
                to: 255
                value: 0
                editable: true
            }
            Label {
                text: "Open To"
            }
        }
        RowLayout {
            SpinBox {
                id: openDuration
                from: 0
                to: 2550
                value: 1
                editable: true
                stepSize: 10
            }
            Label {
                text: "Open Time (ms)"
            }
        }

        RowLayout {
            SpinBox {
                id: holdDuration
                from: 0
                to: 2550
                value: 1000
                editable: true
                stepSize: 10
            }
            Label {
                text: "Hold Time (ms)"
            }
        }

        RowLayout {
            SpinBox {
                id: closeTo
                from: 0
                to: 255
                value: 0
                editable: true
            }
            Label {
                text: "Close To"
            }
        }
        RowLayout {
            SpinBox {
                id: closeDuration
                from: 0
                to: 2550
                value: 1000
                editable: true
                stepSize: 10
            }
            Label {
                text: "Close Time (ms)"
            }
        }
        Button {
            text: "Move Servo"
            onClicked: RadioPacketParser.askToMoveServo(servoId.value - 1, openTo.value, openDuration.value / 10, holdDuration.value / 10, closeDuration.value / 10, closeTo.value)
        }
    }

    ToolSeparator {
        Layout.fillHeight: true
    }

    ColumnLayout {
        id: zeroArmColumn
        Label {
            text: "Zero Arm"
        }
        RowLayout {
            SpinBox {
                id: yawZero
                from: -127
                to: 127
                value: 0
                editable: true
            }
            Label {
                text: "S Yaw"
            }
        }
        RowLayout {
            SpinBox {
                id: sPitchZero
                from: -127
                to: 127
                value: 0
                editable: true
            }
            Label {
                text: "S Pitch"
            }
        }
        RowLayout {
            SpinBox {
                id: ePitchZero
                from: -127
                to: 127
                value: 0
                editable: true
            }
            Label {
                text: "E Pitch"
            }
        }

        RowLayout {
            SpinBox {
                id: wPitchZero
                from: -127
                to: 127
                value: 0
                editable: true
            }
            Label {
                text: "W Pitch"
            }
        }
        Button {
            text: "Zero"
            onClicked: RadioPacketParser.askToZeroArm(yawZero.value, sPitchZero.value, ePitchZero.value, wPitchZero.value)
        }
    }

    ToolSeparator {
        Layout.fillHeight: true
    }

    ColumnLayout {
        id: jogArmColumn
        Label {
            text: "Jog Arm"
        }
        SpinBox {
            id: jogMotorId
            from: 1
            to: 3
            value: 0
            editable: true
        }
        SpinBox {
            id: jogMillivolts
            from: -12000
            to: 12000
            stepSize: 100
            value: 0
            editable: true
        }
        SpinBox {
            id: jogDurationMs
            from: 0
            to: 2550
            value: 0
            stepSize: 10
            editable: true
        }
        Button {
            text: "Jog"
            onClicked: RadioPacketParser.askToJogMotor(jogMotorId.value - 1, jogMillivolts.value, jogDurationMs.value / 10)
        }
    }
}
