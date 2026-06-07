import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

RowLayout {
    id: root
    ColumnLayout {
        id: servoColumn
        SpinBox {
            id: servoId
            from: 1
            to: 3
            value: 0
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

    Label {
        text: "Arm sequencer or more stats"
    }
}
