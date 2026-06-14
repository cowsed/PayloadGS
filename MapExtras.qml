import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtPositioning

Item {
    id: root

    Material.theme: Material.Light

    ColumnLayout {

        Label {
            text: "GS Position (Lat, Long)"
        }

        RowLayout {
            TextField {
                id: latitudeInput
                placeholderText: "Latitude"
                Layout.minimumWidth: 300

                // Restricts input to decimals with up to 2 decimal places
                validator: DoubleValidator {
                    bottom: -90
                    top: 90
                    decimals: 7
                }

                // Allows decimal points and minus signs on virtual keyboards
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
            TextField {
                id: longitudeInput
                placeholderText: "Longitude"
                Layout.minimumWidth: 300

                // Restricts input to decimals with up to 2 decimal places
                validator: DoubleValidator {
                    bottom: -180
                    top: 180
                    decimals: 7
                }

                // Allows decimal points and minus signs on virtual keyboards
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
            Button {
                text: "Set"
                onClicked: function () {
                    console.log("Setting position");
                    if (!latitudeInput.acceptableInput) {
                        console.log("Bad Latitude input");
                        return;
                    }
                    if (!longitudeInput.acceptableInput) {
                        console.log("Bad Longitude input");
                        return;
                    }
                    const coord = QtPositioning.coordinate(Number(latitudeInput.text), Number(longitudeInput.text), 0);
                    TelemetryLogHolder.newStationPosition(new Date(), coord);
                }
            }
        }
    }
}
