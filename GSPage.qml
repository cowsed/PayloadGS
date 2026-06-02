import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs

RowLayout {
    id: root
    required property variant fullscreenToggle

    RowLayout {
        ColumnLayout {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            Label {
                text: "GS Info 🎷🐛"
            }
            ColumnLayout {

                Label {
                    text: "Host: " + SystemInformation.machineHostName
                }
                Label {
                    text: "platform: " + SystemInformation.prettyProductName
                }
                Label {
                    text: "version: " + SystemInformation.productVersion
                }
                Label {
                    text: "kernel type: " + SystemInformation.kernelType
                }
                Label {
                    text: "kernel version: " + SystemInformation.kernelVersion
                }
                Label {
                    text: "arch: " + SystemInformation.currentCpuArchitecture
                }
            }
            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Exit"
                    onClicked: exitConf.visible = true
                }

                Button {
                    text: "Fullscreen"
                    onClicked: root.fullscreenToggle()
                }
            }
            MessageDialog {
                id: exitConf
                title: "Exit Application"
                text: " Are you sure you want to exit"
                visible: false
                buttons: Dialog.Yes | Dialog.No

                onAccepted: Qt.callLater(Qt.quit)
                onRejected: console.log("aborted")
            }
            Label {
                Layout.maximumWidth: 500
                text: "Dir: " + flight_dir
            }
        }

        Flickable {
            id: flick

            Layout.fillHeight: true
            Layout.fillWidth: true

            contentWidth: edit.contentWidth
            contentHeight: edit.contentHeight
            clip: true

            function ensureVisible(r) {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX + width <= r.x + r.width)
                    contentX = r.x + r.width - width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY + height <= r.y + r.height)
                    contentY = r.y + r.height - height;
            }

            TextEdit {
                id: edit
                color: "red"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: LogProvider.logText
                readOnly: true
                wrapMode: TextArea.Wrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
            }
        }
    }
}
