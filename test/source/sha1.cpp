#include <utils.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("sha1 hash")
{
    CHECK(bittorrent::utils::sha1("The quick brown fox jumps over the lazy dog") == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
    CHECK(bittorrent::utils::sha1("") == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}