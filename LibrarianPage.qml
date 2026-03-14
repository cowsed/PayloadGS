import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: librarian
    Material.theme: Material.Light

    RowLayout {
        ColumnLayout {
            Label {
                text: "Ongoing downloads"
            }
            Label {
                text: "timers"
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
