#include "dns/resolver.hpp"
#include "dns/encoder.hpp"
#include "dns/parser.hpp"
#include "net/udp.hpp"

namespace dns {

Resolver::Resolver() {}

Packet Resolver::queryServer(const std::string& domain, const std::string& serverIp) {
    Packet pkt;
    pkt.header.id = 0x1234;
    pkt.header.flags = 0x0100;
    pkt.header.qdcount = 1;
    pkt.header.ancount = 0;
    pkt.header.nscount = 0;
    pkt.header.arcount = 0;
    pkt.questions.push_back({ domain, RecordType::A, 1 });

    auto data = Encoder::encode(pkt);

    net::UDPClient udp;
    auto response = udp.sendAndReceive(serverIp, 53, data);

    return Parser::parse(response, response.size());
}

std::string Resolver::resolveRecursive(const std::string& domain, const std::string& serverIp) {
    Packet resp = queryServer(domain, serverIp);
    
    for (auto& r : resp.answers) {
        if (r.type == 1 && r.rdata.size() >= 4) {
            std::string ip = std::to_string(r.rdata[0]) + "." +
                           std::to_string(r.rdata[1]) + "." +
                           std::to_string(r.rdata[2]) + "." +
                           std::to_string(r.rdata[3]);
            cache.put(domain, r);
            return ip;
        }
    }
    return "";
}

std::string Resolver::resolveA(const std::string& domain) {
    Record r;
    if (cache.get(domain, r) && r.rdata.size() >= 4) {
        return std::to_string(r.rdata[0]) + "." +
               std::to_string(r.rdata[1]) + "." +
               std::to_string(r.rdata[2]) + "." +
               std::to_string(r.rdata[3]);
    }
    return resolveRecursive(domain, "8.8.8.8");
}

}
