import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Node {
    id: node

    required property quaternion orientation
    required property vector3d base_imu

    property real shoulderYaw: 0
    property real shoulderPitch: 0
    property real elbowPitch: 0
    property real wristPitch: 0

    property real rightWallOpenness: 0
    property real leftWallOpenness: 0
    property real wallOpacity: .2
    property bool showReal: true
    property bool showGhost: false
    property bool showAxes: false

    property real ghostShoulderYaw: 0
    property real ghostShoulderPitch: 0
    property real ghostElbowPitch: 0
    property real ghostWristPitch: 0

    // Resources
    property url textureData: "maps/textureData.png"
    property url textureData13: "maps/textureData13.png"
    property url textureData18: "maps/textureData18.png"

    Texture {
        id: _2_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    Texture {
        id: _1_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData13
    }
    Texture {
        id: _0_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData18
    }
    PrincipledMaterial {
        id: wallMaterial_material
        objectName: "wallMaterial"
        baseColor: "#ff160066"
        metalness: 0.1599999964237213
        roughness: 1
        opacity: node.wallOpacity
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Blend
    }
    PrincipledMaterial {
        id: material_004_material
        objectName: "Material.004"
        baseColor: "#ffe74a00"
        metalness: 0.6240000128746033
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: material_003_material
        objectName: "Material.003"
        baseColorMap: _0_texture
        roughness: 0.9160000085830688
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: material_material
        objectName: "Material"
        baseColorMap: _1_texture
        metalness: 0.10400000214576721
        roughness: 0.7480000257492065
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: principledMaterial
        metalness: 1
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: material_001_material
        objectName: "Material.001"
        baseColorMap: _2_texture
        metalness: 1
        roughness: 0.5
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    GhostMaterial {
        id: ghostMat
    }

    function calcBounds(q) {
        let mx = frame.bounds.maximum
        let mn = frame.bounds.minimum

        let points = [mn, Qt.vector3d(
                          mn.x, mn.y,
                          mx.z), Qt.vector3d(mn.x, mx.y, mn.z), Qt.vector3d(
                          mn.x, mx.y,
                          mx.z), Qt.vector3d(mx.x, mn.y, mn.z), Qt.vector3d(
                          mx.x, mn.y, mx.z), Qt.vector3d(mn.x, mx.y, mn.z), mx]
        let transformedPoints = points.map(p => q.times(p))

        let minY = transformedPoints[0].y
        let maxY = transformedPoints[0].y
        for (const p of transformedPoints) {
            if (p.y < minY) {
                minY = p.y
            }
            if (p.y > maxY) {
                maxY = p.y
            }
        }

        return minY
    }

    // Nodes:
    Model {
        id: frame
        objectName: "Frame"
        source: "meshes/frameMesh_mesh.mesh"
        materials: [material_001_material]
        visible: node.showReal

        OrientationArrow {
            id: baseIMUArrow
            imu_reading: node.base_imu

            isVisible: node.showAxes
        }

        Model {
            id: baseJoint
            objectName: "BaseJoint"
            position: Qt.vector3d(-0.109704, 0.0224863, 0.0214806)
            source: "meshes/baseJointMesh_mesh.mesh"
            materials: [principledMaterial]
            eulerRotation.y: node.shoulderYaw

            Model {
                id: lower_link
                objectName: "lower link"
                position: Qt.vector3d(0, 0, 0.000311086)
                source: "meshes/lowrLinkMesh_mesh.mesh"
                materials: [material_material]

                eulerRotation.z: node.shoulderPitch + 90

                // OrientationArrow {
                // id: link1IMU
                // orientation: Qt.quaternion(1, 0, 0, 0)
                // isVisible: node.showAxes
                // col: Qt.rgba(0, 0, 1, 1)
                // position.x: 0.07
                // }
                Model {
                    id: upper_link
                    objectName: "upper link"
                    position: Qt.vector3d(0.135, 0, -0.0439932)
                    source: "meshes/upperLinkMesh_mesh.mesh"
                    materials: [material_003_material]
                    eulerRotation.z: node.elbowPitch + 180

                    Model {
                        id: wrist
                        objectName: "wrist"
                        position: Qt.vector3d(-0.135, 0, 0)
                        // rotation: Qt.quaternion(0.707107, 0, 0, -0.707106)
                        scale: Qt.vector3d(1, 1, 1)
                        source: "meshes/wristMesh_mesh.mesh"
                        materials: [material_004_material]
                        eulerRotation.z: node.wristPitch
                    }
                }
            }
        }
        Model {
            visible: node.showGhost
            id: baseJointGhost
            objectName: "BaseJointGhost"
            position: Qt.vector3d(-0.109704, 0.0224863, 0.0214806)
            source: "meshes/baseJointMesh_mesh.mesh"
            materials: []
            eulerRotation.y: node.ghostShoulderYaw
            Model {
                id: lower_linkGhost
                objectName: "lower linkGhost"
                position: Qt.vector3d(0, 0, 0.000311086)
                source: "meshes/lowrLinkMesh_mesh.mesh"
                materials: [ghostMat]
                eulerRotation.z: node.ghostShoulderPitch + 90

                Model {
                    id: upper_linkGhost
                    objectName: "upper linkGhost"
                    position: Qt.vector3d(0.135, 0, -0.0439932)
                    source: "meshes/upperLinkMesh_mesh.mesh"
                    materials: [ghostMat]
                    eulerRotation.z: node.ghostElbowPitch + 180

                    Model {
                        id: wristGhost
                        objectName: "wristGhost"
                        position: Qt.vector3d(-0.135, 0, 0)
                        // rotation: Qt.quaternion(0.707107, 0, 0, -0.707106)
                        scale: Qt.vector3d(1, 1, 1)
                        source: "meshes/wristMesh_mesh.mesh"
                        materials: [ghostMat]
                        eulerRotation.z: node.ghostWristPitch
                    }
                }
            }
        }

        Model {
            id: leftWall
            objectName: "LeftWall"
            position: Qt.vector3d(-0.026887, 0.00369129, -0.0449786)
            eulerRotation.x: -90 * node.leftWallOpenness
            source: "meshes/leftWall_mesh.mesh"
            materials: [wallMaterial_material]
        }
        Model {
            id: rightWall
            objectName: "RightWall"
            position: Qt.vector3d(-0.026887, 0.00369129, 0.0467295)
            rotation: Qt.quaternion(-1.62921e-07, 0, 1, 0)
            eulerRotation.x: -90 * node.rightWallOpenness

            source: "meshes/rightWall_mesh.mesh"
            materials: [wallMaterial_material]
        }
    }

    // Animations:
}
