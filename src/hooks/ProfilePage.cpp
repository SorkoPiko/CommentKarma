#include <Geode/modify/ProfilePage.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include "../delegates/KarmaScraper.hpp"
#include "../managers/KarmaCache.hpp"
#include "../managers/RequestStutter.hpp"

using namespace geode::prelude;

class $modify(CKProfilePage, ProfilePage) {

    struct Fields {
        int m_page = 0;
        KarmaScraper m_karmaScraper;
        std::set<std::string> m_commentIDs;
        int m_karma = 0;
        CCLabelBMFont* m_karmaLabel;
        LoadingSpinner* m_loadingSpinner;
        bool m_nrl = false;
    };

    void onClose(CCObject* sender) {
        GameLevelManager::sharedState()->m_levelCommentDelegate = nullptr;
        ProfilePage::onClose(sender);
    }

    void loadCommentsFinished(CCArray* comments, const char* key) {
        ProfilePage::loadCommentsFinished(comments, key);

        if (!m_score) {
            return;
        }
        if (m_score->m_commentHistoryStatus != 0) {
            return;
        }

        const auto winSize = CCDirector::sharedDirector()->getWinSize();
        const auto myStuffHint = m_mainLayer->getChildByID("my-stuff-hint");
        const auto globalRankIcon = m_mainLayer->getChildByID("global-rank-icon");

        auto offset = 120.5f;
        auto yPos = 283.f;
        if (globalRankIcon) {
            offset = globalRankIcon->getPositionX();
            yPos = globalRankIcon->getPositionY();
        }
        const auto xPos = winSize.width/2 - offset + winSize.width/2 - 20.f;
        const auto karmaIcon = CCSprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
        karmaIcon->setID("karma-icon"_spr);
        karmaIcon->setPosition({xPos + 20.f, yPos});
        m_mainLayer->addChild(karmaIcon);

        m_fields->m_karmaLabel = CCLabelBMFont::create("0", "bigFont.fnt");
        m_fields->m_karmaLabel->setID("karma-label"_spr);
        m_fields->m_karmaLabel->setPosition({xPos, yPos});
        m_fields->m_karmaLabel->setAnchorPoint({1, 0.5});
        m_fields->m_karmaLabel->limitLabelWidth(100.f, 0.6f, 0.3f);
        m_fields->m_karmaLabel->setVisible(false);
        m_mainLayer->addChild(m_fields->m_karmaLabel);

        m_fields->m_loadingSpinner = LoadingSpinner::create(15.f);
        m_fields->m_loadingSpinner->setID("karma-loading-circle"_spr);
        m_fields->m_loadingSpinner->setPosition({xPos, yPos});
        m_fields->m_loadingSpinner->setAnchorPoint({1, 0.5});
        m_mainLayer->addChild(m_fields->m_loadingSpinner);

        if (myStuffHint) {
            myStuffHint->setVisible(false);
        }

        if (const auto cachedKarma = KarmaCache::getKarma(m_score->m_userID)) {
            showKarma(cachedKarma);
            return;
        }

        const auto glm = GameLevelManager::sharedState();
        m_fields->m_karmaScraper.setCallback([this](CCArray* comments, const bool success) {
            if (success) onCommentsLoaded(comments);
            else {
                KarmaCache::setKarma(m_score->m_userID, m_fields->m_karma);
                showKarma(m_fields->m_karma);
            }
        });
        glm->m_levelCommentDelegate = &m_fields->m_karmaScraper;

        m_fields->m_nrl = Loader::get()->isModLoaded("sorkopiko.noratelimit");

        onCommentsLoaded(comments);
    }

    void calculateKarma() {
        const auto glm = GameLevelManager::sharedState();
        const auto commentKey = glm->getCommentKey(m_score->m_userID, m_fields->m_page, 1, CommentKeyType::User);
        if (const auto comments = glm->getStoredLevelComments(commentKey.c_str()); comments) {
            onCommentsLoaded(comments);
            return;
        }
        if (m_fields->m_nrl) {
            getLevelComments(0.f);
            return;
        }
        if (const auto time = RequestStutter::getRequestTime(); time > 0) {
            this->getScheduler()->scheduleSelector(
                schedule_selector(CKProfilePage::getLevelComments),
                this,
                1,
                0,
                time,
                false
            );
        } else getLevelComments(0);
        glm->getLevelComments(m_score->m_userID, m_fields->m_page, 0, 1, CommentKeyType::User);
    }

    void getLevelComments(float dt) {
        const auto glm = GameLevelManager::sharedState();
        glm->getLevelComments(m_score->m_userID, m_fields->m_page, 0, 1, CommentKeyType::User);
    }

    void onCommentsLoaded(CCArray* comments) {
        for (const auto newArray = CCArrayExt<GJComment*>(comments); const auto comment: newArray) {
            if (!comment) continue;
            if (const auto [fst, snd] = m_fields->m_commentIDs.insert(fmt::format("{}{}", comment->m_commentID, comment->m_levelID)); !snd) continue;
            m_fields->m_karma += comment->m_likeCount;
        }

        ++m_fields->m_page;
        calculateKarma();
    }

    void showKarma(const int karma) {
        m_fields->m_karmaLabel->setString(fmt::format("{}",GameToolbox::intToShortString(karma)).c_str());
        m_fields->m_karmaLabel->setVisible(true);
        if (m_fields->m_loadingSpinner) m_fields->m_loadingSpinner->removeMeAndCleanup();
    }
};