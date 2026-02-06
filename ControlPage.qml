import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: control
    Material.theme: Material.Light

    required property variant window
    required property int flightNumber
    required property date lastFlightNumberUpdate
    required property string stateString

    required property variant fullscreenToggle

    property bool forceBoostMenu: false
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            // Layout.verticalStretchFactor: 1
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Expect Launch"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Boost"
                visible: !control.forceBoostMenu
                onClicked: control.forceBoostMenu = true
                Material.background: Material.Red
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Cancel Boost"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Force Manual"
                visible: !control.forceBoostMenu
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Back"
                visible: control.forceBoostMenu
                onClicked: control.forceBoostMenu = false
            }
            Button {
                Layout.horizontalStretchFactor: 1
                Layout.fillWidth: true
                text: "Really Force"
                Material.background: Material.Red
                visible: control.forceBoostMenu
            }
        }

        RowLayout{
        Label {
        text: ""}
        }

        RowLayout {
            Label {
                text: "State: "
            }
            Label {
                color: "purple"
                font.bold: true
                text: control.stateString
            }
        }

        RowLayout {
            Button {
                text: "New Directory"
            }

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
}
