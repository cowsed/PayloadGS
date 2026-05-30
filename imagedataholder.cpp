#include "imagedataholder.h"
#include <QDir>
#include "QThreadPool"
#include "ssdvdecoder.h"

ImageDataHolder::ImageDataHolder(QObject *parent)
    : QObject{parent}
{}
uint8_t ImageDataHolder::numImages() const
{
    return m_count;
}

ImageMetadataHolder ImageDataHolder::metadataForImageId(uint8_t image_id) const
{
    QString path = QString("%1/Images/%2/meta.json").arg(m_flight_dir, imageName(image_id));
    if (!QFile::exists(path)) {
        return {}; // invalid default construction
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open metadata file '" << path
                 << "' for reading:" << file.errorString();
        return {};
    }

    QByteArray jsonBytes = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument obj = QJsonDocument::fromJson(jsonBytes, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        return {};
    }

    if (!obj.isObject()) {
        return {};
    }
    return ImageMetadataHolder::fromJson(obj.object());
}
QString ImageDataHolder::pathForImage(uint8_t image_id) const
{
    return QString("%1/Images/%2/image.jpg").arg(m_flight_dir, imageName(image_id));
}
QString ImageDataHolder::pathForImageThumbnail(uint8_t image_id) const
{
    return QString("%1/Images/%2/thumbnail.jpg").arg(m_flight_dir, imageName(image_id));
}
double ImageDataHolder::transmissionPercent(uint8_t image_id) const
{
    ImageMetadataHolder meta = metadataForImageId(image_id);
    if (meta.numBlocks == 0) {
        return 0;
    }
    return (float) numDownloadedPackets(image_id) / (float) meta.numBlocks;
}

bool ImageDataHolder::imageComplete(uint8_t image_id)
{
    ImageMetadataHolder meta = metadataForImageId(image_id);
    return numDownloadedPackets(image_id) == meta.numBlocks;
}

void ImageDataHolder::setFlightDir(QString dir)
{
    m_flight_dir = dir;
}

QString ImageDataHolder::imagePacketDirectory(uint8_t image_id) const
{
    return QString("%1/Images/%2/packets").arg(m_flight_dir, imageName(image_id));
}

ImageDataHolder::DownloadProgress ImageDataHolder::downloadedPackets(uint8_t image_id) const
{
    DownloadProgress progress;
    QDir d{imagePacketDirectory(image_id)};

    QStringList entries = d.entryList(QDir::Files | QDir::NoSymLinks);
    for (const QString &entry : std::as_const(entries)) {
        if (!entry.startsWith(PACKET_PREFIX)) {
            qInfo("Bad file in packet directory: %s", qPrintable(entry));
            continue;
        }
        QString str_id = entry.mid(sizeof(PACKET_PREFIX));
        bool ok = false;
        uint32_t packet_id = str_id.toInt(&ok);
        if (!ok) {
            qInfo("Unparseable file in packet directory: %s", qPrintable(entry));
            continue;
        }
        progress.downloaded_packets.insert(packet_id);
    }
    return progress;
}

quint32 ImageDataHolder::numDownloadedPackets(uint8_t image_id) const
{
    DownloadProgress prog = downloadedPackets(image_id);
    return prog.downloaded_packets.count();
}

void ImageDataHolder::rescanCount()
{
    QString path = QString("%1/Images").arg(m_flight_dir);
    qDebug("Scanning %s for images", qPrintable(path));

    QDir dir{path};
    if (!dir.exists()) {
        qDebug("Missing image directory?? bad setup");
        setNumImages(0);
    }

    uint8_t maximum_entry = 0;
    bool foundAny = false;
    QStringList entries = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    for (const QString &entry : std::as_const(entries)) {
        if (!entry.startsWith(IMAGE_PREFIX)) {
            qDebug("Skipping %s since it does not have prefix", qPrintable(entry));
            continue;
        }
        QString str_id = entry.mid(sizeof(IMAGE_PREFIX));
        bool ok = false;
        uint32_t image_id = str_id.toInt(&ok);

        if (!ok) {
            qDebug("Unparseable file in image directory: %s", qPrintable(entry));
            continue;
        }

        foundAny = true;
        if (image_id > maximum_entry) {
            maximum_entry = image_id;
        }
    }
    if (!foundAny) {
        qDebug("Didn't find any images");
        setNumImages(0);
        return;
    }
    setNumImages(maximum_entry + 1);
    qDebug("Found %zu images in scan", m_count);
}

QString ImageDataHolder::imageName(uint8_t image_id)
{
    return QString("%1%2").arg(IMAGE_PREFIX).arg(image_id, 3, 10, QLatin1Char('0'));
}

QString ImageDataHolder::packetName(uint16_t block_id)
{
    return QString("%1%2").arg(PACKET_PREFIX).arg(block_id, 5, 10, QLatin1Char('0'));
}

void ImageDataHolder::setNumImages(size_t num)
{
    size_t old = m_count;
    qDebug("Setting to %zu", num);
    m_count = num;
    if (old != num) {
        emit numImagesChanged();
    }
}

void ImageDataHolder::ImageDataReceived(QDateTime time, const ImageData &buf)
{
    QString fname = imagePacketDirectory(buf.image_id) + "/" + packetName(buf.block_index);
    QFile file{fname};
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open packet for writing going to have a bad time:"
                 << file.errorString();
        return;
    }
    file.write((char *) &buf.buf[0], IMAGE_DATA_SIZE);
    file.close();

    SSDVDecoder *dec = new SSDVDecoder{m_flight_dir + "/..",
                                       m_flight_dir,
                                       IMAGE_DATA_SIZE,
                                       buf.image_id};
    dec->setAutoDelete(true);
    connect(dec, &::SSDVDecoder::conversionFinsihed, this, &ImageDataHolder::SSDVDecodeFinished);
    qDebug("Starting ssdv decoder");
    QThreadPool::globalInstance()->start(dec);
}

void ImageDataHolder::SSDVDecodeFinished(uint8_t image_id, int exit_code)
{
    qDebug("Finished decoding image %d with exit code %d", image_id, exit_code);
    emit imageUpdated(image_id);
}
