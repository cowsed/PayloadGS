import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    property url textureData: "assets/maps/textureData.png"
    Texture {
        id: _0_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    PrincipledMaterial {
        id: material_001_material
        objectName: "Material.001"
        baseColorMap: _0_texture
        roughness: 0.5
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Model {
        id: cube
        objectName: "Cube"
        source: "assets/meshes/cube_001_mesh.mesh"
        materials: [material_001_material]
        castsShadows: true
        receivesShadows: true
    }

    // Animations:
}
