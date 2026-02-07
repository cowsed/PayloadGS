import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

View3D {
    id: v3d
    camera: cameraNode

    property real shoulderYaw: 0
    property real shoulderPitch: 0
    property real elbowPitch: 0
    property real wristPitch: 0

    property bool showGhost: false
    property real ghostShoulderYaw: 0
    property real ghostShoulderPitch: 0
    property real ghostElbowPitch: 0
    property real ghostWristPitch: 0

    property bool showWalls: false
    property bool showAxes: false

    property real rightWallOpenness: 0
    property real leftWallOpenness: 0

    property vector3d baseOrientation: Qt.vector3d(.1, -1, 0).normalized()
    property vector3d upperArmOrientation: Qt.vector3d(0, -1, 0)
    property vector3d foreArmOrientation: Qt.vector3d(0, -1, 0)

    property vector3d referenceBaseOrientation: Qt.vector3d(0, -1, 0)

    Timer {
        interval: 10
        running: false // set to true for fun
        repeat: true
        onTriggered: {
            let t = new Date().getTime() / 400
            v3d.baseOrientation = Qt.vector3d(Math.sin(t), -1,
                                              Math.sin(3 * t / 2)).normalized()
        }
    }

    function quatFromImuAndReference() {
        // https://github.com/toji/gl-matrix/blob/f0583ef53e94bc7e78b78c8a24f09ed5e2f7a20c/src/gl-matrix/quat.js#L54
        // https://stackoverflow.com/questions/18558910/direction-vector-to-rotation-matrix
        let ref = v3d.referenceBaseOrientation.normalized()
        let me = v3d.baseOrientation.normalized()
        let a = ref.crossProduct(me)
        let d = ref.dotProduct(me)

        let s = ref.length() * me.length() + d

        let q = Qt.quaternion(s, a.x, a.y, a.z).normalized()

        return q
    }
    property quaternion payloadOrientation: quatFromImuAndReference()
    property real minY: payload.calcBounds(payloadOrientation)

    Node {
        id: originNode
        position.y: 1000 * (v3d.minY + .015)
        PerspectiveCamera {
            id: cameraNode
            y: 110
            z: 450
        }
        Node {
            id: targetNode
        }
    }
    OrbitCameraController {
        id: orbitController
        anchors.fill: parent
        origin: originNode
        camera: cameraNode
        panEnabled: false
    }

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lightProbe: Texture {
            source: "assets/goegap_2k.hdr"
        }
    }

    DirectionalLight {
        eulerRotation.x: -120
        eulerRotation.y: 235
        eulerRotation.z: 5
        castsShadow: true
        shadowMapQuality: Light.ShadowMapQualityVeryHigh
        shadowMapFar: 1000
    }

    Model {
        id: basePlane

        geometry: CylinderGeometry {
            id: basePlaneGeom
            length: 10
            radius: 250
            rings: 0
            segments: 50
        }

        eulerRotation.x: 0
        position.y: v3d.minY * 1000 - basePlaneGeom.length / 2

        materials: PrincipledMaterial {
            baseColor: "#20a040"
            roughness: 0.9 // make specular highlight visible
        }
        castsShadows: true
        receivesShadows: true
    }

    // Start with a simple material, using the built-in implementation for everything.
    Payload {
        id: payload
        scale: Qt.vector3d(1000, 1000, 1000)

        orientation: v3d.payloadOrientation
        base_imu: v3d.baseOrientation

        shoulderYaw: v3d.shoulderYaw
        shoulderPitch: v3d.shoulderPitch
        elbowPitch: v3d.elbowPitch
        wristPitch: v3d.wristPitch

        showGhost: v3d.showGhost
        showAxes: v3d.showAxes
        ghostShoulderYaw: v3d.ghostShoulderYaw
        ghostShoulderPitch: v3d.ghostShoulderPitch
        ghostElbowPitch: v3d.ghostElbowPitch
        ghostWristPitch: v3d.ghostWristPitch

        wallOpacity: v3d.showWalls ? 0.3 : 0
        leftWallOpenness: v3d.rightWallOpenness
        rightWallOpenness: v3d.leftWallOpenness

        rotation: v3d.payloadOrientation

        // BaseJoint {
        //     id: baseJoint
        // }

        // UpperArm {
        //     id: upperArm
        //     position.x: -0.109769
        //     position.y: 0.015418
        //     // 0
        //     eulerRotation.y: v3d.shoulderYaw
        //     eulerRotation.z: v3d.shoulderPitch
        //     ForeArm {
        //         id: forearm
        //         position.y: 0.115517
        //         eulerRotation.z: v3d.elbowPitch
        //         Wrist {
        //             id: wrist
        //             position.y: 0.085852
        //             eulerRotation.z: v3d.wristPitch
        //         }
        //     }
        // }
        // UpperArm {
        //     id: upperArmGhost
        //     position.x: -0.109769
        //     position.y: 0.015418
        //     isGhost: true

        //     visible: v3d.showGhost
        //     eulerRotation.y: v3d.ghostShoulderYaw
        //     eulerRotation.z: v3d.ghostShoulderPitch

        //     ForeArm {
        //         id: forearmGhost
        //         position.y: 0.115517
        //         isGhost: true
        //         eulerRotation.z: v3d.ghostElbowPitch

        //         Wrist {
        //             id: wristGhost
        //             position.y: 0.085852
        //             eulerRotation.z: v3d.ghostWristPitch
        //             isGhost: true
        //         }
        //     }
        // }
    }
}
