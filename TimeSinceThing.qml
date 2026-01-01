import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Controls.Material

Label {
    Material.theme: Material.Light

    property string desc
    property date event_time
    property string suffix: ""

    property real stale_seconds: 15
    property real bad_seconds: 30

    property color good_color: undefined // default value of theme
    property color stale_color: "#F6BE00"
    property color bad_color: "#DA291C"

    property real seconds_since: Math.round((new Date().getTime(
                                                 ) - event_time.getTime(
                                                 )) / 1000)

    property color text_color: (seconds_since < stale_seconds) ? good_color : (seconds_since < bad_seconds) ? stale_color : bad_color

    id: label
    text: desc + "XX"
    color: text_color

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            parent.seconds_since = Math.round(
                        (new Date().getTime() - parent.event_time.getTime(
                             )) / 1000)
            label.text = parent.desc + parent.seconds_since + "s" + parent.suffix
        }
    }
}
