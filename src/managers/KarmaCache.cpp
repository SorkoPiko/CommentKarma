#include "KarmaCache.hpp"

std::unordered_map<int, int> KarmaCache::cache = {};

void KarmaCache::setKarma(const int userID, const int karma) {
    cache[userID] = karma;
}

int KarmaCache::getKarma(const int userID) {
    return cache[userID];
}