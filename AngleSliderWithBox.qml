import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

ColumnLayout {
    id: root

    required property real actual

    required property real from
    required property real to
    required property string label

    property real value: (from + to) / 2

    Label {
        Layout.alignment: Qt.AlignHCenter

        text: root.label
    }

    Slider {
        id: slider
        from: root.from
        to: root.to
        value: root.value
        orientation: Qt.Vertical
        onValueChanged: root.value = value
    }
    SpinBox {
        implicitWidth: slider.width
        from: root.from
        to: root.to
        value: root.value
        onValueChanged: root.value = value
        editable: true
        Layout.preferredHeight: 40
        up.indicator: Item {
            implicitWidth: 0
        }

        down.indicator: Item {
            implicitWidth: 0
        }
    }

    SpinBox {
        implicitWidth: slider.width
        from: root.from
        to: root.to
        value: root.actual
        editable: false
        Layout.preferredHeight: 40
        enabled: false
        hoverEnabled: false

        up.indicator: Item {
            implicitWidth: 0
        }

        down.indicator: Item {
            implicitWidth: 0
        }
    }
}
