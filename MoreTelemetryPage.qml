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
        RowLayout {
            TextEdit {
                text: TelemetryLogHolder.baseImu.x
                enabled: false
            }
        }
    }
}
