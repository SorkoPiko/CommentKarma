#ifndef KARMASCRAPER_HPP
#define KARMASCRAPER_HPP

using namespace geode::prelude;

class KarmaScraper : public LevelCommentDelegate {
public:
    void setCallback(const std::function<void(CCArray*, bool)>& callback);

private:
    void loadCommentsFinished(CCArray *comments, char const *key) override;
    void loadCommentsFailed(char const *key) override;

    std::function<void(CCArray*, bool)> m_callback;
};

#endif