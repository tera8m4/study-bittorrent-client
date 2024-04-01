#include <bencode/bencode.h>

#include <catch2/catch_test_macros.hpp>

using namespace bittorrent;

TEST_CASE("Bencode decode a number")
{
    const auto& negativeNumber = Bencode::decode("i-421e");

    CHECK(negativeNumber.get<int>() == -421);

    const auto& positiveNumber = Bencode::decode("i53e");
    CHECK(positiveNumber.get<int>() == 53);

    const auto& longDataResult = Bencode::decode("i52e89u398th9f98sdfoigjlkfdjgksfdjg");
    CHECK(longDataResult.get<int>() == 52);

    REQUIRE_THROWS(Bencode::decode("i35553"));
}