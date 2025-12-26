#pragma once
#include "packet.hpp"
#include "cache.hpp"
#include <string>

namespace dns {

    class Resolver {
    public:
        explicit Resolver();
        std::string resolveA(const std::string& domain);
    private:
        Cache cache;
        std::string resolveRecursive(const std::string& domain, const std::string& serverIp);

        Packet queryServer(const std::string& domain,  const std::string& serverIp);
    };

}   