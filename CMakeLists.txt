cmake_minimum_required(VERSION 3.16)
project(cpp_dns_resolver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(dns_resolver
    src/main.cpp
    src/dns/parser.cpp
    src/dns/encoder.cpp
    src/dns/cache.cpp
    src/dns/resolver.cpp
    src/net/udp.cpp
)

if(UNIX)
    target_link_libraries(dns_resolver pthread)
endif()

if(WIN32)
    target_link_libraries(dns_resolver ws2_32)
endif()
