#include "dns/encoder.hpp"

namespace dns {

static void write_u16(std::vector<uint8_t>& buf, uint16_t val) {
    buf.push_back(uint8_t(val >> 8));
    buf.push_back(uint8_t(val & 0xFF));
}

void Encoder::encodeName(const std::string& name, std::vector<uint8_t>& buffer) {
    size_t start = 0;
    while (true) {
        size_t dot = name.find('.', start);
        size_t len = (dot == std::string::npos) ? name.size() - start : dot - start;
        buffer.push_back(static_cast<uint8_t>(len));
        for (size_t i = start; i < start + len; i++)
            buffer.push_back(name[i]);
        if (dot == std::string::npos) break;
        start = dot + 1;
    }
    buffer.push_back(0);
}

std::vector<uint8_t> Encoder::encode(const Packet& packet) {
    std::vector<uint8_t> buf;

    // Write header fields in network byte order
    write_u16(buf, packet.header.id);
    write_u16(buf, packet.header.flags);
    write_u16(buf, packet.header.qdcount);
    write_u16(buf, packet.header.ancount);
    write_u16(buf, packet.header.nscount);
    write_u16(buf, packet.header.arcount);

    for (const auto& q : packet.questions) {
        encodeName(q.name, buf);
        write_u16(buf, static_cast<uint16_t>(q.type));
        write_u16(buf, q.qclass);
    }

    return buf;
}

}
