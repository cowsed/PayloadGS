#include "librarian.h"
#include "packets_p2g.h"

Librarian::Librarian() {}

void Librarian::GatherRequestsFromDisk(ImageDataHolder *image)
{
    // Images
    qDebug("Gathering info for %hhu images", image->numImages());
    for (uint8_t i = 0; i < image->numImages(); i++) {
        ImageMetadataHolder meta = image->metadataForImageId(i);
        if (!meta.isValid()) {
            // request metadata
            AddRequest({
                .type = RequestType::ImageMetadata,
                .image_metadata_id = i,
            });
            continue;
        }
        ImageDataHolder::DownloadProgress prog = image->downloadedPackets(i);
        AddImageRequests(i, meta.numBlocks(), prog);
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
        return stdout == other.stdout;
    case RequestType::ShellStderr:
        return stderr == other.stderr;
    case RequestType::ImageBlockData:
        return image_data == other.image_data;
    case RequestType::ImageMetadata:
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
        return stdout < other.stdout;
    case RequestType::ShellStderr:
        return stderr < other.stderr;
    case RequestType::ImageBlockData:
        return image_data < other.image_data;
    case RequestType::ImageMetadata:
        return image_metadata_id < other.image_metadata_id;
    case RequestType::ShellExecInfo:
        return shell_exec_info_id < other.shell_exec_info_id;
    case RequestType::TelemetryRequest:
        return telem_type < other.telem_type;
    default:
        return false;
    }
}

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
            qDebug("Req: ImageData image: %d block: %d",
                   req.image_data.image_id,
                   req.image_data.block_index);
            break;
        default:
            qDebug("Unknonw request");
        };
    }
}

std::optional<Librarian::Request> Librarian::Pop()
{
    if (queue.size() < 1) {
        return std::nullopt;
    }
    Request r = *queue.begin();
    queue.erase(r);
    return r;
}

void Librarian::StopImageDownload(uint8_t image_id)
{
    std::erase_if(queue, [image_id](const Request &r) {
        if (r.type != RequestType::ImageBlockData) {
            return false;
        }
        if (r.image_data.image_id == image_id) {
            return true;
        }
        return false;
    });
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
}

bool Librarian::activelyAskingForImage(uint8_t image_id) const
{
    return std::any_of(queue.cbegin(), queue.cend(), [image_id](const Request &r) {
        if (r.type == RequestType::ImageBlockData) {
            return r.image_data.image_id == image_id;
        }
        if (r.type == RequestType::ImageMetadata) {
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
