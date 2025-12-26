#pragma once
#include <cstdint>

namespace dns {

#pragma pack(push, 1)

struct Header {
    uint16_t id;       // Identification
    uint16_t flags;    // Flags
    uint16_t qdcount;  // Number of question entries
    uint16_t ancount;  // Number of answer entries
    uint16_t nscount;  // Number of authority entries
    uint16_t arcount; 
 };
 
 #pragma pack(pop)
}