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

TEST_CASE("Bencode decode a string")
{
    const auto& negativeNumber = Bencode::decode("5:-421");

    CHECK(negativeNumber.get<std::string>() == "-421");

    const auto& positiveNumber = Bencode::decode("5:hello");
    CHECK(positiveNumber.get<std::string>() == "hello");

    const auto& longDataResult = Bencode::decode("3:hello");
    CHECK(longDataResult.get<std::string>() == "hel");
}

TEST_CASE("Bencode decode a list")
{
    const auto& simpleList = Bencode::decode("l5:helloi42ee");
    CHECK(simpleList.is_array());
    CHECK(simpleList.size() == 2);
    CHECK(simpleList[0].get<std::string>() == "hello");
    CHECK(simpleList[1].get<int>() == 42);

    const auto& emptyList = Bencode::decode("le");
    CHECK(emptyList.is_array());
    CHECK(emptyList.size() == 0);
}

TEST_CASE("Bencode decode a dictionary")
{
    const auto& simpleDictionary = Bencode::decode("d3:foo3:bar5:helloi52ee");
    CHECK(simpleDictionary.is_object());
    CHECK(simpleDictionary["foo"].get<std::string>() == "bar");
}

TEST_CASE("Bencode encode a number")
{
    const Bencode::data_t& positiveNumber = 42;
    CHECK(Bencode::encode(positiveNumber) == "i42e");

    const Bencode::data_t& negativeNumber = -542;
    CHECK(Bencode::encode(negativeNumber) == "i-542e");
}

TEST_CASE("Bencode encode a string")
{
    const std::string& testString = "hello world";
    const Bencode::data_t& testData = testString;
    CHECK(Bencode::encode(testData) == std::to_string(testString.size()) + ":" + testString);

    const Bencode::data_t& emptyString = std::string {};
    CHECK(Bencode::encode(emptyString) == "0:");
}

TEST_CASE("Bencode encode an array")
{
    Bencode::data_t testArray = Bencode::data_t::array({ 42, -52, "hello" });
    CHECK(Bencode::encode(testArray) == "li42ei-52e5:helloe");

    Bencode::data_t emptyArray = Bencode::data_t::array();
    CHECK(Bencode::encode(emptyArray) == "le");
}

TEST_CASE("Bencode encode a dictionary")
{
    Bencode::data_t testDictionary;
    testDictionary.emplace("a", -42);
    testDictionary.emplace("b", "hello");

    CHECK(Bencode::encode(testDictionary) == "d1:ai-42e1:b5:helloe");
}