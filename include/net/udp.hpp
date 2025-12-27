#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace net {

class UDPClient {
public:
    UDPClient();
    ~UDPClient();

    // Send a UDP packet to serverIp:port and return the response
    std::vector<uint8_t> sendAndReceive(const std::string& serverIp,
                                        uint16_t port,
                                        const std::vector<uint8_t>& data);

private:
    int sockfd;
};

}
