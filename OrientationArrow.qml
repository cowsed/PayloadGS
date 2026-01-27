import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Node {
    id: arrow

    required property bool isVisible
    required property vector3d imu_reading

    property real arrowLen: 0.04
    property real arrowRadius: 0.003
    property color col: Qt.rgba(1, 0, 0, 1)

    property vector3d gravity_vector: Qt.vector3d(0, -1, 0)

    function quatFromIMUVector() {
        // https://github.com/toji/gl-matrix/blob/f0583ef53e94bc7e78b78c8a24f09ed5e2f7a20c/src/gl-matrix/quat.js#L54
        // https://stackoverflow.com/questions/18558910/direction-vector-to-rotation-matrix
        let me = arrow.gravity_vector.normalized()
        let ref = arrow.imu_reading.normalized()
        let a = ref.crossProduct(me)
        let d = ref.dotProduct(me)

        let s = ref.length() * me.length() + d

        let q = Qt.quaternion(s, a.x, a.y, a.z).normalized()

        return q
    }

    property quaternion backToRealSpace: sceneRotation.conjugated()
    property quaternion fromVector: quatFromIMUVector()

    Node {
        rotation: arrow.backToRealSpace // .times(fromVector)
        visible: arrow.isVisible

        CustomMaterial {
            id: arrowMat
            // These properties are automatically exposed to the shaders
            property real alpha: 1.0
            property color arrowColor: arrow.col

            shadingMode: CustomMaterial.Unshaded
            sourceBlend: alpha < 1.0 ? CustomMaterial.SrcAlpha : CustomMaterial.NoBlend
            destinationBlend: alpha < 1.0 ? CustomMaterial.OneMinusSrcAlpha : CustomMaterial.NoBlend
            cullMode: CustomMaterial.BackFaceCulling

            vertexShader: "qrc:/assets/arrowshader.vert"
            fragmentShader: "qrc:/assets/arrowshader.frag"
        }

        Model {
            id: arrowShaft
            geometry: CylinderGeometry {
                id: arrowShaftGeom
                length: arrow.arrowLen
                radius: arrow.arrowRadius
                rings: 0
                segments: 10
            }
            position.y: -arrow.arrowLen / 2
            materials: arrowMat
            castsShadows: true
            receivesShadows: true
        }
        Model {
            id: arrowPoint
            geometry: ConeGeometry {
                id: arrowPointGeom
                length: arrow.arrowLen / 3
                bottomRadius: arrow.arrowRadius * 2.5
                rings: 0
                segments: 10
            }
            eulerRotation.z: 180
            position.y: -arrow.arrowLen
            materials: arrowMat
        }
    }
}
