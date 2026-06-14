import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtCore
import QtPositioning

ApplicationWindow {
    id: mainwindow
    width: 1920 / 2
    height: 1080 / 2

    visible: true
    title: qsTr("Payload Groundstation")

    Material.theme: Material.Light
    Material.accent: Material.Orange

    Material.primary: Qt.rgba(208, 211, 212)
    Material.foreground: Qt.rgba(0, 0, 0, 1)
    Material.background: Qt.rgba(1, 1, 1, 1)

    font.family: "Roboto"
    font.weight: Font.ExtraBold
    font.pointSize: 13

    Component.onCompleted: {

        // toggleFullscreen()
    }

    property var phase: RadioPacketParser.Starting
    property payloadFlags pflags
    property bool flagsValid: false
    property string flightDir: flight_dir

    Settings {
        id: settings
        property alias x: mainwindow.x
        property alias y: mainwindow.y
        property alias width: mainwindow.width
        property alias height: mainwindow.height
        property alias flight_dir: mainwindow.flightDir
    }

    function qsIfNanOrNum(num, numPlaces) {
        if (isNaN(num)) {
            return "???";
        }
        return num.toFixed(numPlaces);
    }
    function defaultIfNan(num, def) {
        if (isNaN(num)) {
            return def;
        }
        return num;
    }
    Connections {
        target: RadioPacketParser
        function onFlightStateUpdated(time, phase, flags, s_since_boost) {
            mainwindow.phase = phase;
            mainwindow.pflags = RadioPacketParser.statusBitsToFlags(flags);
            mainwindow.flagsValid = true;

            const phaseStr = RadioPacketParser.phaseToShortString(mainwindow.phase);
            if (phaseStr == "Flight") {
                controlPage.stateString = `${phaseStr}: ${s_since_boost}s`;
            } else {
                controlPage.stateString = phaseStr;
            }
        }
    }

    function toggleFullscreen() {
        if (mainwindow.visibility === Window.FullScreen) {
            mainwindow.visibility = Window.Windowed;
        } else {
            mainwindow.visibility = Window.FullScreen;
        }
    }

    property real initTab: 1
    property real initV: 1

    header: TabBar {
        id: tabbar
        Layout.fillWidth: true
        currentIndex: mainwindow.initTab

        TabButton {
            id: mapTabButton
            text: "Map"
            onClicked: metaview.currentIndex = 1
        }
        TabButton {
            id: controlTabButton
            text: "Control"
            onClicked: metaview.currentIndex = 1
        }
        TabButton {
            id: telemTabButton
            text: "Telemetry"
            onClicked: metaview.currentIndex = 1
        }
        TabButton {
            id: armTabButton
            text: "Arm"
            onClicked: metaview.currentIndex = 1
        }
        TabButton {
            id: imageTabButton
            text: "Image"
            onClicked: metaview.currentIndex = 1
        }
        TabButton {
            id: radioTabButton
            text: "Radio"
            onClicked: metaview.currentIndex = 1
        }

        Shortcut {
            sequences: ["Alt+m", "Alt+1"]
            onActivated: mapTabButton.click()
        }

        Shortcut {
            sequences: ["Alt+Shift+m", "Alt+Shift+1"]
            onActivated: {
                mapTabButton.click();
                metaview.currentIndex = 0;
            }
        }

        Shortcut {
            sequences: ["Alt+c", "Alt+2"]
            onActivated: controlTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+Shift+c", "Alt+Shift+2"]
            onActivated: {
                controlTabButton.click();
                metaview.currentIndex = 0;
            }
        }
        Shortcut {
            sequences: ["Alt+t", "Alt+3"]
            onActivated: telemTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+Shift+t", "Alt+Shift+3"]
            onActivated: {
                telemTabButton.click();
                metaview.currentIndex = 0;
            }
        }
        Shortcut {
            sequences: ["Alt+a", "Alt+4"]
            onActivated: armTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+Shift+a", "Alt+Shift+4"]
            onActivated: {
                armTabButton.click();
                metaview.currentIndex = 0;
            }
        }
        Shortcut {
            sequences: ["Alt+i", "Alt+5"]
            onActivated: imageTabButton.click()
        }
        Shortcut {
            sequences: ["Alt+Shift+i", "Alt+Shift+5"]
            onActivated: {
                imageTabButton.click();
                metaview.currentIndex = 0;
            }
        }
        Shortcut {
            sequences: ["Alt+r", "Alt+6"]
            onActivated: radioTabButton.click()
        }

        Shortcut {
            sequences: ["Alt+Shift+r", "Alt+Shift+6"]
            onActivated: {
                radioTabButton.click();
                metaview.currentIndex = 0;
            }
        }
        Shortcut {
            sequences: ["Alt+g"]
            onActivated: metaview.currentIndex = 2
        }

        onCurrentIndexChanged: {
            mainview.currentIndex = currentIndex;
            detailView.currentIndex = currentIndex;
        }
    }

    SwipeView {
        id: metaview
        anchors.fill: parent
        orientation: Qt.Vertical
        currentIndex: mainwindow.initV

        SwipeView {
            id: detailView
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: mainwindow.initTab
            onCurrentIndexChanged: {
                tabbar.currentIndex = currentIndex;
            }

            MapExtras {
                // over map
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            MoreControls {
                // over controls
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            MoreTelemetryPage {
                // over telem
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ArmSequenceController {
                // over arm
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            RecropPage {
                // over image
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            LibrarianPage {
                // over radio
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            FakePacketEmitter {
                // over shell
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        SwipeView {
            id: mainview
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: mainwindow.initTab

            onCurrentIndexChanged: {
                tabbar.currentIndex = currentIndex;
            }

            MapPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ControlPage {
                id: controlPage
                Layout.fillWidth: true
                Layout.fillHeight: true

                window: mainwindow
                stateString: RadioPacketParser.phaseToShortString(mainwindow.phase)
                pflags: mainwindow.pflags
            }

            TelemetryPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ArmPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ImageControl {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RadioPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ShellPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        GSPage {
            Layout.fillWidth: true
            Layout.fillHeight: true

            flightDir: mainwindow.flightDir
            fullscreenToggle: mainwindow.toggleFullscreen
            setFlightDirAndRestart: function (newDir) {
                if (TelemetryLogHolder.newDirectory(newDir)) {
                    mainwindow.flightDir = newDir;
                    console.log("Should restart");
                }
            }
        }
    }

    footer: ToolBar {
        font.weight: Font.Normal
        font.pointSize: 12

        implicitHeight: 25
        RowLayout {
            anchors.fill: parent
            Layout.bottomMargin: 0
            Layout.topMargin: 0
            Layout.fillHeight: true

            Rectangle {
                Layout.fillHeight: true
                Layout.minimumWidth: 10
            }

            Label {
                text: RadioPacketParser.phaseToShortString(mainwindow.phase) + "📡"
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
            }
            ToolSeparator {}

            Label {
                text: "🔋 " + mainwindow.qsIfNanOrNum(TelemetryLogHolder.batteryVoltage.latestValue, 1) + "V"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}

            Label {
                text: RadioPacketParser.loraSettings.spreadingFactorString(RadioPacketParser.loraSettings.spreadingFactor)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: RadioPacketParser.loraSettings.bandwidthString(RadioPacketParser.loraSettings.bandwidth)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: RadioPacketParser.loraSettings.codingRateString(RadioPacketParser.loraSettings.codingRate)
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "F " + (RadioPacketParser.loraSettings.frequency / 1000000).toFixed(3) + " MHz"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: RadioPacketParser.statusLine
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            TimeSinceThing {
                desc: "RX: "
                event_time: RadioPacketParser.latestRxDateTime
                ifNan: "NVR"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            TimeSinceThing {
                desc: "TX: "
                event_time: RadioPacketParser.latestTxDateTime
                ifNan: "NVR"
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "Rem: " + RadioPacketParser.numLeftBeforeResponse
                Layout.alignment: Qt.AlignTop
            }
            ToolSeparator {}
            Label {
                text: "Rq: " + Librarian.numRequests
                Layout.alignment: Qt.AlignTop
            }
            Rectangle {
                Layout.fillHeight: true
                Layout.minimumWidth: 10
            }
        }
    }
}
