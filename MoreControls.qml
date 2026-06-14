import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: root
    ColumnLayout {
        Label {
            text: "More controls"
        }
        Button {
            text: "New Flight Dir"
            onClicked: RadioPacketParser.askForNewFlight()
        }
    }
}
