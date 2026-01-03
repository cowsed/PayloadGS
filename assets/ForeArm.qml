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
        id: cube_002
        objectName: "Cube.002"
        position: Qt.vector3d(-0.10964, 0.130935, 0)
        source: "meshes/cube_002_mesh.mesh"
        materials: [principledMaterial]
    }

    // Animations:
}
