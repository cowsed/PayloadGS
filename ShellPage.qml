import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Material.theme: Material.Light

    RowLayout {
        ColumnLayout {
            Layout.fillHeight: true
            Layout.maximumWidth: 200

            Label {
                text: "0: ls"
            }
            Label {
                text: "1: top | head -n 5"
            }
        }

        ColumnLayout {
            Layout.fillWidth: true

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                TextField {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    text: "cmd"

                    cursorVisible: true
                    readOnly: false
                }
                Button {
                    text: "Execute"
                }
            }
        }
    }
}
