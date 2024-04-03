#include <catch2/catch_test_macros.hpp>
#include <hash.h>
#include <utils.h>

TEST_CASE("hash view")
{
    bittorrent::HashView<3> view;
    CHECK(view == "000000");
    CHECK(view[0] == 0);
    CHECK(view[1] == 0);
    CHECK(view[2] == 0);

    view[0] = 0xda;
    view[1] = 0x39;
    view[2] = 0xa3;

    CHECK(view[0] == 0xda);
    CHECK(view[1] == 0x39);
    CHECK(view[2] == 0xa3);
    CHECK(view == "da39a3");
}

TEST_CASE("sha1 hash")
{
    CHECK(bittorrent::utils::sha1("The quick brown fox jumps over the lazy dog") == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
    CHECK(bittorrent::utils::sha1("") == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}