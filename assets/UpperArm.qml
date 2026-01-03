import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    PrincipledMaterial {
        id: principledMaterial
        metalness: 1
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
        cullMode: PrincipledMaterial.NoCulling
    }

    // Nodes:
    Model {
        id: cube_001
        objectName: "Cube.001"
        position: Qt.vector3d(-0.109769, 0.0154177, 0)
        source: "meshes/cube_mesh.mesh"
        materials: [principledMaterial]
    }

    // Animations:
}
