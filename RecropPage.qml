import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    property photoTransform oldCrop: ImageDataHolder.metadataForImageId(
                                         imageSelector.value).photoTransform()
    property photoTransform newCrop

    function setNewCrop() {
        root.newCrop.left = left.value
        root.newCrop.right = right.value
        root.newCrop.top = top.value
        root.newCrop.bottom = bottom.value
        root.newCrop.encodedWidth = encodedWidth.value
        root.newCrop.encodedQuality = encodedWidth.value

        const w = (root.newCrop.right - root.newCrop.left)
        const h = (root.newCrop.top - root.newCrop.bottom)

        width.text = "Width: " + w.toFixed(0)
        height.text = "Height: " + h.toFixed(0)

        encodedHeight.text = "encodedHeight: " + (root.newCrop.encodedWidth * h / w).toFixed(
                    0)

        cropVis.crop = newCrop
    }

    Component.onCompleted: {
        setNewCrop()
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
            }
        }
        ColumnLayout {
            RowLayout {
                SpinBox {
                    id: left
                    from: 0
                    to: cropVis.original_width
                    value: 128
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
                    to: cropVis.original_width
                    value: 256
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
                    to: cropVis.original_height
                    value: 256
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
                    to: cropVis.original_height
                    onValueModified: root.setNewCrop()
                    value: 128
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
                    to: 8192
                    value: 128
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
                id: width
                text: "Width: 0"
            }
            Label {
                id: height
                text: "Height: 0"
            }
            Label {
                id: encodedHeight
                text: "Encoded Height: "
                      + (root.newCrop.encodedWidth * (root.newCrop.top - root.newCrop.bottom)
                         / (root.newCrop.right - root.newCrop.left)).toFixed(0)
            }

            Button {
                text: "Enhance"
            }
        }
    }
}
