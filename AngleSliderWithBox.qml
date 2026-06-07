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
    function update(newVal) {
        root.value = newVal;
        slider.value = newVal;
        currentBox.value = newVal;
    }

    Label {
        Layout.alignment: Qt.AlignHCenter

        text: root.label
    }

    Slider {
        id: slider
        from: root.from
        to: root.to
        value: (root.to + root.from) / 2
        orientation: Qt.Vertical
        onValueChanged: root.update(value)
    }
    SpinBox {
        id: currentBox
        implicitWidth: slider.width
        from: root.from
        to: root.to
        value: (root.to + root.from) / 2
        onValueChanged: root.update(value)
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
        id: actualBox
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
