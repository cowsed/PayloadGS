#ifndef IMAGEDATAHOLDER_H
#define IMAGEDATAHOLDER_H

#include <QObject>
#include <QQmlEngine>
#include <imagemetadataholder.h>

class ImageDataHolder : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint8_t numImages READ numImages NOTIFY numImagesChanged FINAL)
    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("tied to C++ data source")

public:
    static constexpr const char PACKET_PREFIX[] = "pkt_";
    static constexpr const char IMAGE_PREFIX[] = "img_";
    struct DownloadProgress
    {
        QSet<uint32_t> downloaded_packets;
    };
    explicit ImageDataHolder(QObject *parent = nullptr);

    Q_INVOKABLE uint8_t numImages() const;

    // don't forget to check isValid
    Q_INVOKABLE ImageMetadataHolder metadataForImageId(uint8_t image_id) const;
    Q_INVOKABLE QString pathForImage(uint8_t image_id) const;
    Q_INVOKABLE QString pathForImageThumbnail(uint8_t image_id) const;
    Q_INVOKABLE double transmissionPercent(uint8_t image_id) const;

    void setFlightDir(QString dir);

    QString imagePacketDirectory(uint8_t image_id) const;
    DownloadProgress downloadedPackets(uint8_t image_id) const;
    Q_INVOKABLE quint32 numDownloadedPackets(uint8_t image_id) const;

    static QString imageName(uint8_t image_id);
    static QString packetName(uint16_t block_id);

    Q_INVOKABLE void rescanCount();

signals:
    void numImagesChanged();
    void imageUpdated(uint8_t image_id);
    void gotNewMetadata(uint8_t image_id);

public slots:
    void ImageDataReceived(uint8_t image_id, uint16_t block_id, const QByteArray &buf);

    // got metadata oacket
    // got image packet
private:
    void setNumImages(size_t num);
    size_t m_count = 0;
    QString m_flight_dir;
};

#endif // IMAGEDATAHOLDER_H
