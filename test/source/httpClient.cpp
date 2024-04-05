#include <catch2/catch_test_macros.hpp>
#include <httpClient.h>
#include <nlohmann/json.hpp>
#include <utils.h>

using namespace bittorrent;

TEST_CASE("get request")
{
    HTTPClient client;
    HTTPClient::Params params;
    params["hello"] = "world";
    const std::string& result = client.get("http://www.httpbin.org/get", params);
    nlohmann::json json = nlohmann::json::parse(result);

    CHECK(result.size() > 0);
    CHECK(json["args"]["hello"] == "world");
}

TEST_CASE("get request with an encoded hash")
{
    HTTPClient client;
    HTTPClient::Params params;

    utils::Sha1Hash hash = utils::sha1("hello world!");
    params["hash"] = hash.asRawString();

    const std::string& result = client.get("http://www.httpbin.org/get", params);
    nlohmann::json json = nlohmann::json::parse(result);

    CHECK(result.size() > 0);
    CHECK(json["args"]["hash"].get<std::string>().size() > 0);
}