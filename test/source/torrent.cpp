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
}
