#pragma once

#include "utils.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace bittorrent {
class TorrentFile {
public:
    TorrentFile(const std::filesystem::path& inFilePath);
    void decode();

    std::string getTrackerURL() const;
    std::size_t getLength() const;
    std::string getInfoHash() const;
    std::size_t getPieceLength() const;
    std::vector<utils::Sha1Hash> getPieceHashes() const;

private:
    std::filesystem::path filePath;
    nlohmann::json metaData;
};
}