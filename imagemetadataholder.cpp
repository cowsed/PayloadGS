#include "imagemetadataholder.h"

PhotoTransformQ::PhotoTransformQ(uint16_t left,
                                 uint16_t right,
                                 uint16_t top,
                                 uint16_t bottom,
                                 uint16_t encodedWidth,
                                 uint8_t encodedQuality)
    : left(left)
    , right(right)
    , top(top)
    , bottom(bottom)
    , encodedWidth(encodedWidth)
    , encodedQuality(encodedQuality)
{}

bool PhotoTransformQ::isValid()
{
    qDebug("Checking validity");
    return right > left && bottom < top && encodedQuality <= 7;
}

ImageMetadataHolder::ImageMetadataHolder() {}

ImageMetadataHolder::ImageMetadataHolder(const ImageMetadata &meta)
    : image_id(meta.image_id)
    , datetime(QDateTime::fromSecsSinceEpoch(meta.timestamp))
    , numBlocks(meta.num_blocks)
    , left(meta.transform.left)
    , right(meta.transform.right)
    , top(meta.transform.top)
    , bottom(meta.transform.bottom)
    , encodedWidth(meta.transform.output_width)
    , encodedHeight(((meta.transform.right - meta.transform.left) == 0)
                        ? 0
                        : meta.transform.output_width
                              * ((float) (meta.transform.top - meta.transform.bottom))
                              / ((float) (meta.transform.right - meta.transform.left)))
    , encodedQuality(meta.transform.quality)
    , latitude(meta.latitude)
    , longitude(meta.longitude)
    , j1(meta.location.shoulder_yaw)
    , j2(meta.location.shoulder_pitch)
    , j3(meta.location.elbow_pitch)
    , j4(meta.location.wrist_pitch)
{}


QJsonObject ImageMetadataHolder::toJson() const
{
    QJsonObject obj;
    obj["image_id"] = image_id;
    obj["datetime"] = datetime.toSecsSinceEpoch();
    obj["numBlocks"] = numBlocks;

    obj["left"] = left;
    obj["right"] = right;
    obj["top"] = top;
    obj["bottom"] = bottom;

    obj["encodedWidth"] = encodedWidth;
    obj["encodedHeight"] = encodedHeight;
    obj["encodedQuality"] = encodedQuality;

    obj["latitude"] = latitude;
    obj["longitude"] = longitude;

    obj["j1"] = j1;
    obj["j2"] = j2;
    obj["j3"] = j3;
    obj["j4"] = j4;
    return obj;
}

ImageMetadataHolder ImageMetadataHolder::fromJson(const QJsonObject &json)
{
    ImageMetadataHolder obj;

    if (const QJsonValue v = json["image_id"]; v.isDouble()) {
        obj.image_id = v.toDouble();
    }

    if (const QJsonValue v = json["datetime"]; v.isDouble()) {
        obj.datetime = QDateTime::fromSecsSinceEpoch(v.toDouble());
    }
    if (const QJsonValue v = json["numBlocks"]; v.isDouble()) {
        obj.numBlocks = v.toDouble();
    }
    if (const QJsonValue v = json["left"]; v.isDouble()) {
        obj.left = v.toDouble();
    }
    if (const QJsonValue v = json["right"]; v.isDouble()) {
        obj.right = v.toDouble();
    }

    if (const QJsonValue v = json["top"]; v.isDouble()) {
        obj.top = v.toDouble();
    }
    if (const QJsonValue v = json["bottom"]; v.isDouble()) {
        obj.bottom = v.toDouble();
    }

    if (const QJsonValue v = json["encodedWidth"]; v.isDouble()) {
        obj.encodedWidth = v.toDouble();
    }
    if (const QJsonValue v = json["encodedHeight"]; v.isDouble()) {
        obj.encodedHeight = v.toDouble();
    }
    if (const QJsonValue v = json["encodedQuality"]; v.isDouble()) {
        obj.encodedQuality = v.toDouble();
    }

    if (const QJsonValue v = json["latitude"]; v.isDouble()) {
        obj.latitude = v.toDouble();
    }
    if (const QJsonValue v = json["longitude"]; v.isDouble()) {
        obj.longitude = v.toDouble();
    }

    if (const QJsonValue v = json["j1"]; v.isDouble()) {
        obj.j1 = v.toDouble();
    }
    if (const QJsonValue v = json["j2"]; v.isDouble()) {
        obj.j2 = v.toDouble();
    }
    if (const QJsonValue v = json["j3"]; v.isDouble()) {
        obj.j3 = v.toDouble();
    }
    if (const QJsonValue v = json["j4"]; v.isDouble()) {
        obj.j4 = v.toDouble();
    }

    return obj;
}

bool ImageMetadataHolder::isValid()
{
    return datetime.toSecsSinceEpoch() != 0;
}

PhotoTransformQ ImageMetadataHolder::photoTransform()
{
    return PhotoTransformQ{
        left,
        right,
        top,
        bottom,
        encodedWidth,
        encodedQuality,
    };
}
