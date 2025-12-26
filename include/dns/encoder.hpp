#pragma once
#include <vector>
#include "packet.hpp"

namespace dns {


class Encoder {
public:
    static std::vector<uint8_t> encode(const Packet& packet);
private:
    static void encodeName(const std::string& name, std::vector<uint8_t>& buffer);
    
};

}