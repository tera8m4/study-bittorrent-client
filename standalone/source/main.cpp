#include <bencode/bencode.h>
#include <iostream>
#include <string_view>

int main()
{
    std::cout << bittorrent::Bencode::decode(std::string_view { "i52e" })
              << std::endl;
    return 0;
}