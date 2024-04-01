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

std::size_t bittorrent::TorrentFile::getPieceLength() const
{
    return metaData.at("info").at("piece length");
}

std::vector<std::string> bittorrent::TorrentFile::getPieceHashes() const
{
    std::cout << metaData.at("info").at("pieces").type_name() << std::endl;

    const std::string& pieces = metaData.at("info").at("pieces").get<std::string>();
    std::vector<std::string> result;
    result.reserve(pieces.size() / 20);

    for (std::size_t i = 0; i < 40; i += 20) {

        std::stringstream ss;
        ss << std::hex
           << std::setfill('0')
           << std::setw(2);

        for (int j = 0; j < 20; ++j) {
            const unsigned char c = pieces[i + j];
            ss << static_cast<int>(c);
        }
        result.emplace_back(ss.str());
    }

    return result;
}
