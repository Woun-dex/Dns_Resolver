#include "dns/parser.hpp"

namespace dns {

static uint16_t read_u16(const std::vector<uint8_t>& data, size_t& offset) {
    uint16_t v = (uint16_t(data[offset]) << 8) | uint16_t(data[offset + 1]);
    offset += 2;
    return v;
}

static uint32_t read_u32(const std::vector<uint8_t>& data, size_t& offset) {
    uint32_t v = (uint32_t(data[offset]) << 24) | (uint32_t(data[offset + 1]) << 16) | (uint32_t(data[offset + 2]) << 8) | uint32_t(data[offset + 3]);
    offset += 4;
    return v;
}

std::string Parser::parseName(const std::vector<uint8_t>& data, size_t& offset) {
    std::string name;
    bool jumped = false;
    size_t jumpOffset = 0;

    while (true) {
        uint8_t len = data[offset];
        if ((len & 0xC0) == 0xC0) {
            uint16_t ptr = uint16_t((len & 0x3F) << 8) | data[offset + 1];
            if (!jumped) jumpOffset = offset + 2;
            offset = ptr;
            jumped = true;
            continue;
        }
        if (len == 0) {
            offset++;
            break;
        }
        offset++;
        if (!name.empty()) name += '.';
        name.append(reinterpret_cast<const char*>(data.data() + offset), len);
        offset += len;
    }

    if (jumped) offset = jumpOffset;
    return name;
}

Record Parser::parseRecord(const std::vector<uint8_t>& data, size_t& offset) {
    Record r;
    r.name = parseName(data, offset);
    r.type = read_u16(data, offset);
    r.rclass = read_u16(data, offset);
    r.ttl = read_u32(data, offset);
    uint16_t rdlen = read_u16(data, offset);
    r.rdata.assign(data.begin() + offset, data.begin() + offset + rdlen);
    offset += rdlen;
    return r;
}

Packet Parser::parse(const std::vector<uint8_t>& data, size_t length) {
    Packet pkt;
    if (length < sizeof(Header)) return pkt;

    // copy header fields manually (network byte order -> host)
    const Header* h = reinterpret_cast<const Header*>(data.data());
    pkt.header.id = h->id;
    pkt.header.flags = h->flags;
    pkt.header.qdcount = (uint16_t)((data[4] << 8) | data[5]);
    pkt.header.ancount = (uint16_t)((data[6] << 8) | data[7]);
    pkt.header.nscount = (uint16_t)((data[8] << 8) | data[9]);
    pkt.header.arcount = (uint16_t)((data[10] << 8) | data[11]);

    size_t offset = sizeof(Header);

    for (int i = 0; i < pkt.header.qdcount; i++) {
        Question q;
        q.name = parseName(data, offset);
        q.type = (RecordType)read_u16(data, offset);
        q.qclass = read_u16(data, offset);
        pkt.questions.push_back(q);
    }

    for (int i = 0; i < pkt.header.ancount; i++)
        pkt.answers.push_back(parseRecord(data, offset));

    return pkt;
}

}
