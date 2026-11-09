
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr int kFirstLevel = 1;
    constexpr int kLastLevel = 22;

    bool isMainDemon(int id) {
        return id == 14 || id == 18 || id == 20;
    }

    struct Result {
        int levels = 0;
        int changedNormal = 0;
        int changedPractice = 0;
        int coins = 0;
    };

    Result completeAllClassicLevels() {
        Result result;
        auto* levels = GameLevelManager::sharedState();
        auto* stats = GameStatsManager::sharedState();

        if (!levels || !stats)
            return result;

        for (int levelID = kFirstLevel; levelID <= kLastLevel; ++levelID) {
            auto* level = levels->getMainLevel(levelID, true);

            if (!level) {
                log::warn("Missing official level {}", levelID);
                continue;
            }

            ++result.levels;

            if (!stats->hasCompletedMainLevel(levelID)) {
                stats->completedLevel(level);
            }

            if (!stats->hasCompletedStarLevel(level)) {
                stats->completedStarLevel(level);
            }

            if (isMainDemon(levelID) &&
                !stats->hasCompletedDemonLevel(level)) {
                stats->completedDemonLevel(level);
            }

            if (level->getNormalPercent() < 100) {
                level->savePercentage(100, false, 0, 1, true);
                ++result.changedNormal;
            }

            if (level->m_practicePercent < 100) {
                level->savePercentage(100, true, 0, 1, true);
                ++result.changedPractice;
            }

            for (int coin = 1; coin <= 3; ++coin) {
                const char* key = level->getCoinKey(coin);

                if (key && *key && !stats->hasSecretCoin(key)) {
                    stats->storeSecretCoin(key);
                    ++result.coins;
                }
            }

            stats->checkCoinAchievement(level);
        }

        // Save progress using the game's save system.
        if (auto* app = AppDelegate::get()) {
            app->trySaveGame(true);
        }

        return result;
    }
}

class $modify(AllMainMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto* menu = CCMenu::create();
        if (!menu)
            return true;

        auto* button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Main 100%"),
            this,
            menu_selector(AllMainMenuLayer::onCompleteAll)
        );

        if (!button)
            return true;

        menu->addChild(button);
        menu->setPosition({115.f, 34.f});
        this->addChild(menu, 100);
        menu->setID("main-completer-menu");

        return true;
    }

    void onCompleteAll(CCObject*) {
        geode::createQuickPopup(
            "Complete Official Levels?",
            "Set all <cg>22 classic levels</c> to "
            "<cg>100%</c> in Normal and Practice, "
            "and collect all <cy>66 secret coins</c>? "
            "<cr>Changes your save data.</c> "
            "Make a backup first! The Tower is not included.",
            "Cancel",
            "Apply",
            [](auto*, bool confirmed) {
                if (!confirmed)
                    return;

                auto result = completeAllClassicLevels();

                if (!result.levels) {
                    FLAlertLayer::create(
                        "Not available",
                        "Couldn't load the main levels.",
                        "OK"
                    )->show();
                    return;
                }

                log::info(
                    "All Main Completer: {} levels, "
                    "{} normal, {} practice, {} coins",
                    result.levels,
                    result.changedNormal,
                    result.changedPractice,
                    result.coins
                );

                FLAlertLayer::create(
                    "Finished",
                    "Done. Reopen the main level menu "
                    "to refresh your progress. "
                    "Check your save before uploading "
                    "it to the cloud.",
                    "OK"
                )->show();
            }
        );
    }
};
