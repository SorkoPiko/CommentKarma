#include <chrono>
#include "RequestStutter.hpp"

using namespace geode::prelude;

long long RequestStutter::lastRequestTime = 0;

long long getCurrentTimestampMillis() {
    const auto now = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return duration.count();
}

float RequestStutter::getRequestTime() {
    constexpr auto maxReqs = 80;
    constexpr auto delay = 60 * 1000 / maxReqs;
    const auto now = getCurrentTimestampMillis();
    const auto nextRequestTime = lastRequestTime + delay;
    lastRequestTime = std::max(nextRequestTime, now);
    return (lastRequestTime - now) / 1000.f;
}