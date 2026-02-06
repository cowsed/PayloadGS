import QtQuick
import QtQuick.Shapes
import QtQuick.Controls.Material

Rectangle {
    id: back
    property real smaller: Math.min(back.width, back.height)
    property real radius: smaller * .48

    property real fullArcAngle: 290

    property color backColor: "#808080"
    property color textColor: Material.foreground
    property color frontColor: Material.Green

    property string gaugeTitle: "Title" // bottom between arc
    property real gaugeTitleFontSize: 20
    property color gaugeTitleColor: textColor

    property string mainText: "Main" // big number in middle
    property real mainTextFontSize: 28
    property color mainTextColor: textColor

    property string subText: "Sub" // small item below
    property real subTextFontSize: 20
    property color subTextColor: textColor

    property string topText: "Top" // small item on top
    property real topTextFontSize: 20
    property color topTextColor: textColor

    required property real from
    required property real to
    property real value: (from + to) / 2

    property real sweepAngle: (Math.max(
                                   from,
                                   value) - from) / (to - from) * fullArcAngle

    Label {
        id: topTextLabel
        color: back.topTextColor
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: back.topTextFontSize
        anchors.bottom: mainTextLabel.top
        text: back.topText
    }

    Label {
        id: mainTextLabel
        color: back.mainTextColor
        font.pointSize: back.mainTextFontSize
        anchors.centerIn: parent
        text: back.mainText
    }

    Label {
        id: subTextLabel
        color: back.subTextColor
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: back.subTextFontSize
        anchors.top: mainTextLabel.bottom
        text: back.subText
    }

    Label {
        id: titleLabel
        color: back.gaugeTitleColor
        font.pointSize: back.gaugeTitleFontSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: back.gaugeTitle
    }

    Shape {
        id: shape
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            strokeColor: back.backColor
            strokeWidth: 4
            fillColor: "transparent" // Set fill color to transparent for just the arc line
            capStyle: ShapePath.FlatCap // Optional: use round caps for a smoother look

            PathAngleArc {
                centerX: shape.x + shape.width / 2 // Center X coordinate relative to the Shape
                centerY: shape.y + shape.height / 2 // Center Y coordinate relative to the Shape
                radiusX: back.radius // Horizontal radius
                radiusY: back.radius // Vertical radius
                startAngle: -90 - back.fullArcAngle / 2 + back.sweepAngle
                sweepAngle: back.fullArcAngle
                            - back.sweepAngle // Sweep angle in degrees (half circle)
            }
        }
    }

    Shape {
        id: shape2
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        preferredRendererType: Shape.CurveRenderer
        ShapePath {
            strokeColor: back.frontColor
            strokeWidth: 7.5
            fillColor: "transparent" // Set fill color to transparent for just the arc line
            capStyle: ShapePath.FlatCap // Optional: use round caps for a smoother look

            PathAngleArc {
                centerX: shape.x + shape.width / 2 // Center X coordinate relative to the Shape
                centerY: shape.y + shape.height / 2 // Center Y coordinate relative to the Shape
                radiusX: back.radius // Horizontal radius
                radiusY: back.radius // Vertical radius
                startAngle: -90 - back.fullArcAngle
                            / 2 // Start angle in degrees (top of the circle)
                sweepAngle: back.sweepAngle // Sweep angle in degrees (half circle)
            }
        }
    }
}
