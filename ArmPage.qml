import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: page
    Material.theme: Material.Light

    property real shoulderYaw: 0
    property real shoulderPitch: 0
    property real elbowPitch: 0
    property real wristPitch: 0
    property bool comeBack: true

    Connections {
        target: RadioPacketParser
        function onArmAnglesUpdated(time, shoulder_yaw, shoulder_pitch, elbow_pitch, wrist_pitch) {
            timeSinceAngles.event_time = time;
            page.shoulderYaw = shoulder_yaw;
            page.shoulderPitch = shoulder_pitch;
            page.elbowPitch = elbow_pitch;
            page.wristPitch = wrist_pitch;
            console.log("ArmAnglesUpdated", time, shoulder_yaw, shoulder_pitch, elbow_pitch, wrist_pitch);
        }
    }

    property bool showWalls: true

    function explainMovement() {
        let s = "Take Picture: " + (takePictureButton.checked ? "yes" : "no") + "\n";
        s += "Shoulder Yaw: " + shoulderYawSlider.value.toFixed(1) + "\n";
        s += "Shoulder Pitch: " + shoulderPitchSlider.value.toFixed(1) + "\n";
        s += "Elbow Pitch: " + elbowPitchSlider.value.toFixed(1) + "\n";
        s += "Wrist Pitch: " + wristPitchSlider.value.toFixed(1) + "\n";
        s += "Come Back: " + page.comeBack + "\n";
        return s;
    }

    // Timer {
    //     interval: 10
    //     running: true // set to true for fun
    //     repeat: true
    //     onTriggered: {
    //         let t = new Date().getTime() / 400;
    //         page.shoulderYaw = 90 * Math.sin(t);
    //     }
    // }

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

            showGhost: showGhostButton.checked
            showAxes: showAxesButton.checked
            ghostShoulderYaw: shoulderYawSlider.value
            ghostShoulderPitch: shoulderPitchSlider.value
            ghostElbowPitch: elbowPitchSlider.value
            ghostWristPitch: wristPitchSlider.value

            showWalls: wallTransparency.checked
            leftWallOpenness: wallOpenness.value
            rightWallOpenness: wallOpenness.value

            ColumnLayout {
                anchors.left: visualization.left
                anchors.top: visualization.top
                anchors.bottom: visualization.bottom
                anchors.leftMargin: 4
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                spacing: 4
                width: 60

                Button {
                    id: showAxesButton
                    checkable: true
                    checked: true

                    topInset: 0
                    bottomInset: 0

                    Material.roundedScale: Material.ExtraSmallScale
                    Material.background: "white"

                    Layout.fillWidth: true
                    Layout.preferredHeight: width

                    icon.height: 70
                    icon.width: 70
                    icon.source: checked ? "qrc:/assets/images/axes_shown_icon.png" : "qrc:/assets/images/axes_hidden_icon.png"
                }

                Button {
                    id: showGhostButton
                    checkable: true
                    checked: true

                    topInset: 0
                    bottomInset: 0

                    Material.roundedScale: Material.ExtraSmallScale
                    Material.background: "white"

                    Layout.fillWidth: true
                    Layout.preferredHeight: width

                    icon.height: 70
                    icon.width: 70
                    icon.source: checked ? "qrc:/assets/images/ghost_hidden_icon.png" : "qrc:/assets/images/ghost_hidden_icon.png"
                }

                Button {
                    id: wallTransparency
                    topInset: 0
                    bottomInset: 0
                    Material.roundedScale: Material.ExtraSmallScale

                    checkable: true
                    checked: true

                    icon.height: 64
                    icon.width: 64
                    icon.source: checked ? "qrc:/assets/images/wall_shown_icon.png" : "qrc:/assets/images/wall_hidden_icon.png"

                    Material.background: "white"
                    padding: 0

                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                }

                Rectangle {

                    Layout.minimumHeight: 120
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    radius: 10
                    Slider {
                        id: wallOpenness
                        enabled: wallTransparency.checked
                        anchors.topMargin: 12
                        anchors.bottomMargin: 12
                        orientation: Qt.Vertical
                        from: 1
                        to: 0
                        value: 0
                        anchors.fill: parent
                        anchors.centerIn: parent
                    }
                }
                Button {
                    id: resetButton

                    Material.roundedScale: Material.ExtraSmallScale
                    Material.background: "white"

                    Layout.fillWidth: true
                    Layout.preferredHeight: width
                    topInset: 0
                    bottomInset: 0
                    text: "⟲"

                    onClicked: {
                        shoulderYawSlider.value = page.shoulderYaw;
                        shoulderPitchSlider.value = page.shoulderPitch;
                        elbowPitchSlider.value = page.elbowPitch;
                        wristPitchSlider.value = page.wristPitch;
                    }
                }
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
                    actual: page.shoulderYaw
                }
                AngleSliderWithBox {
                    id: shoulderPitchSlider
                    from: -90
                    to: 90
                    label: "S"
                    actual: page.shoulderPitch
                }
                AngleSliderWithBox {
                    id: elbowPitchSlider
                    from: -180
                    to: 180
                    label: "E"
                    actual: page.elbowPitch
                }
                AngleSliderWithBox {
                    id: wristPitchSlider
                    from: -90
                    to: 90
                    label: "W"
                    actual: page.wristPitch
                }
            }
            RowLayout {
                Button {
                    text: "Move"
                    onClicked: function () {
                        page.comeBack = false;
                        confMovement.visible = true;
                    }
                }
                Button {
                    text: "Set Idle"
                    Material.background: Material.Red
                }
            }
            Button {
                text: "Move and Back"
                onClicked: function () {
                    page.comeBack = true;
                    confMovement.visible = true;
                }
            }

            TimeSinceThing {
                id: timeSinceAngles
                desc: "Angles: "
            }
            TimeSinceThing {
                desc: "Accels: "
                event_time: new Date()
            }
        }
    }

    MessageDialog {
        id: confMovement
        title: "Arm Movement"
        text: "Is this what you want to do: " + page.explainMovement()
        visible: false
        buttons: Dialog.Yes | Dialog.No

        onAccepted: function () {
            if (takePictureButton.checked && page.comeBack) {
                console.log("go and back with pic");
                RadioPacketParser.askToGoToPositionAndComeBack(page.shoulderYaw, page.shoulderPitch, page.elbowPitch, page.wristPitch);
            } else if (!takePictureButton.checked && page.comeBack) {
                console.log("go and back no pic");
                RadioPacketParser.askToGoToPositionAndComeBack(page.shoulderYaw, page.shoulderPitch, page.elbowPitch, page.wristPitch);
            } else {
                console.log("just go not back");
                RadioPacketParser.askToGoToPosition(page.shoulderYaw, page.shoulderPitch, page.elbowPitch, page.wristPitch);
            }
        }

        onRejected: console.log("aborted")
    }
}
