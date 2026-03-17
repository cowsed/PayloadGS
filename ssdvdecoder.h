#ifndef SSDVDECODER_H
#define SSDVDECODER_H

#include <QObject>
#include <qrunnable.h>

class SSDVDecoder : public QObject, public QRunnable
{
    Q_OBJECT
public:
    static constexpr char DECODE_SCRIPT[] = "./ssdv_dec.sh";
    explicit SSDVDecoder(QString script_dir,
                         QString flight_dir,
                         uint8_t block_size,
                         uint8_t image_id);

    void run();
signals:
    void conversionFinsihed(uint8_t image_id, int result);

private:
    QString script_dir;
    QString flight_dir;
    uint8_t block_size;
    uint8_t image_id;
};

#endif // SSDVDECODER_H
