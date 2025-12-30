import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtCharts

Item {
    anchors.fill: parent
    Material.theme: Material.Light

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ComboBox {
                displayText: "SF" + currentValue
                model: [7, 8, 9, 10, 11, 12]
                onActivated: displayText = "SF" + currentValue
            }
            ComboBox {
                id: bwBox
                displayText: "BW" + currentValue
                model: [62.5, 125, 250, 500]
                onActivated: displayText = "BW" + currentValue
            }
            ComboBox {
                displayText: "CR" + currentValue
                model: ["4/5", "4/6", "4/7", "4/8"]
                onActivated: displayText = "CR" + currentValue
            }

            SpinBox {
                id: spinBox
                from: 0
                value: decimalToInt(420)
                to: decimalToInt(450)
                stepSize: decimalFactor
                editable: true

                property int decimals: 3
                property real realValue: value / decimalFactor
                readonly property int decimalFactor: Math.pow(10, decimals)

                function decimalToInt(decimal) {
                    return decimal * decimalFactor
                }

                validator: DoubleValidator {
                    bottom: Math.min(spinBox.from, spinBox.to)
                    top: Math.max(spinBox.from, spinBox.to)
                    decimals: spinBox.decimals
                    notation: DoubleValidator.StandardNotation
                }

                textFromValue: function (value, locale) {
                    return Number(value / decimalFactor).toLocaleString(
                                locale, 'f', spinBox.decimals)
                }

                valueFromText: function (text, locale) {
                    return Math.round(Number.fromLocaleString(
                                          locale, text) * decimalFactor)
                }
            }
            Label {
                text: "EDR: 12 bps"
            }

            RowLayout {
                Button {
                    text: "Set"
                }
                Button {
                    text: "Reset"
                }
            }
            Button {
                text: "Auto Neg"
            }

            Button {
                text: "Recover"
                enabled: false
            }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
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
                    id: rssiAxisY
                    min: 30 // Sets the minimum value for the Y-axis
                    max: 100 // Sets the maximum value for the Y-axis
                }
                ValueAxis {
                    id: snrAxisY
                    min: 30 // Sets the minimum value for the Y-axis
                    max: 100 // Sets the maximum value for the Y-axis
                }
                LineSeries {
                    name: "RSSI"
                    axisX: axisX
                    axisY: rssiAxisY
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
                    name: "SNR"
                    axisX: axisX
                    axisYRight: snrAxisY
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
        }
    }
}
