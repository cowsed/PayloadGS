import QtQuick
import QtCharts
import QtQuick.Layouts

ChartView {
    id: tempChart
    legend.alignment: Qt.AlignTop
    antialiasing: true

    Layout.topMargin: 0

    margins.top: 0
    margins.bottom: 0

    Layout.fillWidth: true
    Layout.fillHeight: true

    theme: ChartView.ChartThemeQt

    property date startDate: new Date()

    Timer {
        interval: 1000
        running: true // set to true for fun
        repeat: true
        onTriggered: {
            let t = (new Date().getTime(
                         ) - tempChart.startDate.getTime()) / 1000
            TelemetryLogHolder.newCpuTemp(new Date(), 30 + 5 * Math.sin(t))
            TelemetryLogHolder.newRadioTemp(new Date(), 30 + 5 * Math.cos(t))

            TelemetryLogHolder.newBatteryVoltage(new Date(),
                                                 Math.max(12.5 - t / 10, 10.2))

            const starts = [TelemetryLogHolder.earliestCpuTempTime(
                                ), TelemetryLogHolder.earliestRadioTempTime(
                                ), TelemetryLogHolder.earliestBatteryVoltageTime(
                                )]

            const ends = [TelemetryLogHolder.cpuTempUpdateTime, TelemetryLogHolder.radioTempUpdateTime, TelemetryLogHolder.batteryVoltageUpdateTime]

            axisX.min = new Date(Math.min(...starts))
            axisX.max = new Date(Math.max(...ends))
        }
    }

    Connections {
        target: TelemetryLogHolder

        function onCpuTempChanged() {
            TelemetryLogHolder.updateCpuTempSeries(tempChart.series(
                                                       cpuTempSeries.name))
        }
        function onRadioTempChanged() {
            TelemetryLogHolder.updateRadioTempSeries(tempChart.series(
                                                         radioTempSeries.name))
        }
        function onBatteryVoltageChanged() {
            TelemetryLogHolder.updateBatteryVoltageSeries(
                        tempChart.series(batteryVoltageSeries.name))
        }
    }

    DateTimeAxis {
        id: axisX
        tickCount: 6
        format: "hh:mm:ss"
    }

    ValueAxis {
        id: axisYTemp
        min: 25
        max: 100
        titleText: "Temperature"
    }

    ValueAxis {
        id: axisYVoltage
        min: 7
        max: 12.6
        titleText: "Voltage"
    }

    LineSeries {
        id: batteryVoltageSeries
        name: "Bat Voltage V"
        axisX: axisX
        axisYRight: axisYVoltage
        color: "red"
        width: 2
        pointsVisible: true
    }
    LineSeries {
        id: cpuTempSeries
        name: "PI CPU °C"
        axisX: axisX
        axisY: axisYTemp
        color: "blue"
        width: 2
        pointsVisible: true
    }
    LineSeries {
        id: radioTempSeries
        name: "Radio °C"
        axisX: axisX
        axisY: axisYTemp
        color: "green"
        width: 2
        pointsVisible: true
    }
}
