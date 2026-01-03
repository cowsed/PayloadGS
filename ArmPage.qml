import QtQuick

import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Material.theme: Material.Light
    RowLayout {
        anchors.fill: parent
        Item {
            Layout.minimumWidth: 100
            TabButton {
                id: viewButton
                text: "View"
                rotation: -90
                anchors.bottom: seqButton.top
                anchors.horizontalCenter: seqButton.horizontalCenter
            }
            TabButton {
                id: seqButton
                text: "Sequence"
                rotation: -90
            }
        }

        Payload3D {
            Layout.horizontalStretchFactor: 4
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        ColumnLayout {
            Layout.horizontalStretchFactor: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            Label {
                text: "Control"
            }
        }
    }
}
