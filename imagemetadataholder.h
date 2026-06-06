#ifndef IMAGEMETADATAHOLDER_H
#define IMAGEMETADATAHOLDER_H

#include <QDateTime>
#include <QJsonObject>
#include <QQmlEngine>
#include "cubesat_comms/packets_p2g.h"

class PhotoTransformQ
{
    Q_GADGET
    QML_VALUE_TYPE(photoTransform)
public:
    PhotoTransformQ() = default;
    PhotoTransformQ(uint16_t left,
                    uint16_t right,
                    uint16_t top,
                    uint16_t bottom,
                    uint16_t encodedWidth,
                    uint8_t encodedQuality);

    Q_PROPERTY(uint16_t left MEMBER left)
    Q_PROPERTY(uint16_t right MEMBER right)
    Q_PROPERTY(uint16_t top MEMBER top)
    Q_PROPERTY(uint16_t bottom MEMBER bottom)

    Q_PROPERTY(uint16_t encodedWidth MEMBER encodedWidth)
    Q_PROPERTY(uint8_t encodedQuality MEMBER encodedQuality)

    Q_INVOKABLE QString errors(uint16_t cam_width, uint16_t cam_height);

    uint16_t left = 0;
    uint16_t right = 0;
    uint16_t top = 0;
    uint16_t bottom = 0;
    uint16_t encodedWidth = 0;
    uint8_t encodedQuality = 0;
};

class ImageMetadataHolder
{
    Q_GADGET

    Q_PROPERTY(uint8_t image_id MEMBER image_id)
    Q_PROPERTY(QDateTime datetime MEMBER datetime)
    Q_PROPERTY(uint16_t numBlocks MEMBER numBlocks)
    Q_PROPERTY(uint16_t left MEMBER left)
    Q_PROPERTY(uint16_t right MEMBER right)
    Q_PROPERTY(uint16_t top MEMBER top)
    Q_PROPERTY(uint16_t bottom MEMBER bottom)

    Q_PROPERTY(uint16_t encodedWidth MEMBER encodedWidth)
    Q_PROPERTY(uint16_t encodedHeight MEMBER encodedHeight)
    Q_PROPERTY(uint8_t encodedQuality MEMBER encodedQuality)

    Q_PROPERTY(float latitude MEMBER latitude)
    Q_PROPERTY(float longitude MEMBER longitude)

    Q_PROPERTY(uint8_t j1 MEMBER j1)
    Q_PROPERTY(uint8_t j2 MEMBER j2)
    Q_PROPERTY(uint8_t j3 MEMBER j3)
    Q_PROPERTY(uint8_t j4 MEMBER j4)

    QML_VALUE_TYPE(imageMetadataHolder)

public:
    ImageMetadataHolder();
    explicit ImageMetadataHolder(const ImageMetadata &meta);


    Q_INVOKABLE bool isValid();
    Q_INVOKABLE PhotoTransformQ photoTransform();

    QJsonObject toJson() const;
    static ImageMetadataHolder fromJson(const QJsonObject &json);

public:
    uint8_t image_id = 0;
    QDateTime datetime = QDateTime::fromSecsSinceEpoch(0);
    uint16_t numBlocks = 0;
    uint16_t left = 0;
    uint16_t right = 0;
    uint16_t top = 0;
    uint16_t bottom = 0;
    uint16_t encodedWidth = 0;
    uint16_t encodedHeight = 0;
    uint8_t encodedQuality = 0;
    float latitude = 0;
    float longitude = 0;

    uint8_t j1 = 0;
    uint8_t j2 = 0;
    uint8_t j3 = 0;
    uint8_t j4 = 0;
};

#endif // IMAGEMETADATAHOLDER_H
