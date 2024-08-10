#include "KarmaScraper.hpp"

using namespace geode::prelude;

void KarmaScraper::setCallback(const std::function<void(CCArray*, bool)>& callback) {
    m_callback = callback;
}

void KarmaScraper::loadCommentsFinished(CCArray* comments, const char* key) {
    m_callback(comments, true);
}

void KarmaScraper::loadCommentsFailed(char const *key) {
    m_callback(new CCArray(), false);
}
