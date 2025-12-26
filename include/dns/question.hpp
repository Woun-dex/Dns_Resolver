#pragma once
#include <string>
#include <cstdint>

namespace dns {

#pragma pack(push, 1)

enum class RecordType : uint16_t {
    A     = 1,
    NS    = 2,
    CNAME = 5
};


struct Question {
    std::string name;      // Domain name
    RecordType type;       // Type of the query
    uint16_t qclass;        // Class of the query
};

}