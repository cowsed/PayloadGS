import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    property url textureData: "maps/armCameraTextureData.png"
    Texture {
        id: _0_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    PrincipledMaterial {
        id: material_002_material
        objectName: "Material.002"
        baseColorMap: _0_texture
        roughness: 0.5
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Model {
        id: cube_003
        objectName: "Cube.003"
        position: Qt.vector3d(-0.107605, 0.216787, 0)
        source: "meshes/cube_003_mesh.mesh"
        materials: [material_002_material]
    }

    // Animations:
}
