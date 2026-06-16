import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Controls.Material

Label {
    id: label
    Material.theme: Material.Light

    property string desc: ""
    property date event_time
    property string suffix: ""
    property string ifNan

    property real stale_seconds: 15
    property real bad_seconds: 30

    property color good_color: Material.foreground
    property color stale_color: "#F6BE00"
    property color bad_color: "#DA291C"

    property real seconds_since: Math.round((new Date().getTime() - event_time.getTime()) / 1000)

    property color text_color: (seconds_since < stale_seconds) ? good_color : (seconds_since < bad_seconds) ? stale_color : bad_color
    text: desc + "XX"
    color: text_color

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            parent.seconds_since = Math.round((new Date().getTime() - parent.event_time.getTime()) / 1000);
            label.text = (!isNaN(parent.seconds_since) || label.ifNan == undefined) ? (parent.desc + parent.seconds_since + "s" + parent.suffix) : (parent.desc + label.ifNan);
        }
    }
}
