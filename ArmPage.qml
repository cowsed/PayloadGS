import QtQuick
import QtQuick.Controls

import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    Material.theme: Material.Light
    id: page

    property real shoulderYaw: 0
    property real shoulderPitch: 0
    property real elbowPitch: 0
    property real wristPitch: 0

    property bool showWalls: true
    Timer {
        interval: 10
        running: true // set to true for fun
        repeat: true
        onTriggered: {
            let t = new Date().getTime() / 400
            page.shoulderYaw = 90 * Math.sin(t)
        }
    }

    RowLayout {
        anchors.fill: parent

        Payload3D {
            id: visualization
            Layout.horizontalStretchFactor: 4
            Layout.fillWidth: true
            Layout.fillHeight: true

            shoulderYaw: page.shoulderYaw
            shoulderPitch: page.shoulderPitch
            elbowPitch: page.elbowPitch
            wristPitch: page.wristPitch

            showGhost: true
            ghostShoulderYaw: shoulderYawSlider.value
            ghostShoulderPitch: shoulderPitchSlider.value
            ghostElbowPitch: elbowPitchSlider.value
            ghostWristPitch: wristPitchSlider.value

            showWalls: wallTransparency.checked
            wallsOpen: wallOpen.checked

            Button {
                id: showAxesButton
                anchors.left: visualization.left
                anchors.top: visualization.top
                anchors.topMargin: 4
                anchors.leftMargin: 4
                checkable: true
                checked: true

                Material.roundedScale: Material.ExtraSmallScale
                height: 60
                width: 60
                Material.background: "white"
                padding: 0
                topInset: 0
                leftInset: 0
                rightInset: 0
                bottomInset: 0

                icon.height: 70
                icon.width: 70
                icon.source: checked ? "qrc:/assets/images/axes_shown_icon.png" : "qrc:/assets/images/axes_hidden_icon.png"
            }

            Button {
                id: resetButton
                anchors.left: visualization.left
                anchors.top: showAxesButton.bottom
                anchors.topMargin: 4
                anchors.leftMargin: 4

                Material.roundedScale: Material.ExtraSmallScale
                height: 60
                width: 60
                Material.background: "white"
                padding: 0
                topInset: 0
                leftInset: 0
                rightInset: 0
                bottomInset: 0

                text: "⟲"

                onClicked: {
                    shoulderYawSlider.value = page.shoulderYaw
                    shoulderPitchSlider.value = page.shoulderPitch
                    elbowPitchSlider.value = page.elbowPitch
                    wristPitchSlider.value = page.wristPitch
                }
            }

            Button {
                id: wallTransparency

                anchors.left: visualization.left
                anchors.top: resetButton.bottom
                Material.roundedScale: Material.ExtraSmallScale
                height: 60
                width: 60

                checkable: true
                checked: true

                icon.height: 70
                icon.width: 70
                icon.source: checked ? "qrc:/assets/images/wall_shown_image.png" : "qrc:/assets/images/wall_hidden_image.png"

                Material.background: "white"
                padding: 0

                anchors.topMargin: 4
                anchors.leftMargin: 4
                topInset: 0
                leftInset: 0
                rightInset: 0
                bottomInset: 0
            }
            Button {
                id: wallOpen
                font.pointSize: 12
                text: "Open"
                checkable: true
                checked: true

                anchors.left: visualization.left
                anchors.top: wallTransparency.bottom
                Material.roundedScale: Material.ExtraSmallScale
                height: 60
                width: 60
                Material.background: "white"
                padding: 0
                anchors.topMargin: 20
                anchors.bottomMargin: 0
                anchors.leftMargin: 4
            }
            Button {
                id: takePictureButton
                anchors.right: visualization.right
                anchors.top: visualization.top
                anchors.topMargin: 4
                anchors.rightMargin: 4

                Material.foreground: "white"
                checkable: true
                checked: true

                Material.roundedScale: Material.ExtraSmallScale
                height: 60
                width: 60
                Material.background: "white"
                padding: 0
                topInset: 0
                leftInset: 0
                rightInset: 0
                bottomInset: 0

                icon.height: 70
                icon.width: 70
                icon.source: checked ? "qrc:/assets/images/picture.png" : "qrc:/assets/images/no_picture.png"
            }
        }

        ColumnLayout {
            Layout.horizontalStretchFactor: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            RowLayout {
                AngleSliderWithBox {
                    id: shoulderYawSlider
                    from: -110
                    to: 110
                    label: "Y"
                    value: 0
                    current: page.shoulderYaw
                }
                AngleSliderWithBox {
                    id: shoulderPitchSlider
                    from: -90
                    to: 90
                    label: "S"
                    value: 0
                    current: page.shoulderPitch
                }
                AngleSliderWithBox {
                    id: elbowPitchSlider
                    from: -180
                    to: 180
                    label: "E"
                    value: 0
                    current: page.elbowPitch
                }
                AngleSliderWithBox {
                    id: wristPitchSlider
                    from: -90
                    to: 90
                    label: "W"
                    value: 0
                    current: page.wristPitch
                }
            }
            RowLayout {
                Button {
                    text: "Move"
                }
                Button {
                    text: "Set Idle"
                    Material.background: Material.Red
                }
            }

            TimeSinceThing {
                desc: "Angles: "
                event_time: new Date()
            }
            TimeSinceThing {
                desc: "Accels: "
                event_time: new Date()
            }
        }
    }
}
