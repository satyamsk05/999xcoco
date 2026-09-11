#include "Scenes/MainScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

MainScene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;

    // Fullscreen Gradient Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();
    loadGamesList();

    return true;
}

void MainScene::onEnter() {
    Scene::onEnter();
    // Safely refresh server data on next frame after OpenGL view is fully active
    scheduleOnce([this](float dt) {
        refreshData();
    }, 0.2f, "main_scene_data_refresh");
}

void MainScene::setupUI() {
    // 1. Top Header
    _topHeader = TopHeader::create(_designWidth);
    _topHeader->setPosition(Vec2(0, _designHeight - 70.0f));
    addChild(_topHeader, 10);

    _topHeader->setOnProfileClick([]() {
        SceneManager::getInstance()->showScene(AppSceneType::PROFILE);
    });

    _topHeader->setOnAddCashClick([]() {
        SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH);
    });

    // 2. Online Ticker
    _ticker = OnlineTicker::create(_designWidth);
    _ticker->setPosition(Vec2(0, _designHeight - 70.0f - 36.0f));
    addChild(_ticker, 9);

    // 3. Scrollable Content Container
    _contentLayer = Node::create();
    _contentLayer->setPosition(Vec2(0, 70.0f));
    addChild(_contentLayer, 1);

    // Promo Banner
    float bannerH = 140.0f;
    float bannerY = _designHeight - 70.0f - 36.0f - 70.0f - bannerH - 15.0f;
    _banner = PromoBanner::create(_designWidth - 24.0f, bannerH);
    _banner->setPosition(Vec2(12.0f, bannerY));
    _contentLayer->addChild(_banner, 2);

    _banner->setOnTap([]() {
        SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH);
    });

    // Games Section Header
    float gamesLabelY = bannerY - 32.0f;
    auto gamesTitle = Label::createWithSystemFont("FEATURED GAMES", "Arial", 16);
    gamesTitle->setColor(Color3B::WHITE);
    gamesTitle->setAnchorPoint(Vec2(0, 0.5f));
    gamesTitle->setPosition(Vec2(16.0f, gamesLabelY));
    _contentLayer->addChild(gamesTitle, 2);

    auto liveBadge = Label::createWithSystemFont("• LIVE NOW", "Arial", 12);
    liveBadge->setColor(Color3B(0x00, 0xFF, 0x87));
    liveBadge->setAnchorPoint(Vec2(0, 0.5f));
    liveBadge->setPosition(Vec2(165.0f, gamesLabelY));
    _contentLayer->addChild(liveBadge, 2);

    // Games Horizontal Container
    _gamesContainer = Node::create();
    _gamesContainer->setPosition(Vec2(0, gamesLabelY - 260.0f));
    _contentLayer->addChild(_gamesContainer, 2);

    // 4. Bottom Navigation Bar (5 tabs)
    _bottomNav = BottomNavBar::create(_designWidth, 0);
    _bottomNav->setPosition(Vec2(0, 0));
    addChild(_bottomNav, 10);

    _bottomNav->setOnItemSelected([this](int index) {
        switch (index) {
            case 0: break; // Already Home
            case 1: SceneManager::getInstance()->showScene(AppSceneType::WALLET); break;
            case 2: SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH); break;
            case 3: SceneManager::getInstance()->showScene(AppSceneType::SHARE); break;
            case 4: SceneManager::getInstance()->showScene(AppSceneType::PROFILE); break;
            default: break;
        }
    });
}

void MainScene::refreshData() {
    ApiService::getUserProfile([this](bool success, const UserProfileModel& profile) {
        _userProfile = profile;
        if (_topHeader) {
            _topHeader->updateUserData(profile.username, profile.avatarPath, profile.balance.total);
        }
    });

    loadGamesList();
}

void MainScene::loadGamesList() {
    ApiService::getGamesList([this](bool success, const std::vector<GameCardModel>& games) {
        _games = games;
        if (!_gamesContainer) return;
        _gamesContainer->removeAllChildren();

        float cardW = 160.0f;
        float cardH = 240.0f;
        float startX = 16.0f;
        float spacing = 175.0f;

        for (size_t i = 0; i < games.size(); ++i) {
            const auto& g = games[i];
            auto card = GameCard::create(g, cardW, cardH);
            card->setPosition(Vec2(startX + i * spacing + cardW / 2, cardH / 2));
            _gamesContainer->addChild(card, 1);

            card->setOnTap([this](const GameCardModel& model) {
                if (model.isAvailable && (model.id == "seven_up_down" || model.id == "7updown")) {
                    SceneManager::getInstance()->showScene(AppSceneType::SEVEN_UP_DOWN);
                } else if (model.isAvailable && model.id == "classic_dice") {
                    SceneManager::getInstance()->showScene(AppSceneType::SEVEN_UP_DOWN);
                } else {
                    showComingSoon(model.title);
                }
            });
        }
    });
}

void MainScene::showComingSoon(const std::string& title) {
    std::string msg = title + " — Coming Soon! 🚀";
    SceneManager::getInstance()->showToast(msg, Color3B(0xFF, 0xC1, 0x07));
}
