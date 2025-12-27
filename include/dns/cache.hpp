#pragma once
#include "record.hpp"
#include <unordered_map>
#include <chrono>

namespace dns {

class Cache {
public:
    bool get(const std::string& key, Record& out);
    void put(const std::string& key, const Record& r);

private:
    struct Entry {
        Record record;
        std::chrono::steady_clock::time_point expires;
    };
    std::unordered_map<std::string, Entry> store;
};

}
