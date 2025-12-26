#pragma once 
#include "record.hpp"
#include <unordered_map>
#include <chrono>

namespace dns {

    class Cache {
        public:
            bool get(const std::string& name, uint16_t type , Record& record);
            void put(const std::string& name, uint16_t type , const Record& record);

        private:
            struct Entry {
                Record record;
                std::chrono::steady_clock::time_point expiry;
            };

            std::unordered_map<std::string, Entry> store;
};

}