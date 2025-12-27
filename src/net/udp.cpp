#include "net/udp.hpp"
#include <stdexcept>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

namespace net {

UDPClient::UDPClient() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Failed to create UDP socket");
    }
#else
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) throw std::runtime_error("Failed to create UDP socket");
#endif
}

UDPClient::~UDPClient() {
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
}

std::vector<uint8_t> UDPClient::sendAndReceive(const std::string& serverIp,
                                               uint16_t port,
                                               const std::vector<uint8_t>& data) {
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
#ifdef _WIN32
    addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
#else
    inet_pton(AF_INET, serverIp.c_str(), &addr.sin_addr);
#endif

#ifdef _WIN32
    if (sendto(sockfd, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0,
               reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        throw std::runtime_error("sendto failed");
    }
#else
    if (sendto(sockfd, data.data(), data.size(), 0,
               reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        throw std::runtime_error("sendto failed");
    }
#endif

    uint8_t buffer[512];
#ifdef _WIN32
    int received = recvfrom(sockfd, reinterpret_cast<char*>(buffer), sizeof(buffer), 0, nullptr, nullptr);
    if (received == SOCKET_ERROR) throw std::runtime_error("recvfrom failed");
#else
    ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (received < 0) throw std::runtime_error("recvfrom failed");
#endif

    return std::vector<uint8_t>(buffer, buffer + received);
}

}
