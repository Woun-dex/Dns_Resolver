#include "dns/cache.hpp"

namespace dns {

bool Cache::get(const std::string& key, Record& out) {
    auto it = store.find(key);
    if (it == store.end()) return false;
    if (std::chrono::steady_clock::now() > it->second.expires) {
        store.erase(it);
        return false;
    }
    out = it->second.record;
    return true;
}

void Cache::put(const std::string& key, const Record& r) {
    store[key] = { r, std::chrono::steady_clock::now() + std::chrono::seconds(r.ttl) };
}

}
