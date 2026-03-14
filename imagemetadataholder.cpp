#include "imagemetadataholder.h"

ImageMetadataHolder::ImageMetadataHolder() {}

ImageMetadataHolder::ImageMetadataHolder(const ImageMetadata &meta)
    : m_image_id(meta.image_id)
    , m_datetime(QDateTime::fromSecsSinceEpoch(meta.timestamp))
    , m_numBlocks(meta.num_blocks)
    , m_left(meta.transform.left)
    , m_right(meta.transform.right)
    , m_top(meta.transform.top)
    , m_bottom(meta.transform.bottom)
    , m_encodedWidth(meta.transform.output_width)
    , m_encodedHeight(((meta.transform.right - meta.transform.left) == 0)
                          ? 0
                          : meta.transform.output_width
                                * ((float) (meta.transform.top - meta.transform.bottom))
                                / ((float) (meta.transform.right - meta.transform.left)))
    , m_encodedQuality(meta.transform.quality)
    , m_latitude(meta.latitude)
    , m_longitude(meta.longitude)
    , m_j1(meta.location.shoulder_yaw)
    , m_j2(meta.location.shoulder_pitch)
    , m_j3(meta.location.elbow_pitch)
    , m_j4(meta.location.wrist_pitch)
{}

uint8_t ImageMetadataHolder::image_id() const
{
    return m_image_id;
}

QDateTime ImageMetadataHolder::datetime() const
{
    return m_datetime;
}

uint16_t ImageMetadataHolder::numBlocks() const
{
    return m_numBlocks;
}

uint16_t ImageMetadataHolder::left() const
{
    return m_left;
}

uint16_t ImageMetadataHolder::right() const
{
    return m_right;
}

uint16_t ImageMetadataHolder::top() const
{
    return m_top;
}

uint16_t ImageMetadataHolder::bottom() const
{
    return m_bottom;
}

uint16_t ImageMetadataHolder::encodedWidth() const
{
    return m_encodedWidth;
}

uint16_t ImageMetadataHolder::encodedHeight() const
{
    return m_encodedHeight;
}

uint8_t ImageMetadataHolder::encodedQuality() const
{
    return m_encodedQuality;
}

float ImageMetadataHolder::latitude() const
{
    return m_latitude;
}

float ImageMetadataHolder::longitude() const
{
    return m_longitude;
}

uint8_t ImageMetadataHolder::j1() const
{
    return m_j1;
}
uint8_t ImageMetadataHolder::j2() const
{
    return m_j2;
}
uint8_t ImageMetadataHolder::j3() const
{
    return m_j3;
}
uint8_t ImageMetadataHolder::j4() const
{
    return m_j4;
}

QJsonObject ImageMetadataHolder::toJson() const
{
    QJsonObject obj;
    obj["image_id"] = m_image_id;
    obj["datetime"] = m_datetime.toSecsSinceEpoch();
    obj["numBlocks"] = m_numBlocks;

    obj["left"] = m_left;
    obj["right"] = m_right;
    obj["top"] = m_top;
    obj["bottom"] = m_bottom;

    obj["encodedWidth"] = m_encodedWidth;
    obj["encodedHeight"] = m_encodedHeight;
    obj["encodedQuality"] = m_encodedQuality;

    obj["latitude"] = m_latitude;
    obj["longitude"] = m_longitude;

    obj["j1"] = m_j1;
    obj["j2"] = m_j2;
    obj["j3"] = m_j3;
    obj["j4"] = m_j4;
    return obj;
}

ImageMetadataHolder ImageMetadataHolder::fromJson(const QJsonObject &json)
{
    ImageMetadataHolder obj;

    if (const QJsonValue v = json["image_id"]; v.isDouble()) {
        obj.m_image_id = v.toDouble();
    }

    if (const QJsonValue v = json["datetime"]; v.isDouble()) {
        obj.m_datetime = QDateTime::fromSecsSinceEpoch(v.toDouble());
    }
    if (const QJsonValue v = json["numBlocks"]; v.isDouble()) {
        obj.m_numBlocks = v.toDouble();
    }
    if (const QJsonValue v = json["left"]; v.isDouble()) {
        obj.m_left = v.toDouble();
    }
    if (const QJsonValue v = json["right"]; v.isDouble()) {
        obj.m_right = v.toDouble();
    }

    if (const QJsonValue v = json["top"]; v.isDouble()) {
        obj.m_top = v.toDouble();
    }
    if (const QJsonValue v = json["bottom"]; v.isDouble()) {
        obj.m_bottom = v.toDouble();
    }

    if (const QJsonValue v = json["encodedWidth"]; v.isDouble()) {
        obj.m_encodedWidth = v.toDouble();
    }
    if (const QJsonValue v = json["encodedHeight"]; v.isDouble()) {
        obj.m_encodedHeight = v.toDouble();
    }
    if (const QJsonValue v = json["encodedQuality"]; v.isDouble()) {
        obj.m_encodedQuality = v.toDouble();
    }

    if (const QJsonValue v = json["latitude"]; v.isDouble()) {
        obj.m_latitude = v.toDouble();
    }
    if (const QJsonValue v = json["longitude"]; v.isDouble()) {
        obj.m_longitude = v.toDouble();
    }

    if (const QJsonValue v = json["j1"]; v.isDouble()) {
        obj.m_j1 = v.toDouble();
    }
    if (const QJsonValue v = json["j2"]; v.isDouble()) {
        obj.m_j2 = v.toDouble();
    }
    if (const QJsonValue v = json["j3"]; v.isDouble()) {
        obj.m_j3 = v.toDouble();
    }
    if (const QJsonValue v = json["j4"]; v.isDouble()) {
        obj.m_j4 = v.toDouble();
    }

    return obj;
}

bool ImageMetadataHolder::isValid()
{
    return m_datetime.toSecsSinceEpoch() != 0;
}
