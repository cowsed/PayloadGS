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
    animationOptions: ChartView.SeriesAnimations

    property date startDate: new Date()

    Timer {
        interval: 460
        running: false // set to true for fun
        repeat: true
        onTriggered: {
            let t = (new Date().getTime(
                         ) - tempChart.startDate.getTime()) / 1000

            TelemetryLogHolder.batteryVoltage.newValue(new Date(),
                                                       Math.max(12.5 - t / 10,
                                                                10.2))

            TelemetryLogHolder.batteryCurrent.newValue(
                        new Date(), 120 + 200 + 10 * Math.sin(t))

            const starts = [TelemetryLogHolder.cpuTemp.earliestTime, TelemetryLogHolder.radioTemp.earliestTime, TelemetryLogHolder.batteryVoltage.earliestTime]

            const ends = [TelemetryLogHolder.cpuTemp.latestTime, TelemetryLogHolder.radioTemp.latestTime, TelemetryLogHolder.batteryVoltage.latestTime]

            axisX.min = new Date(Math.min(...starts))
            axisX.max = new Date(Math.max(...ends))
        }
    }

    Timer {
        interval: 1000
        running: false // set to true for fun
        repeat: true
        onTriggered: {
            let t = (new Date().getTime(
                         ) - tempChart.startDate.getTime()) / 1000

            TelemetryLogHolder.cpuTemp.newValue(new Date(),
                                                30 + 5 * Math.sin(t))
            TelemetryLogHolder.radioTemp.newValue(new Date(),
                                                  30 + 5 * Math.cos(t))

            const starts = [TelemetryLogHolder.cpuTemp.earliestTime, TelemetryLogHolder.radioTemp.earliestTime, TelemetryLogHolder.batteryVoltage.earliestTime]
            const ends = [TelemetryLogHolder.cpuTemp.latestTime, TelemetryLogHolder.radioTemp.latestTime, TelemetryLogHolder.batteryVoltage.latestTime]

            axisX.min = new Date(Math.min(...starts))
            axisX.max = new Date(Math.max(...ends))
        }
    }

    Connections {
        target: TelemetryLogHolder.cpuTemp

        function onValueChanged() {
            TelemetryLogHolder.cpuTemp.fillXYSeries(tempChart.series(
                                                        cpuTempSeries.name))
        }
    }
    Connections {
        target: TelemetryLogHolder.radioTemp

        function onValueChanged() {
            TelemetryLogHolder.radioTemp.fillXYSeries(tempChart.series(
                                                          radioTempSeries.name))
        }
    }
    Connections {
        target: TelemetryLogHolder.batteryVoltage

        function onValueChanged() {
            TelemetryLogHolder.batteryVoltage.fillXYSeries(
                        tempChart.series(batteryVoltageSeries.name))
        }
    }
    Component.onCompleted: {
        TelemetryLogHolder.cpuTemp.fillXYSeries(tempChart.series(
                                                    cpuTempSeries.name))
        TelemetryLogHolder.radioTemp.fillXYSeries(tempChart.series(
                                                      radioTempSeries.name))
        TelemetryLogHolder.batteryVoltage.fillXYSeries(
                    tempChart.series(batteryVoltageSeries.name))

        const starts = [TelemetryLogHolder.cpuTemp.earliestTime, TelemetryLogHolder.radioTemp.earliestTime, TelemetryLogHolder.batteryVoltage.earliestTime]
        const ends = [TelemetryLogHolder.cpuTemp.latestTime, TelemetryLogHolder.radioTemp.latestTime, TelemetryLogHolder.batteryVoltage.latestTime]

        axisX.min = new Date(Math.min(...starts))
        axisX.max = new Date(Math.max(...ends))
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
