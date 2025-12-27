# DNS Resolver in C++

A simple DNS resolver I built to learn how DNS queries work under the hood. It sends UDP packets to DNS servers and parses the responses.

## What This Does

Basically, you give it a domain name (like `google.com`) and it queries a DNS server to get the IP address back. Pretty straightforward but was fun figuring out how DNS packets are structured.

## Building It

### On Windows (with MinGW/MSYS2)

```bash
cd build
cmake -S .. -B . -G "MinGW Makefiles"
cmake --build .
```

Then run it:
```bash
.\dns_resolver.exe google.com
```

### On Linux/WSL

```bash
mkdir -p build && cd build
cmake -G "Unix Makefiles" ..
make -j4
./dns_resolver google.com
```

## Requirements

- CMake (3.16 or higher)
- C++17 compiler
- On Windows: MinGW-w64 or MSYS2
- On Linux: g++ and standard build tools

## Project Structure

```
├── include/
│   ├── dns/          # DNS-related headers
│   │   ├── header.hpp
│   │   ├── question.hpp
│   │   ├── record.hpp
│   │   ├── packet.hpp
│   │   ├── parser.hpp
│   │   ├── encoder.hpp
│   │   ├── cache.hpp
│   │   └── resolver.hpp
│   └── net/
│       └── udp.hpp   # UDP client
├── src/
│   ├── main.cpp
│   ├── dns/          # DNS implementations
│   └── net/
│       └── udp.cpp   # Cross-platform UDP (Winsock + POSIX)
└── CMakeLists.txt
```

## How It Works

1. **Encoder** - Takes a DNS query and converts it to binary format (network byte order)
2. **UDP Client** - Sends the query to a DNS server (defaults to Google's 8.8.8.8)
3. **Parser** - Reads the binary response and extracts the IP address
4. **Cache** - Stores results so we don't query the same domain twice
5. **Resolver** - Ties everything together

## Things I Learned

- DNS packets use network byte order (big-endian), so you have to convert everything
- DNS names are encoded with length prefixes (not null-terminated strings!)
- Windows uses Winsock while Linux uses POSIX sockets - had to make it work on both
- DNS can use pointer compression in responses to save space (the 0xC0 prefix thing)

## Known Issues / Limitations

- Only supports A records (IPv4) right now, no AAAA or other types
- No recursive resolution - just sends one query to 8.8.8.8
- Doesn't handle all DNS response codes
- Cache is in-memory only (lost when program exits)
- Timeout handling could be better

## Future Ideas

If I come back to this:
- [ ] Add AAAA record support (IPv6)
- [ ] Implement actual recursive resolution
- [ ] Better error handling
- [ ] Make DNS server configurable via CLI
- [ ] Add more record types (MX, CNAME, etc.)
- [ ] Save cache to disk

## Example Usage

```bash
# Resolve a domain
.\dns_resolver.exe example.com

# Try different domains
.\dns_resolver.exe github.com
.\dns_resolver.exe youtube.com
```

## Troubleshooting

**Build fails with missing socket headers on Windows?**
- Make sure you're using MinGW or MSYS2, not MSVC
- The code uses Winsock2 on Windows

**"Failed to create UDP socket"?**
- On Windows, might need to run as admin for network access
- Check firewall settings

**No output or hangs?**
- DNS port (53) might be blocked
- Try a different network or disable VPN

## License

Do whatever you want with this code. It's just a learning project.

---

Made this to understand how DNS works at a lower level. Definitely learned a lot about network byte order and UDP sockets!
