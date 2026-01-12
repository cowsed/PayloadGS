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
    property bool wallsOpen: false

    property vector3d baseOrientation: Qt.vector3d(0, -1, 0)
    property vector3d upperArmOrientation: Qt.vector3d(0, -1, 0)
    property vector3d foreArmOrientation: Qt.vector3d(0, -1, 0)

    Node {
        id: originNode
        PerspectiveCamera {
            id: cameraNode
            y: 110
            z: 450
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
        lockShadowmapTexels: true
        shadowMapQuality: Light.ShadowMapQualityVeryHigh
        shadowMapFar: 1000
    }

    Model {
        geometry: CylinderGeometry {
            length: 10
            radius: 250
            rings: 0
            segments: 50
        }

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
        position.y: 10

        shoulderYaw: v3d.shoulderYaw
        shoulderPitch: v3d.shoulderPitch
        elbowPitch: v3d.elbowPitch
        wristPitch: v3d.wristPitch

        showGhost: v3d.showGhost
        ghostShoulderYaw: v3d.ghostShoulderYaw
        ghostShoulderPitch: v3d.ghostShoulderPitch
        ghostElbowPitch: v3d.ghostElbowPitch
        ghostWristPitch: v3d.ghostWristPitch

        wallOpacity: v3d.showWalls ? 0.3 : 0
        leftWallOpenness: v3d.wallsOpen ? 1 : 0
        rightWallOpenness: v3d.wallsOpen ? 1 : 0

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
