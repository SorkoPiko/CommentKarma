#include <Geode/modify/ProfilePage.hpp>
#include "LevelCommentDelegate.hpp"

using namespace geode::prelude;

class $modify(CKProfilePage, ProfilePage) {

    struct Fields {
        int m_page = 0;
    };

    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;
        calculateKarma();
    }

    int calculateKarma() {
        if (!m_score) return -1;
        const auto glm = GameLevelManager::sharedState();
        glm->m_levelCommentDelegate = new CKLevelCommentDelegate();
        const auto key = glm->getCommentKey(m_score->m_userID, m_fields->m_page, 1, CommentKeyType::User);
        ++m_fields->m_page;
        log::info("{}", key);
        glm->getLevelComments(m_score->m_userID, m_fields->m_page, 0, 1, CommentKeyType::User);
        return 0;
    }
};