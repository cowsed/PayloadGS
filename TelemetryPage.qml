import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "Pages/Telemetry/"

Item {
    id: telem
    Material.theme: Material.Light

    ColumnLayout {
        anchors.fill: parent

        StorageGauges {
            Layout.verticalStretchFactor: 2
        }
        TempPlot {
            Layout.verticalStretchFactor: 2
        }
    }
}
