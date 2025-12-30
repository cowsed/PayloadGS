import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Material.theme: Material.Light
    anchors.fill: parent
    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            // Item {
            Image {
                // Layout.horizontalStretchFactor: 2
                Layout.preferredWidth: 512
                Layout.preferredHeight: 512
                fillMode: Image.PreserveAspectFit
                source: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/build/WorkingDir/Images/0/sofar.png"
            }
            // }
            Label {
                text: "ID: 0   Size: 159   Pkts: 120"
            }
        }
        // ColumnLayout {
        //     // Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     Label {
        //         // Layout.fillWidth: true
        //         text: "Available"
        //     }

        //     Item {
        //         Layout.fillWidth: true
        //         Layout.preferredWidth: 100
        //         ImagePreview {
        //             // required property string imageName
        //             imageName: imageName
        //             transmissionPercent: transmissionPercent
        //             dir: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/build/WorkingDir/Images/0/"
        //         }
        //     }
        // }
    }
}
