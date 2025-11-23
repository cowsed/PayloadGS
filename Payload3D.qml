import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import Qt3D.Render
import QtQuick.Scene3D
import Qt3D.Extras
import Qt3D.Input
import Qt3D.Core

// import "./assets"
Item {

    View3D {

        id: v3d
        anchors.fill: parent

        camera: camera

        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture {
                source: "assets/maps/goegap_2k.hdr"
            }
        }

        PerspectiveCamera {
            id: camera
            fieldOfView: 45
            position: Qt.vector3d(0, 100, 900)
        }
        // SOrbitController {
        // camera: camera
        // }
        WasdController {
            controlledObject: camera
        }
        Model {
            scale: Qt.vector3d(500, 500, 500)
            geometry: GridGeometry {
                horizontalLines: 20
                verticalLines: 20
            }
            eulerRotation.x: -90
            materials: [
                DefaultMaterial {}
            ]
        }

        // Start with a simple material, using the built-in implementation for everything.
        Payload {
            scale: Qt.vector3d(1000, 1000, 1000)
        }
    }
}
