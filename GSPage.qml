import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtCore

ColumnLayout {
    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
    Label {
        text: "GS View"
    }
    Label {
        text: "IDK What to put here bc i can't get the FPS view easily"
    }
    ColumnLayout {
        Label {
            text: "word size: " + SystemInformation.wordSize + " bytes"
        }
        Label {
            text: "byte order: " + SystemInformation.byteOrder
        }

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
}
