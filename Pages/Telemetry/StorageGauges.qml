import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

RowLayout {

    // Material.theme: Material.Light
    id: telem
    Layout.topMargin: 5
    Layout.bottomMargin: 5

    required property real ramUsed
    required property real ramAvail
    required property real fsUsed
    required property real fsAvail

    required property real batteryVoltage
    required property real batteryCurrent

    property real batteryGoodLevel: 11
    property real batteryDangerLevel: 10.5

    property string batteryHealth: (batteryVoltage > batteryGoodLevel) ? "Good" : ((batteryVoltage > batteryDangerLevel) ? "Low" : "DANGER")

    property string batteryColor: (batteryVoltage > batteryGoodLevel) ? "green" : ((batteryVoltage > batteryDangerLevel) ? "orange" : "red")
    Layout.fillWidth: true
    Layout.fillHeight: true

    Dial {
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        value: telem.batteryVoltage
        enabled: false
        from: 10.2
        to: 12.6
        Label {
            color: telem.batteryColor
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: battVoltageLabel.top
            text: telem.batteryHealth
        }
        Label {
            color: telem.batteryColor
            id: battVoltageLabel
            font.pointSize: 28
            anchors.centerIn: parent
            text: telem.batteryVoltage + "V"
        }
        Label {
            id: battCurrentLabel
            font.pointSize: 20
            color: telem.batteryColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: battVoltageLabel.bottom
            text: telem.batteryCurrent + "mA"
        }
        TimeSinceThing {
            id: timeSincebattLabel
            event_time: new Date()
            suffix: " ago"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: battCurrentLabel.bottom
            font.pointSize: 16
        }
    }

    ChartView {
        Layout.horizontalStretchFactor: 1
        title: "RAM"
        Layout.fillWidth: true
        Layout.fillHeight: true
        id: ramUsage
        legend.alignment: Qt.AlignLeft
        antialiasing: true

        PieSeries {
            id: ramPieSeries
            PieSlice {
                label: "Used"
                value: telem.ramUsed
            }
            PieSlice {
                label: "Available"
                value: telem.ramAvail
            }
        }
    }
    ChartView {
        id: fsUsage
        title: "Storage"
        Layout.horizontalStretchFactor: 1

        Layout.fillWidth: true
        Layout.fillHeight: true
        legend.alignment: Qt.AlignLeft
        antialiasing: true

        PieSeries {
            id: fsPieSeries
            PieSlice {
                label: "Used"
                value: telem.fsUsed
            }
            PieSlice {
                label: "Available"
                value: telem.fsAvail
            }
        }
    }
}
