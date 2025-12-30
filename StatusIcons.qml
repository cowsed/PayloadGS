import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml

Item {

    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "yellow"
    }

    enum LinkAction {
        Idle = 0,
        LinkNegotiating = 1,
        ShellDownload = 2,
        ImageDownload = 3
    }
    enum State {
        // things are fine, received packets recently
        AllGood = 0,
        // haven't heard in a while, temps high,
        Warning = 1,
        // haven't heard in a long time, temps really high,
        Error = 2
    }

    required property State alert
    required property Action action

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Label {
            text: "A"
        }
        Label {
            text: "B"
        }
    }
}
