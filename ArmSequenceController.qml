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
                value: 0
            }
            Label {
                text: "Servo ID"
            }
        }
        RowLayout {
            SpinBox {
                id: openTo
                from: 1
                to: 255
                value: 0
            }
            Label {
                text: "Open To"
            }
        }
        RowLayout {
            SpinBox {
                id: openDuration
                from: 1
                to: 2550
                value: 0
            }
            Label {
                text: "Open Time (ms)"
            }
        }

        RowLayout {
            SpinBox {
                id: closeTo
                from: 1
                to: 255
                value: 0
            }
            Label {
                text: "Close To"
            }
        }
        RowLayout {
            SpinBox {
                id: closeDuration
                from: 1
                to: 2550
                value: 0
            }
            Label {
                text: "Close Time (ms)"
            }
        }

        CheckBox {
            id: comeBack
            text: "Come Back"
        }
        CheckBox {
            id: stayPowered
            text: "Stay On"
        }
    }
    ColumnLayout {
        id: zeroArmColumn
        Label {
            text: "Zero Arm"
        }
        SpinBox {
            id: yawZero
            from: -127
            to: 127
            value: 0
            editable: true
        }
        SpinBox {
            id: sPitchZero
            from: -127
            to: 127
            value: 0
            editable: true
        }
        SpinBox {
            id: ePitchZero
            from: -127
            to: 127
            value: 0
            editable: true
        }
        SpinBox {
            id: wPitchZero
            from: -127
            to: 127
            value: 0
            editable: true
        }
        Button {
            text: "Zero"
            onClicked: RadioPacketParser.askToZeroArm(yawZero.value, sPitchZero.value, ePitchZero.value, wPitchZero.value)
        }
    }

    ColumnLayout {
        id: jogArmColumn
        Label {
            text: "Jog Arm"
        }
        SpinBox {
            id: jogMotorId
            from: 0
            to: 2
            value: 0
            editable: true
        }
        SpinBox {
            id: jogMillivolts
            from: -12000
            to: 12000
            value: 0
            editable: true
        }
        SpinBox {
            id: jogDurationMs
            from: 0
            to: 5000
            value: 0
            editable: true
        }
        Button {
            text: "Jog"
            onClicked: RadioPacketParser.askToJogArm(jogMotorId.valiue, jogMillivolts.value, jogDurationMs.value / 10)
        }
    }
}
