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
        }
        SpinBox {
            id: sPitchZero
            from: -127
            to: 127
            value: 0
        }
        SpinBox {
            id: ePitchZero
            from: -127
            to: 127
            value: 0
        }
        SpinBox {
            id: wPitchZero
            from: -127
            to: 127
            value: 0
        }
        Button {
            text: "Zero"
        }
    }
}
