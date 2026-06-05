import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    property int cam_width: 1280
    property int cam_height: 800

    property photoTransform oldCrop: ImageDataHolder.metadataForImageId(imageSelector.value).photoTransform()
    property photoTransform newCrop
    function setNewCrop() {
        root.newCrop.left = left.value;
        root.newCrop.right = right.value;
        root.newCrop.top = top.value;
        root.newCrop.bottom = bottom.value;
        root.newCrop.encodedWidth = encodedWidth.value;
        root.newCrop.encodedQuality = encodedWidth.value;

        const w = (root.newCrop.right - root.newCrop.left);
        const h = (root.newCrop.bottom - root.newCrop.top);

        dimensions.text = "Dimensions: " + w.toFixed(0) + "x" + h.toFixed(0);

        const encodedHeight = (root.newCrop.encodedWidth * h / w).toFixed(0);
        encodedDimensions.text = "Encoded Dims: " + encodedWidth.value + "x" + encodedHeight;

        cropVis.crop = newCrop;
    }

    Component.onCompleted: {
        setNewCrop();
    }

    // if no images taken, say that
    // if some images taken, let pick from them or else just use the one thats selected below
    RowLayout {
        ColumnLayout {
            RowLayout {
                SpinBox {
                    id: imageSelector
                    from: 0
                    to: ImageDataHolder.numImages - 1
                    value: 0
                    editable: true
                }
                Label {
                    text: "Image ID"
                }
            }

            CropVisualizer {
                id: cropVis
                oldCrop: root.oldCrop
                crop: root.newCrop
                oldImageId: imageSelector.value
                original_width: root.cam_width
                original_height: root.cam_height
            }
        }
        ColumnLayout {
            RowLayout {
                SpinBox {
                    id: left
                    from: 0
                    to: root.cam_width
                    value: 0
                    stepSize: 16
                    editable: true
                    onValueModified: root.setNewCrop()
                }

                Label {
                    text: "Left"
                }
            }
            RowLayout {
                SpinBox {
                    id: right
                    from: 0
                    to: root.cam_width
                    value: root.cam_width
                    stepSize: 16
                    editable: true
                    onValueModified: root.setNewCrop()
                }
                Label {
                    text: "Right"
                }
            }
            RowLayout {
                SpinBox {
                    id: top
                    from: 0
                    to: root.cam_height
                    value: 0
                    stepSize: 16
                    editable: true
                    onValueModified: root.setNewCrop()
                }
                Label {
                    text: "Top"
                }
            }
            RowLayout {
                SpinBox {
                    id: bottom
                    from: 0
                    to: root.cam_height
                    onValueModified: root.setNewCrop()
                    value: root.cam_height
                    stepSize: 16
                    editable: true
                }
                Label {
                    text: "Bottom"
                }
            }

            RowLayout {
                SpinBox {
                    id: encodedWidth
                    from: 0
                    to: (right.value - left.value)
                    value: (right.value - left.value)
                    editable: true
                    stepSize: 16
                    onValueModified: root.setNewCrop()
                }
                Label {
                    text: "Encoded Width"
                }
            }

            RowLayout {
                SpinBox {
                    id: encodedQuality
                    from: 0
                    to: 7
                    value: 4
                    editable: true
                    onValueModified: root.setNewCrop()
                }
                Label {
                    text: "Quality"
                }
            }

            // imageMetadataHolder {
            //     id: holder
            // }
        }
        ColumnLayout {
            Label {
                id: dimensions
                text: "Dimensions: 0x0"
            }
            Label {
                id: encodedDimensions
                text: "Encoded Dims: 0x0"
            }

            Button {
                text: "Enhance"
            }
            Button {
                text: "Still Picture"
                onClicked: function () {
                    const xform = root.newCrop;
                    const errors = xform.errors(root.cam_width, root.cam_height);
                    if (errors == "") {
                        console.log("Stil pcutre transform", xform);
                        RadioPacketParser.takeStillPicture(xform);
                    } else {
                        console.log("Invalid settings", errors);
                    }
                }
            }
        }
    }
}
