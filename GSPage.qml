import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtCore
import QtQuick.Dialogs

ColumnLayout {
    required property variant fullscreenToggle

    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
    Label {
        text: "GS View"
    }
    Label {
        text: "IDK What to put here bc i can't get the FPS view easily"
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
            onClicked: fullscreenToggle()
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
}
