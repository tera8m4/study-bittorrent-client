#include <torrent.h>

#include <catch2/catch_test_macros.hpp>

using namespace bittorrent;

TEST_CASE("Torrent file")
{
    TorrentFile torrent { "ubuntu-23.10.1-desktop-amd64.iso.torrent" };

    SECTION("decoding a torrent file")
    {
        torrent.decode();

        CHECK(torrent.getTrackerURL() == "https://torrent.ubuntu.com/announce");
        CHECK(torrent.getLength() > 0);
    }

    torrent.decode();

    SECTION("calculating info hash")
    {
        const auto& infoHash = torrent.getInfoHash();
        CHECK(infoHash == "9ecd4676fd0f0474151a4b74a5958f42639cebdf");
    }

    SECTION("get pieces information")
    {
        CHECK(torrent.getPieceLength() == 262144);
        const std::vector<std::string>& pieces = torrent.getPieceHashes();
        CHECK(pieces.size() > 0);
        CHECK(pieces[0] == "b593682638ebd24032736050c1744adc5befd732");
    }
}
