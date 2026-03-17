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
                }
                LibrarianTimerControl {
                    id: landedHearbeatSeconds
                    from: 0
                    to: 30 * 60
                    value: 2 * 60
                    label: "Landed ♥"
                }
                LibrarianTimerControl {
                    id: sysInfoSeconds
                    from: 0
                    to: 30 * 60
                    value: 10 * 60
                    label: "SysInfo"
                }
                LibrarianTimerControl {
                    id: gnssSeconds
                    from: 0
                    to: 30 * 60
                    value: 3 * 60
                    label: "GNSS"
                }
            }
        }
        ColumnLayout {
            Label {
                text: "Queue"
            }
            Label {
                text: "telem: GNSS"
            }
            Label {
                text: "MIid: 0"
            }
            Label {
                text: "Iid: 0, bid: 0"
            }
            Label {
                text: "Iid: 0, bid: 1"
            }
            Label {
                text: "Sid: 0, bid: 0"
            }
        }
    }
}
