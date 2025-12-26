#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace dns {

struct Record {
    std::string name;        // Domain name
    uint16_t type;          // Type of the record
    uint16_t rclass;        // Class of the record
    uint32_t ttl;           // Time to live
    std::vector<uint8_t> rdata; // Resource data

};

}