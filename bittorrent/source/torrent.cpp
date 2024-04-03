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

std::vector<utils::Sha1Hash> bittorrent::TorrentFile::getPieceHashes() const
{
    std::cout << metaData.at("info").at("pieces").type_name() << std::endl;

    const std::string& pieces = metaData.at("info").at("pieces").get<std::string>();
    std::vector<utils::Sha1Hash> result;
    constexpr std::size_t SHA1_SIZE = 20;
    const std::size_t piecesNum = pieces.size() / SHA1_SIZE;
    result.reserve(piecesNum);

    for (std::size_t i = 0; i < piecesNum; i += SHA1_SIZE) {
        utils::Sha1Hash hash;
        for (int j = 0; j < 20; ++j) {
            hash[j] = pieces[i + j];
        }
        result.emplace_back(hash);
    }

    return result;
}
