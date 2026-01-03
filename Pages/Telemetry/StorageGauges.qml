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
        id: batteryGauge
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
        Label {
            id: battDialLabel
            font.pointSize: 20
            color: telem.batteryColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: battCurrentLabel.bottom
            text: "Batt"
        }
    }
    Dial {
        id: ramDial
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        from: 0
        to: telem.ramUsed / (1024 * 1024) + telem.ramAvail / (1024 * 1024)
        value: telem.ramUsed / (1024 * 1024)

        enabled: false
        Label {
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: ramDialUsed.top
            text: (telem.ramUsed / (1024 * 1024 * 1024)).toFixed(2) + "GB"
        }
        Label {
            id: ramDialUsed
            font.pointSize: 28
            anchors.centerIn: parent
            text: (100 * (telem.ramUsed) / (telem.ramUsed + telem.ramAvail)).toFixed(
                      1) + "%"
        }
        Label {
            id: ramDialTotal
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: ramDialUsed.bottom
            text: ((telem.ramUsed + telem.ramAvail) / (1024 * 1024 * 1024)).toFixed(
                      2) + "GB"
        }
        Label {
            id: ramDialLabel
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: ramDialTotal.bottom
            text: "RAM"
        }
    }

    Dial {
        id: fsDial
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        from: 0
        to: telem.fsUsed / (1024 * 1024) + telem.fsAvail / (1024 * 1024)
        value: telem.fsUsed / (1024 * 1024)

        enabled: false
        Label {
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: fsDialUsed.top
            text: (telem.fsUsed / (1024 * 1024 * 1024)).toFixed(2) + "GB"
        }
        Label {
            id: fsDialUsed
            font.pointSize: 28
            anchors.centerIn: parent
            text: (100 * (telem.fsUsed) / (telem.fsUsed + telem.fsAvail)).toFixed(
                      1) + "%"
        }

        Label {
            id: fsDialTotal
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: fsDialUsed.bottom
            text: ((telem.fsUsed + telem.fsAvail) / (1024 * 1024 * 1024)).toFixed(
                      2) + "GB"
        }
        Label {
            id: fsDialLabel
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: fsDialTotal.bottom
            text: "FS"
        }
    }
}
