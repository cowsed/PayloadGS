import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

RowLayout {
    id: root
    property int telem_id: 0
    required property string label
    property int from: 0
    property int to: 30 * 60
    property int value: 0
    SpinBox {
        from: root.from
        to: root.to
        value: root.value
    }

    Label {
        text: root.label
    }
    Button {
        text: "Now"
        onClicked: RadioPacketParser.askForTelemetryInt(root.telem_id)
    }
}
