#pragma once
#include "packet.hpp"
#include <vector>
#include <cstddef>

namespace dns {

    class Parser {
    public:
        static Packet parse(const std::vector<uint8_t>& data, size_t length);
    private:
        static std::string parseName(const std::vector<uint8_t>& data, size_t& offset);
        static Record parseRecord(const std::vector<uint8_t>& data, size_t& offset);
    };
}