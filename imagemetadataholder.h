#ifndef IMAGEMETADATAHOLDER_H
#define IMAGEMETADATAHOLDER_H

#include <QDateTime>
#include <QJsonObject>
#include <QQmlEngine>
#include "packets_p2g.h"

class ImageMetadataHolder
{
    Q_GADGET

    Q_PROPERTY(uint8_t image_id READ image_id FINAL CONSTANT)
    Q_PROPERTY(QDateTime datetime READ datetime FINAL CONSTANT)
    Q_PROPERTY(uint16_t numBlocks READ numBlocks FINAL CONSTANT)
    Q_PROPERTY(uint16_t left READ left FINAL CONSTANT)
    Q_PROPERTY(uint16_t right READ right FINAL CONSTANT)
    Q_PROPERTY(uint16_t top READ top FINAL CONSTANT)
    Q_PROPERTY(uint16_t bottom READ bottom FINAL CONSTANT)

    Q_PROPERTY(uint16_t encodedWidth READ encodedWidth FINAL CONSTANT)
    Q_PROPERTY(uint16_t encodedHeight READ encodedHeight FINAL CONSTANT)
    Q_PROPERTY(uint8_t encodedQuality READ encodedQuality FINAL CONSTANT)

    Q_PROPERTY(float latitude READ latitude FINAL CONSTANT)
    Q_PROPERTY(float longitude READ longitude FINAL CONSTANT)

    Q_PROPERTY(uint8_t j1 READ j1 FINAL CONSTANT)
    Q_PROPERTY(uint8_t j2 READ j2 FINAL CONSTANT)
    Q_PROPERTY(uint8_t j3 READ j3 FINAL CONSTANT)
    Q_PROPERTY(uint8_t j4 READ j4 FINAL CONSTANT)

    QML_ELEMENT

public:
    ImageMetadataHolder();
    explicit ImageMetadataHolder(const ImageMetadata &meta);

    uint8_t image_id() const;
    QDateTime datetime() const;
    uint16_t numBlocks() const;
    uint16_t left() const;
    uint16_t right() const;
    uint16_t top() const;
    uint16_t bottom() const;
    uint16_t encodedWidth() const;
    uint16_t encodedHeight() const;
    uint8_t encodedQuality() const;

    float latitude() const;
    float longitude() const;
    uint8_t j1() const;
    uint8_t j2() const;
    uint8_t j3() const;
    uint8_t j4() const;

    Q_INVOKABLE bool isValid();

    QJsonObject toJson() const;
    static ImageMetadataHolder fromJson(const QJsonObject &json);

private:
    uint8_t m_image_id = 0;
    QDateTime m_datetime = QDateTime::fromSecsSinceEpoch(0);
    uint16_t m_numBlocks = 0;
    uint16_t m_left = 0;
    uint16_t m_right = 0;
    uint16_t m_top = 0;
    uint16_t m_bottom = 0;
    uint16_t m_encodedWidth = 0;
    uint16_t m_encodedHeight = 0;
    uint8_t m_encodedQuality = 0;
    float m_latitude = 0;
    float m_longitude = 0;

    uint8_t m_j1 = 0;
    uint8_t m_j2 = 0;
    uint8_t m_j3 = 0;
    uint8_t m_j4 = 0;
};

Q_DECLARE_METATYPE(ImageMetadataHolder)
#endif // IMAGEMETADATAHOLDER_H
