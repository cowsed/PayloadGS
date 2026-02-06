import QtQuick
import QtQuick.Controls
import QtCharts
import QtQuick.Layouts

RowLayout {

    // Material.theme: Material.Light
    id: telem
    Layout.topMargin: 5
    Layout.bottomMargin: 5

    property real ramUsed: 4120341234
    property real fsUsed: 11231221234

    property real ramTotal: 32967942144
    property real fsTotal: 291398508544

    property real batteryGoodLevel: 11
    property real batteryDangerLevel: 10.5

    property real batteryVoltage: 10.4
    property real batteryCurrent: 123

    property string batteryHealth: (batteryVoltage > batteryGoodLevel) ? "Good" : ((batteryVoltage > batteryDangerLevel) ? "Low" : "DANGER")

    property string batteryColor: (batteryVoltage > batteryGoodLevel) ? "green" : ((batteryVoltage > batteryDangerLevel) ? "orange" : "red")
    Layout.fillWidth: true
    Layout.fillHeight: true

    Gauge {
        Layout.minimumHeight: 100
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true
        from: 10.2
        to: 12.6
        value: telem.batteryVoltage

        frontColor: telem.batteryColor
        textColor: telem.batteryColor

        gaugeTitle: "Batt"
        topText: telem.batteryHealth
        mainText: telem.batteryVoltage + "V"
        subText: telem.batteryCurrent + "mA"
    }
    Gauge {
        id: ramGauge
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true
        from: 0
        to: telem.ramTotal / (1024 * 1024)
        value: telem.ramUsed / (1024 * 1024)

        topText: (telem.ramUsed / (1024 * 1024 * 1024)).toFixed(2) + "GB"
        subText: (telem.ramTotal / (1024 * 1024 * 1024)).toFixed(2) + " GB"

        mainText: (100 * (telem.ramUsed) / (telem.ramTotal)).toFixed(1) + "%"

        gaugeTitle: "RAM"
    }

    Gauge {
        id: fsGauge
        Layout.horizontalStretchFactor: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        from: 0
        to: telem.fsTotal / (1024 * 1024)
        value: telem.fsUsed / (1024 * 1024)

        topText: (telem.fsUsed / (1024 * 1024 * 1024)).toFixed(2) + "GB"

        subText: ((telem.fsTotal) / (1024 * 1024 * 1024)).toFixed(2) + "GB"

        mainText: (100 * (telem.fsUsed) / (telem.fsTotal)).toFixed(1) + "%"

        gaugeTitle: "FS"
    }
}
