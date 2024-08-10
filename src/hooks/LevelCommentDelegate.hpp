#ifndef LEVELCOMMENTDELEGATE_HPP
#define LEVELCOMMENTDELEGATE_HPP

#include <Geode/modify/LevelCommentDelegate.hpp>

using namespace geode::prelude;

class $modify(CKLevelCommentDelegate, LevelCommentDelegate) {
    void loadCommentsFinished(CCArray* comments, const char* key);
};

#endif
