#include "ssdvdecoder.h"
#include <QProcess>
#include "imagedataholder.h"

SSDVDecoder::SSDVDecoder(QString script_dir,
                         QString flight_dir,
                         uint8_t block_size,
                         uint8_t image_id)
    : script_dir(script_dir)
    , flight_dir(flight_dir)
    , block_size(block_size)
    , image_id(image_id)
{}

void SSDVDecoder::run()
{
    qDebug("Running SSDV process for image %d", image_id);
    QProcess proc{};
    proc.setWorkingDirectory(script_dir);

    QStringList args = {DECODE_SCRIPT,
                        flight_dir + "/Images/" + ImageDataHolder::imageName(image_id),
                        QString::number(block_size)};

    qDebug("Starting SSDV process for image %d: %s", image_id, qPrintable(args.join(", ")));
    proc.start("bash", args);

    if (proc.waitForFinished()) {
        qDebug("finished SSDV process for image %d", image_id);

        int exit = proc.exitCode();
        emit conversionFinished(image_id, exit);
    } else {
        qDebug("SSDV encoder wait returned false: %d - %s",
               proc.exitCode(),
               qPrintable(proc.errorString()));
    }
}
