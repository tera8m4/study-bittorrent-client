#include "torrent.h"
#include "bencode/bencode.h"
#include "utils.h"
#include <iostream>

using namespace bittorrent;

TorrentFile::TorrentFile(const std::filesystem::path& inFilePath)
    : filePath { inFilePath }
{
}

void TorrentFile::decode()
{
    const std::string& fileContent = bittorrent::utils::readFile(filePath);
    metaData = Bencode::decode(fileContent);
}

std::string TorrentFile::getTrackerURL() const
{
    return metaData.at("announce").get<std::string>();
}

std::size_t TorrentFile::getLength() const
{
    return metaData.at("info").at("length").get<std::size_t>();
}

std::string bittorrent::TorrentFile::getInfoHash() const
{
    const std::string& encodedInfo = Bencode::encode(metaData.at("info"));
    return utils::sha1(encodedInfo);
}
