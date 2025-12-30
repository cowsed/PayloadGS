import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Controls.Material


Label {
    property string desc
    property date event_time
    id: label
    text: desc + ": " + "XX"

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: label.text = parent.desc + ": " + Math.round(
                         (new Date().getTime() - parent.event_time.getTime(
                              )) / 1000) + "s"
    }
}
