#include "LevelCommentDelegate.hpp"

using namespace geode::prelude;

void CKLevelCommentDelegate::loadCommentsFinished(CCArray* comments, const char* key) {
    log::info("Comments loaded for key: {}", key);
}
