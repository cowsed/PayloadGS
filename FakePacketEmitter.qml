import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Layout.fillWidth: true

    ColumnLayout {
        Layout.fillWidth: true
        Label {
            Layout.fillWidth: true
            text: "Fake Packet"
        }
        TextField {
            id: b64
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 800

            text: "b64"

            cursorVisible: true
            readOnly: false
        }
        Button {
            Layout.fillWidth: true
            text: "Receive"
            onPressed: {
                RadioPacketParser.b64PacketReceived(new Date(), 1, 1, b64.text);
            }
        }
    }
}
