import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    property url textureData: "maps/textureData.png"
    property url textureData13: "maps/textureData13.png"
    Texture {
        id: _1_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    Texture {
        id: _0_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData13
    }
    PrincipledMaterial {
        id: material_001_material
        objectName: "Material.001"
        baseColorMap: _1_texture
        metalness: 1
        roughness: 0.5
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
        id: material_material
        objectName: "Material"
        baseColorMap: _0_texture
        metalness: 0.10400000214576721
        roughness: 0.7480000257492065
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: material_003_material
        objectName: "Material.003"
        roughness: 0.9160000085830688
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }
    PrincipledMaterial {
        id: material_004_material
        objectName: "Material.004"
        baseColor: "#ffe74a00"
        metalness: 0.6240000128746033
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Model {
        id: frame
        objectName: "Frame"
        source: "meshes/frameMesh_mesh.mesh"
        materials: [
            material_001_material
        ]
        Model {
            id: baseJoint_001
            objectName: "BaseJoint.001"
            position: Qt.vector3d(-0.109704, 0.0224863, 0.0214806)
            source: "meshes/cylinder_006_mesh.mesh"
            materials: [
                principledMaterial
            ]
            Model {
                id: lower_link
                objectName: "lower link"
                position: Qt.vector3d(0, 0, 0.000311086)
                scale: Qt.vector3d(1, 1.00001, 1.28571)
                source: "meshes/cylinder_mesh.mesh"
                materials: [
                    material_material
                ]
                Model {
                    id: upper_link
                    objectName: "upper link"
                    position: Qt.vector3d(0.135, 0, -0.034217)
                    rotation: Qt.quaternion(1.62921e-07, 0, 0, 1)
                    scale: Qt.vector3d(1, 0.99999, 0.777778)
                    source: "meshes/cylinder_004_mesh.mesh"
                    materials: [
                        material_003_material
                    ]
                    Model {
                        id: wrist
                        objectName: "wrist"
                        position: Qt.vector3d(0.135, 0, 0)
                        rotation: Qt.quaternion(0.707107, 0, 0, 0.707107)
                        scale: Qt.vector3d(0.0210409, 0.0210409, 0.0210409)
                        source: "meshes/suzanne_mesh.mesh"
                        materials: [
                            material_004_material
                        ]
                    }
                }
            }
        }
    }

    // Animations:
}
