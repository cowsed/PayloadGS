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

    ValueAxis {
        id: axisX
        min: 0 // Sets the minimum value for the X-axis
        max: 6 // Sets the maximum value for the X-axis
    }

    ValueAxis {
        id: axisY
        min: 30 // Sets the minimum value for the Y-axis
        max: 100 // Sets the maximum value for the Y-axis
    }
    LineSeries {
        name: "PI CPU °C"
        axisX: axisX
        axisY: axisY
        XYPoint {
            x: 0
            y: 35
        }
        XYPoint {
            x: 1.1
            y: 35.1
        }
        XYPoint {
            x: 1.9
            y: 37.3
        }
        XYPoint {
            x: 2.1
            y: 42.1
        }
        XYPoint {
            x: 2.9
            y: 44.9
        }
        XYPoint {
            x: 3.4
            y: 43.0
        }
        XYPoint {
            x: 4.1
            y: 44.3
        }
    }
    LineSeries {
        name: "Radio °C"
        axisX: axisX
        axisY: axisY
        XYPoint {
            x: 0
            y: 40
        }
        XYPoint {
            x: 1.1
            y: 41
        }
        XYPoint {
            x: 1.9
            y: 43
        }
        XYPoint {
            x: 2.1
            y: 48
        }
        XYPoint {
            x: 2.9
            y: 49
        }
        XYPoint {
            x: 3.4
            y: 50
        }
        XYPoint {
            x: 4.1
            y: 53
        }
    }
}
