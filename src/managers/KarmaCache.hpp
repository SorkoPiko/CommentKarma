#ifndef KARMACACHE_HPP
#define KARMACACHE_HPP

class KarmaCache {
protected:
    static std::unordered_map<int, int> cache;

public:
    static void setKarma(int userID, int karma);
    static int getKarma(int userID);
};

#endif
