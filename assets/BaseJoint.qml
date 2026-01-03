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
    }

    // Nodes:
    Model {
        id: cylinder
        objectName: "Cylinder"
        position: Qt.vector3d(-0.109612, 0.015169, 0)
        source: "meshes/cylinder_mesh.mesh"
        materials: [
            principledMaterial
        ]
    }

    // Animations:
}
