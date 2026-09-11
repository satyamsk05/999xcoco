#include "Scenes/SettingsScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

SettingsScene* SettingsScene::createScene() {
    return SettingsScene::create();
}

bool SettingsScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;

    auto def = UserDefault::getInstance();
    _soundEnabled = def->getBoolForKey("setting_sound", true);
    _musicEnabled = def->getBoolForKey("setting_music", true);
    _vibrationEnabled = def->getBoolForKey("setting_vibration", true);

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void SettingsScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("SETTINGS", "Arial", 18);
    title->setColor(Color3B::WHITE);
    title->setPosition(Vec2(_designWidth / 2, topY + 30));
    addChild(title, 2);

    auto backListener = EventListenerTouchOneByOne::create();
    backListener->setSwallowTouches(true);
    backListener->onTouchBegan = [this, topY](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= topY && loc.x <= 100) {
            SceneManager::getInstance()->showScene(AppSceneType::PROFILE);
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backListener, backBtn);

    // 1. Audio & Haptic Toggles Card
    float cardW = _designWidth - 32.0f;
    float card1Y = topY - 170.0f;
    auto card1 = DrawNode::create();
    card1->drawSolidRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 150), Color4F(0.16f, 0.03f, 0.23f, 0.95f));
    card1->drawRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 150), Color4F(0.35f, 0.08f, 0.48f, 1.0f));
    addChild(card1, 1);

    // Sound FX Row
    auto sLbl = Label::createWithSystemFont("Sound Effects (SFX)", "Arial", 15);
    sLbl->setAnchorPoint(Vec2(0, 0.5f));
    sLbl->setPosition(Vec2(32, card1Y + 115));
    addChild(sLbl, 2);

    _soundLabel = Label::createWithSystemFont(_soundEnabled ? "ON" : "OFF", "Arial", 15);
    _soundLabel->setColor(_soundEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
    _soundLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _soundLabel->setPosition(Vec2(16 + cardW - 20, card1Y + 115));
    addChild(_soundLabel, 2);

    // Music Row
    auto mLbl = Label::createWithSystemFont("Background Music (BGM)", "Arial", 15);
    mLbl->setAnchorPoint(Vec2(0, 0.5f));
    mLbl->setPosition(Vec2(32, card1Y + 75));
    addChild(mLbl, 2);

    _musicLabel = Label::createWithSystemFont(_musicEnabled ? "ON" : "OFF", "Arial", 15);
    _musicLabel->setColor(_musicEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
    _musicLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _musicLabel->setPosition(Vec2(16 + cardW - 20, card1Y + 75));
    addChild(_musicLabel, 2);

    // Vibration Row
    auto vLbl = Label::createWithSystemFont("Vibration & Haptics", "Arial", 15);
    vLbl->setAnchorPoint(Vec2(0, 0.5f));
    vLbl->setPosition(Vec2(32, card1Y + 35));
    addChild(vLbl, 2);

    _vibLabel = Label::createWithSystemFont(_vibrationEnabled ? "ON" : "OFF", "Arial", 15);
    _vibLabel->setColor(_vibrationEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
    _vibLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _vibLabel->setPosition(Vec2(16 + cardW - 20, card1Y + 35));
    addChild(_vibLabel, 2);

    // 2. Info / Policy Links Card
    float card2Y = card1Y - 210.0f;
    auto card2 = DrawNode::create();
    card2->drawSolidRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 190), Color4F(0.14f, 0.02f, 0.20f, 0.95f));
    card2->drawRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 190), Color4F(0.31f, 0.06f, 0.43f, 1.0f));
    addChild(card2, 1);

    const char* links[4] = {"Help Centre & FAQs", "Fair Play Policy", "About Us & Licensing", "Contact Customer Support"};
    for (int i = 0; i < 4; ++i) {
        float ly = card2Y + 155 - i * 42;
        auto linkLbl = Label::createWithSystemFont(links[i], "Arial", 14);
        linkLbl->setAnchorPoint(Vec2(0, 0.5f));
        linkLbl->setPosition(Vec2(32, ly));
        addChild(linkLbl, 2);

        auto arrow = Label::createWithSystemFont("›", "Arial", 20);
        arrow->setColor(Color3B(0xFF, 0xD7, 0x00));
        arrow->setAnchorPoint(Vec2(1.0f, 0.5f));
        arrow->setPosition(Vec2(16 + cardW - 20, ly));
        addChild(arrow, 2);
    }

    // 3. Logout Button
    float logY = card2Y - 70.0f;
    auto logBtn = DrawNode::create();
    logBtn->drawSolidRect(Vec2(16, logY), Vec2(16 + cardW, logY + 48), Color4F(0.45f, 0.10f, 0.15f, 0.9f));
    addChild(logBtn, 1);

    auto logText = Label::createWithSystemFont("LOG OUT", "Arial", 14);
    logText->setColor(Color3B::WHITE);
    logText->setPosition(Vec2(_designWidth / 2, logY + 24));
    addChild(logText, 2);

    // App Version
    auto verLbl = Label::createWithSystemFont("999x Game v1.0.0 (Cocos2d-x 4.0 Native Android)", "Arial", 11);
    verLbl->setColor(Color3B(0x88, 0x77, 0x99));
    verLbl->setPosition(Vec2(_designWidth / 2, 25));
    addChild(verLbl, 1);

    // Touch Handling for Toggles & Links & Logout
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, card1Y, card2Y, logY, cardW](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());

        // Toggle rows
        if (loc.x >= 16 && loc.x <= (16 + cardW)) {
            if (loc.y >= (card1Y + 95) && loc.y <= (card1Y + 135)) { toggleSound(); return true; }
            if (loc.y >= (card1Y + 55) && loc.y <= (card1Y + 95)) { toggleMusic(); return true; }
            if (loc.y >= (card1Y + 15) && loc.y <= (card1Y + 55)) { toggleVibration(); return true; }

            // Links
            if (loc.y >= (card2Y + 130) && loc.y <= (card2Y + 175)) { SceneManager::getInstance()->showScene(AppSceneType::HELP_CENTRE); return true; }
            if (loc.y >= (card2Y + 90) && loc.y <= (card2Y + 130)) { SceneManager::getInstance()->showScene(AppSceneType::FAIR_PLAY); return true; }
            if (loc.y >= (card2Y + 50) && loc.y <= (card2Y + 90)) { SceneManager::getInstance()->showScene(AppSceneType::ABOUT_US); return true; }
            if (loc.y >= (card2Y + 10) && loc.y <= (card2Y + 50)) { SceneManager::getInstance()->showScene(AppSceneType::CONTACT_US); return true; }

            // Logout
            if (loc.y >= logY && loc.y <= (logY + 48)) {
                TokenManager::clearSession();
                SceneManager::getInstance()->showToast("Logged out successfully", Color3B(0xFF, 0xD7, 0x00));
                SceneManager::getInstance()->showScene(AppSceneType::LOGIN);
                return true;
            }
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SettingsScene::toggleSound() {
    _soundEnabled = !_soundEnabled;
    UserDefault::getInstance()->setBoolForKey("setting_sound", _soundEnabled);
    UserDefault::getInstance()->flush();
    _soundLabel->setString(_soundEnabled ? "ON" : "OFF");
    _soundLabel->setColor(_soundEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
}

void SettingsScene::toggleMusic() {
    _musicEnabled = !_musicEnabled;
    UserDefault::getInstance()->setBoolForKey("setting_music", _musicEnabled);
    UserDefault::getInstance()->flush();
    _musicLabel->setString(_musicEnabled ? "ON" : "OFF");
    _musicLabel->setColor(_musicEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
}

void SettingsScene::toggleVibration() {
    _vibrationEnabled = !_vibrationEnabled;
    UserDefault::getInstance()->setBoolForKey("setting_vibration", _vibrationEnabled);
    UserDefault::getInstance()->flush();
    _vibLabel->setString(_vibrationEnabled ? "ON" : "OFF");
    _vibLabel->setColor(_vibrationEnabled ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
}
