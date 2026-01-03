import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick.Scene3D

// import "./assets"
View3D {

    id: v3d
    camera: cameraNode
    Node {
        id: originNode
        PerspectiveCamera {
            id: cameraNode
            y: 110
            z: 450
        }
    }
    OrbitCameraController {
        anchors.fill: parent
        origin: originNode
        camera: cameraNode
    }

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lightProbe: Texture {
            source: "assets/maps/goegap_2k.hdr"
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

    // SOrbitController {
    // camera: camera
    // }


    /*WasdController {
        controlledObject: camera
    }*/
    Model {

        // position.y: payload.cube.bound.minimum.y - 10
        geometry: CylinderGeometry {
            length: 10
            radius: 250
            rings: 0
            segments: 50
        }
        // eulerRotation.x: -90
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

        BaseJoint {
            id: baseJoint
        }

        UpperArm {
            id: upperarm
            // scale: Qt.vector3d(1000, 1000, 1000)
            // eulerRotation.z: 40
            ForeArm {
                id: forearm
                // eulerRotation.z: 40
                // scale: Qt.vector3d(1000, 1000, 1000)
                Wrist {
                    id: wrist
                    // scale: Qt.vector3d(1000, 1000, 1000)
                }
            }
        }
    }
}
