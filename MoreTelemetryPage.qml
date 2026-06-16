import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: root

    ColumnLayout {
        Label {
            text: "moarrrr telemetryyy"
        }
        TimeSinceThing {
            desc: "Last Updated: "
            event_time: TelemetryLogHolder.latestImuUpdateTime
            ifNan: "NVR"
        }

        Label {
            text: `Base: ${TelemetryLogHolder.baseImu.x}, ${TelemetryLogHolder.baseImu.y} ${TelemetryLogHolder.baseImu.z}`
        }
        Label {
            text: `Link2: ${TelemetryLogHolder.link2Imu.x}, ${TelemetryLogHolder.link2Imu.y} ${TelemetryLogHolder.link2Imu.z}`
        }
    }
}
