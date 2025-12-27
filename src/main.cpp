#include "dns/resolver.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <domain>\n";
        return 1;
    }

    std::string domain = argv[1];
    dns::Resolver resolver;

    try {
        std::string ip = resolver.resolveA(domain);
        if (!ip.empty()) {
            std::cout << domain << " -> " << ip << "\n";
        } else {
            std::cout << "Could not resolve " << domain << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
