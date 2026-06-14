import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtCharts

// import QtSensors
Item {
    id: page
    Material.theme: Material.Light

    property real freqMin: 410000000
    property real freqMax: 450000000

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ChartView {
                id: radioChart
                legend.alignment: Qt.AlignTop
                antialiasing: true

                Layout.topMargin: 0

                margins.top: 0
                margins.bottom: 0

                Layout.fillWidth: true
                Layout.fillHeight: true
                theme: ChartView.ChartThemeQt

                Connections {
                    target: RadioPacketParser.radioRSSI

                    function onValueChanged() {
                        RadioPacketParser.radioRSSI.fillXYSeries(radioChart.series(radioRSSISeries.name));
                    }
                }

                Connections {
                    target: RadioPacketParser.radioSNR

                    function onValueChanged() {
                        RadioPacketParser.radioSNR.fillXYSeries(radioChart.series(radioSNRSeries.name));
                        const starts = [RadioPacketParser.radioSNR.earliestTime, RadioPacketParser.radioRSSI.earliestTime];
                        const ends = [RadioPacketParser.radioSNR.latestTime, RadioPacketParser.radioRSSI.latestTime];

                        axisX.min = new Date(Math.min(...starts));
                        axisX.max = new Date(Math.max(...ends));
                    }
                }

                DateTimeAxis {
                    id: axisX
                    tickCount: 6
                    format: "hh:mm:ss"
                }

                ValueAxis {
                    id: rssiAxisY
                    titleText: "RSSI"
                    min: -130
                    max: 10
                    color: "red"
                }
                ValueAxis {
                    id: snrAxisY
                    titleText: "SNR"
                    min: -40
                    max: 20
                    color: "blue"
                }
                LineSeries {
                    id: radioRSSISeries
                    name: "RSSI"
                    axisX: axisX
                    axisY: rssiAxisY

                    color: "red"
                    width: 2
                    pointsVisible: true
                }
                LineSeries {
                    id: radioSNRSeries
                    name: "SNR"
                    axisX: axisX
                    axisYRight: snrAxisY

                    color: "blue"
                    width: 2
                    pointsVisible: true
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                ComboBox {
                    id: sfBox
                    currentIndex: 5
                    textRole: "text"
                    valueRole: "value"

                    model: ListModel {
                        // ListElement {
                        //     text: "SF6"
                        //     value: LoraSettings.SpreadingFactor.SF6
                        // }
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
                    currentIndex: 1
                    textRole: "text"
                    valueRole: "value"

                    model: ListModel {
                        // ListElement {
                        //     text: "BW8"
                        //     value: LoraSettings.Bandwidth.BW8
                        // }
                        // ListElement {
                        //     text: "BW10"
                        //     value: LoraSettings.Bandwidth.BW10
                        // }
                        // ListElement {
                        //     text: "BW15"
                        //     value: LoraSettings.Bandwidth.BW15
                        // }
                        // ListElement {
                        //     text: "BW20"
                        //     value: LoraSettings.Bandwidth.BW20
                        // }
                        // ListElement {
                        //     text: "BW31"
                        //     value: LoraSettings.Bandwidth.BW31
                        // }
                        // ListElement {
                        //     text: "BW41"
                        //     value: LoraSettings.Bandwidth.BW41
                        // }
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
                    currentIndex: 3
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
                    id: remoteDbm
                    from: -8
                    value: -8
                    to: 22
                    editable: true
                }

                SpinBox {
                    id: freqBox
                    from: page.freqMin
                    value: RadioPacketParser.loraSettings.frequency
                    to: page.freqMax
                    stepSize: 1000
                    editable: true
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "Neg"
                onClicked: RadioPacketParser.negotiateLoraParams(freqBox.value, sfBox.currentValue, bwBox.currentValue, crBox.currentValue, remoteDbm.value)
            }

            Button {
                text: "Auto"
            }

            Button {
                id: resetButton
                text: "↺"
                onClicked: {
                    freqBox.value = RadioPacketParser.loraSettings.frequency;
                    sfBox.currentValue = RadioPacketParser.loraSettings.spreadingFactor;
                    bwBox.currentValue = RadioPacketParser.loraSettings.bandwidth;
                    crBox.currentValue = RadioPacketParser.loraSettings.codingRate;
                }
            }
            Button {
                text: "Recover"
                enabled: false
            }
            Button {
                text: "Call"
                onClicked: RadioPacketParser.sendCallsign()
            }

            Button {
                text: "Set Self"
                Material.background: Material.Red
                onClicked: confSelfConfirmation.visible = true
            }
            SpinBox {
                id: localDbm
                from: 2
                value: 14
                to: 17
                editable: true
                onValueChanged: RadioPacketParser.setTxPower(value)
            }
        }
    }
    MessageDialog {
        id: confSelfConfirmation
        title: "Configure Own Radio"
        text: " Proceed to configure radio without negotiation? May drop connection."
        visible: false
        buttons: Dialog.Yes | Dialog.No

        onAccepted: RadioPacketParser.setLocalLoraParams(freqBox.value, sfBox.currentValue, bwBox.currentValue, crBox.currentValue)
        onRejected: console.log("aborted")
    }
}
