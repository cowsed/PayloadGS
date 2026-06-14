import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: librarian
    Material.theme: Material.Light

    RowLayout {
        ColumnLayout {

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    text: "Timers (s)"
                }

                LibrarianTimerControl {
                    id: flightHearbeatSeconds
                    from: 0
                    to: 30 * 60
                    value: 2 * 60
                    label: "Flight ♥"
                    telem_id: 0
                }
                LibrarianTimerControl {
                    id: landedHearbeatSeconds
                    from: 0
                    to: 30 * 60
                    value: 2 * 60
                    label: "Landed ♥"
                    telem_id: 1
                }
                LibrarianTimerControl {
                    id: orientationSeconds
                    from: 0
                    to: 30 * 60
                    value: 10 * 60
                    label: "IMUs"
                    telem_id: 5
                }
                LibrarianTimerControl {
                    id: gnssSeconds
                    from: 0
                    to: 30 * 60
                    value: 3 * 60
                    label: "GNSS"
                    telem_id: 3
                }
            }
        }

        ColumnLayout {
            SpinBox {
                id: blocksPerRequest
                from: 1
                to: 60
                value: 60
            }
            Label {
                text: "Blocks/req"
            }

            Button {
                text: "Perform Request"
                // on click, pop off the stack and send it to radio
                onClicked: Librarian.SubmitRequestToRadio(RadioPacketParser, blocksPerRequest.value)
            }

            Button {
                text: "Reconnect"
                // on click, pop off the stack and send it to radio
                onClicked: RadioPacketParser.redialServer()
            }
        }
    }
}
