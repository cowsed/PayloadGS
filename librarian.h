#ifndef LIBRARIAN_H
#define LIBRARIAN_H
#include <QObject>
#include "common.h"
#include "imagedataholder.h"
#include "packets_p2g.h"

#include <optional>
#include <qlist.h>
#include <set>

class Librarian : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint64_t numRequests READ NumRequests NOTIFY NumRequestsChanged)
    Q_PROPERTY(QVariantList summary READ GetSummary NOTIFY SummaryChanged)

    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("tied to C++ instance ")

public:
    enum RequestType {
        // value by priority. higher happens sooner

        ShellStdout,
        ShellStderr,

        ImageBlockData,
        ImageMetadata,

        ShellExecInfo,
        TelemetryRequest,
    };
    struct ShellOutputRequest
    {
        uint8_t exec_id;
        uint16_t block_index;
        bool compressed;
        bool operator==(const ShellOutputRequest &other) const;
        bool operator<(const ShellOutputRequest &other) const;
    };
    struct ImageDataRequest
    {
        uint8_t image_id;
        uint16_t block_index;
        bool operator==(const ImageDataRequest &other) const;
        bool operator<(const ImageDataRequest &other) const;
    };

    struct Request
    {
        RequestType type;
        union {
            ShellOutputRequest stdout;
            ShellOutputRequest stderr;

            ImageDataRequest image_data;
            uint8_t image_metadata_id;

            uint8_t shell_exec_info_id;
            TelemetryType telem_type;
        };
        bool operator==(const Request &other) const;
        bool operator<(const Request &other) const;
        QString summarize() const;
    };

    Librarian();

    /**
     * @brief SubmitRequestToRadio grabs the highest priority request from its queue and sends it to the radio
     */
    void SubmitRequestToRadio();

    /**
     * @brief AddRequests to the list of pondered requests that the librarian will choose from
     * @param reqs list of requests to add.
     * Upon adding, the list is flushed to disk
     */
    void AddRequests(const QList<Request> &reqs);
    void AddRequest(Request req);

    // process incoming data, remove request from queue and add any new requests to it
    void ShellExecInfoReceived(const ShellExecReturnData &info);
    void ImageMetadataReceived(const struct ImageMetadata &metadata);

    // if there was a timer that had gone off and asked for this, and we just got it, clear out the queue so they don't stack up
    void TelemetryReceived(TelemetryType type);

    size_t NumRequests();
    std::optional<Librarian::Request> Pop();

    // removes requests for this image from what we're looking for
    Q_INVOKABLE void StopImageDownload(uint8_t image_id);
    Q_INVOKABLE void StartImageDownload(uint8_t image_id, ImageDataHolder *image);
    void AddImageRequests(uint8_t image_id,
                          uint16_t num_blocks,
                          const ImageDataHolder::DownloadProgress &progress);

    Q_INVOKABLE bool activelyAskingForImage(uint8_t image_id) const;

    void DumpInfo() const;

    Q_INVOKABLE QList<Request> getHead(size_t num);
    Q_INVOKABLE QVariantList GetSummary();
    // search around directories for what we have and don't have (run at startup)
    void GatherRequestsFromDisk(ImageDataHolder *image);

public slots:
    void ImageDataReceived(QDateTime time, const ImageData &data);

    void ShellStdoutReceived(const ShellReadOutputData &data);
    void ShellStderrReceived(const ShellReadOutputData &data);

signals:
    void NumRequestsChanged();
    void SummaryChanged();

protected:
    std::set<Request> queue;
};

#endif // LIBRARIAN_H
