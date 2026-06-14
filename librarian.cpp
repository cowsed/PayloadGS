#include "librarian.h"
#include <QQmlApplicationEngine>
#include "cubesat_comms/packets_p2g.h"

Librarian::Librarian()
{
    QQmlApplicationEngine engine;
    holder = engine.singletonInstance<ImageDataHolder *>("PayloadGS", "ImageDataHolder");
}

void Librarian::GatherRequestsFromDisk()
{
    // Images
    qDebug("Lib: Gathering info for %hhu images", holder->numImages());
    for (uint8_t i = 0; i < holder->numImages(); i++) {
        StartImageDownload(i, holder);
    }
}

bool Librarian::ShellOutputRequest::operator==(const ShellOutputRequest &other) const
{
    return exec_id == other.exec_id && block_index == other.block_index
           && compressed == other.compressed;
}
bool Librarian::ShellOutputRequest::operator<(const ShellOutputRequest &other) const
{
    if (exec_id == other.exec_id) {
        if (block_index == other.block_index) {
            return compressed < other.compressed;
        }
        return block_index < other.block_index;
    }
    return exec_id < other.exec_id;
}

bool Librarian::ImageDataRequest::operator==(const ImageDataRequest &other) const
{
    return image_id == other.image_id && block_index == other.block_index;
}

bool Librarian::ImageDataRequest::operator<(const ImageDataRequest &other) const
{
    if (image_id == other.image_id) {
        return block_index < other.block_index;
    }
    return image_id < other.image_id;
}

bool Librarian::Request::operator==(const Request &other) const
{
    if (type != other.type) {
        return false;
    }
    // types are equal
    switch (type) {
    case RequestType::ShellStdout:
        return this->stdout == other.stdout;
    case RequestType::ShellStderr:
        return this->stderr == other.stderr;
    case RequestType::ImageBlockData:
        return image_data == other.image_data;
    case RequestType::SingleImageMetadata:
        return image_metadata_id == other.image_metadata_id;
    case RequestType::ShellExecInfo:
        return shell_exec_info_id == other.shell_exec_info_id;
    case RequestType::TelemetryRequest:
        return telem_type == other.telem_type;
    default:
        return false;
    }
}
bool Librarian::Request::operator<(const Request &other) const
{
    if (type != other.type) {
        return type < other.type;
    }
    switch (type) {
    case RequestType::ShellStdout:
        return this->stdout < other.stdout;
    case RequestType::ShellStderr:
        return this->stderr < other.stderr;
    case RequestType::ImageBlockData:
        return image_data < other.image_data;
    case RequestType::SingleImageMetadata:
        return image_metadata_id < other.image_metadata_id;
    case RequestType::ShellExecInfo:
        return shell_exec_info_id < other.shell_exec_info_id;
    case RequestType::TelemetryRequest:
        return telem_type < other.telem_type;
    default:
        return false;
    }
}

void Librarian::NumImagesIncreased(QDateTime time, uint8_t next_image)
{
    // StartImageDownload(next_image);
}
void Librarian::ImageMetadataReceived(QDateTime time, const struct ImageMetadata &metadata) {}

void Librarian::AddRequest(Request r)
{
    queue.insert(r);
}

size_t Librarian::NumRequests()
{
    return queue.size();
}
void Librarian::DumpInfo() const
{
    for (const auto &req : queue) {
        switch (req.type) {
        case RequestType::ImageBlockData:
            qDebug("Lib: Req ImageData image: %d block: %d",
                   req.image_data.image_id,
                   req.image_data.block_index);
            break;
        default:
            qDebug("Lib: Unknown request");
        };
    }
}

std::vector<uint16_t> Librarian::gatherImageBlocksToRequest(uint8_t image_id,
                                                            uint16_t first_block,
                                                            size_t allowed_blocks)
{
    std::vector<uint16_t> blocks{};
    blocks.reserve(allowed_blocks);
    blocks.push_back(first_block);

    for (auto req : queue) { // traverse in order
        if (req.type != RequestType::ImageBlockData) {
            // not an image anymore
            break;
        }
        if (req.image_data.image_id != image_id) {
            // not this one anymore
            break;
        }
        if (blocks.size() >= allowed_blocks) {
            // full up
            break;
        }
        blocks.push_back(req.image_data.block_index);
    }
    return blocks;
}

void Librarian::SubmitRequestToRadio(RadioPacketParser *radio, size_t max_blocks)
{
    if (max_blocks > MAX_BLOCKS_PER_REQUEST) {
        max_blocks = MAX_BLOCKS_PER_REQUEST;
    }
    std::optional<Librarian::Request> maybe_req = Pop();
    if (!maybe_req) {
        qDebug("Nothing for librarian to do right now");
        return;
    }
    switch (maybe_req->type) {
    case RequestType::SingleImageMetadata:
        qDebug("Asking for metadata for image %d", (int) maybe_req->image_metadata_id);
        radio->askForMetadata(maybe_req->image_metadata_id);
        break;
    case RequestType::ImageBlockData: {
        std::vector<uint16_t> ids = gatherImageBlocksToRequest(maybe_req->image_data.image_id,
                                                               maybe_req->image_data.block_index,
                                                               max_blocks);
        radio->askForBlocks(maybe_req->image_data.image_id, ids);
    } break;
    default:
        qWarning("Have pity, the librarian is not smart and can't handle type %d",
                 (int) maybe_req->type);
    }
}

std::optional<Librarian::Request> Librarian::Pop()
{
    if (queue.size() < 1) {
        return std::nullopt;
    }
    Request r = *queue.begin();
    queue.erase(r);

    emit NumRequestsChanged();
    emit SummaryChanged();
    return r;
}

void Librarian::StopImageDownload(uint8_t image_id)
{
    size_t old_len = queue.size();
    std::erase_if(queue, [image_id](const Request &r) {
        if (r.type != RequestType::ImageBlockData) {
            return false;
        }
        if (r.image_data.image_id == image_id) {
            return true;
        }
        return false;
    });
    if (queue.size() != old_len) {
        emit NumRequestsChanged();
        emit SummaryChanged();
    }
}

void Librarian::StartImageDownload(uint8_t image_id, ImageDataHolder *image)
{
    ImageMetadataHolder meta = image->metadataForImageId(image_id);
    if (!meta.isValid()) {
        // request metadata
        AddRequest({
            .type = RequestType::SingleImageMetadata,
            .image_metadata_id = image_id,
        });
        qDebug("Lib: Asking for metadata for id %d", image_id);
        return;
    }
    ImageDataHolder::DownloadProgress prog = image->downloadedPackets(image_id);
    AddImageRequests(image_id, meta.numBlocks, prog);
}

void Librarian::AddImageRequests(uint8_t image_id,
                                 uint16_t num_blocks,
                                 const ImageDataHolder::DownloadProgress &progress_sofar)
{
    for (uint16_t block_i = 0; block_i < num_blocks; block_i++) {
        if (!progress_sofar.downloaded_packets.contains(block_i)) {
            AddRequest({.type = RequestType::ImageBlockData,
                        .image_data = {.image_id = image_id, .block_index = block_i}});
        }
    }
    emit NumRequestsChanged();
    emit SummaryChanged();
}

bool Librarian::activelyAskingForImage(uint8_t image_id) const
{
    return std::any_of(queue.cbegin(), queue.cend(), [image_id](const Request &r) {
        if (r.type == RequestType::ImageBlockData) {
            return r.image_data.image_id == image_id;
        }
        if (r.type == RequestType::SingleImageMetadata) {
            return r.image_metadata_id == image_id;
        }
        return false;
    });
}

void Librarian::ImageDataReceived(QDateTime time, const ImageData &data)
{
    const Request r = {
        .type = RequestType::ImageBlockData,
        .image_data = {.image_id = data.image_id, .block_index = data.block_index},
    };
    queue.erase(r);
    emit NumRequestsChanged();
    emit SummaryChanged();
}

QList<Librarian::Request> Librarian::getHead(size_t num)
{
    QList<Request> reqs{};
    for (const Request &r : queue) {
        if (reqs.size() > num) {
            break;
        }
        reqs.append(r);
    }
    return reqs;
}

void Librarian::ShellStdoutReceived(const ShellReadOutputData &data) {}
void Librarian::ShellStderrReceived(const ShellReadOutputData &data) {}

QVariantList Librarian::GetSummary()
{
    std::set<QString> sum;

    for (const Request &req : queue) {
        sum.insert(req.summarize());
    }
    QVariantList l;
    for (const QString &r : sum) {
        l.append(r);
    }

    return l;
}

QString Librarian::Request::summarize() const
{
    switch (type) {
    case ShellStdout:
        return QString("shell stdout %1").arg(stderr.exec_id);
    case ShellStderr:
        return QString("shell stderr %1").arg(stderr.exec_id);
    case ImageBlockData:
        return QString("image data %1").arg(image_data.image_id);
    case SingleImageMetadata:
        return QString("image meta %1").arg(image_metadata_id);
    case ShellExecInfo:
        return QString("shell return %1").arg(shell_exec_info_id);
    case TelemetryRequest:
        return "telemetry"; //telem_type.summarize();
    }
    return "Unknown req";
}
