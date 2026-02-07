import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtCharts

// import QtSensors
Item {
    Material.theme: Material.Light

    id: page

    property real freqMin: 410000000
    property real freqMax: 440000000

    required property LoraSettings currentSettings

    RowLayout {
        anchors.fill: parent

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

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ComboBox {
                id: sfBox
                currentIndex: 0
                textRole: "text"
                valueRole: "value"

                model: ListModel {
                    ListElement {
                        text: "SF6"
                        value: LoraSettings.SpreadingFactor.SF6
                    }
                    ListElement {
                        text: "SF7"
                        value: LoraSettings.SpreadingFactor.SF7
                    }
                    ListElement {
                        text: "SF8"
                        value: LoraSettings.SpreadingFactor.SF8
                    }
                    ListElement {
                        text: "SF9"
                        value: LoraSettings.SpreadingFactor.SF9
                    }
                    ListElement {
                        text: "SF10"
                        value: LoraSettings.SpreadingFactor.SF10
                    }
                    ListElement {
                        text: "SF11"
                        value: LoraSettings.SpreadingFactor.SF11
                    }
                    ListElement {
                        text: "SF12"
                        value: LoraSettings.SpreadingFactor.SF12
                    }
                }
            }
            ComboBox {
                id: bwBox
                currentIndex: 0
                textRole: "text"
                valueRole: "value"

                model: ListModel {
                    ListElement {
                        text: "BW8"
                        value: LoraSettings.Bandwidth.BW8
                    }
                    ListElement {
                        text: "BW10"
                        value: LoraSettings.Bandwidth.BW10
                    }
                    ListElement {
                        text: "BW15"
                        value: LoraSettings.Bandwidth.BW15
                    }
                    ListElement {
                        text: "BW20"
                        value: LoraSettings.Bandwidth.BW20
                    }
                    ListElement {
                        text: "BW31"
                        value: LoraSettings.Bandwidth.BW31
                    }
                    ListElement {
                        text: "BW41"
                        value: LoraSettings.Bandwidth.BW41
                    }
                    ListElement {
                        text: "BW62"
                        value: LoraSettings.Bandwidth.BW62
                    }
                    ListElement {
                        text: "BW125"
                        value: LoraSettings.Bandwidth.BW125
                    }
                    ListElement {
                        text: "BW250"
                        value: LoraSettings.Bandwidth.BW250
                    }
                    ListElement {
                        text: "BW500"
                        value: LoraSettings.Bandwidth.BW500
                    }
                }
            }
            ComboBox {
                id: crBox
                currentIndex: 0
                textRole: "text"
                valueRole: "value"
                model: ListModel {
                    ListElement {
                        text: "CR4/5"
                        value: LoraSettings.CodingRate.CR4_5
                    }
                    ListElement {
                        text: "CR4/6"
                        value: LoraSettings.CodingRate.CR4_6
                    }
                    ListElement {
                        text: "CR4/7"
                        value: LoraSettings.CodingRate.CR4_7
                    }
                    ListElement {
                        text: "CR4/8"
                        value: LoraSettings.CodingRate.CR4_8
                    }
                }
            }

            SpinBox {
                id: spinBox
                from: page.freqMin
                value: page.currentSettings.frequency
                to: page.freqMax
                stepSize: 1000
                editable: true
            }
            Label {
                text: "EDR: 12 bps"
            }

            RowLayout {
                Button {
                    text: "Set"
                }
                Button {
                    id: resetButton
                    text: "↺"
                    onClicked: {
                        spinBox.value = page.currentSettings.frequency
                        sfBox.currentValue = page.currentSettings.spreadingFactor
                        bwBox.currentValue = page.currentSettings.bandwidth
                        crBox.currentValue = page.currentSettings.codingRate
                    }
                }
            }
            Button {
                text: "Auto Neg"
            }

            Button {
                text: "Recover"
                enabled: false
            }
            // Button {
            //     text: "Set Self"
            //     Material.background: Material.Red
            //     onClicked: confSelfConfirmation.visible = true
            // }
        }
    }
    MessageDialog {
        id: confSelfConfirmation
        title: "Configure Own Radio"
        text: " Proceed to configure radio without negotiation? May drop connection."
        visible: false
        buttons: Dialog.Yes | Dialog.No

        onAccepted: console.log("changing")
        onRejected: console.log("aborted")
    }
}
