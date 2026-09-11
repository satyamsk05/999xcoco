#include "Scenes/LoginScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

LoginScene* LoginScene::createScene() {
    return LoginScene::create();
}

bool LoginScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _phoneNumber = "9876543210";
    _otp = "1234";
    _isOtpSent = false;

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void LoginScene::setupUI() {
    // 1. Branding Logo
    float logoY = _designHeight - 120.0f;
    auto appTitle = Label::createWithSystemFont("999x Game", "Arial", 28);
    appTitle->setColor(Color3B(0xFF, 0xD7, 0x00));
    appTitle->setPosition(Vec2(_designWidth / 2, _designHeight - 110.0f));
    addChild(appTitle, 2);

    auto appSub = Label::createWithSystemFont("Play Real Games • Win Real Cash", "Arial", 14);
    appSub->setColor(AppColors::profileSubtext);
    appSub->setPosition(Vec2(_designWidth / 2, logoY - 30));
    addChild(appSub, 2);

    // 2. Mobile Login Box
    float cardW = _designWidth - 32.0f;
    float cardY = logoY - 200.0f;
    auto card = DrawNode::create();
    card->drawSolidRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + 140), Color4F(0.18f, 0.03f, 0.26f, 0.95f));
    card->drawRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + 140), Color4F(0.40f, 0.10f, 0.58f, 1.0f));
    addChild(card, 1);

    auto boxTitle = Label::createWithSystemFont("ENTER MOBILE NUMBER", "Arial", 12);
    boxTitle->setColor(AppColors::profileSubtext);
    boxTitle->setPosition(Vec2(_designWidth / 2, cardY + 115));
    addChild(boxTitle, 2);

    _phoneDisplayLabel = Label::createWithSystemFont("+91 " + _phoneNumber, "Arial", 20);
    _phoneDisplayLabel->setColor(Color3B::WHITE);
    _phoneDisplayLabel->setPosition(Vec2(_designWidth / 2, cardY + 75));
    addChild(_phoneDisplayLabel, 2);

    // Primary Action Button (GET OTP / VERIFY OTP)
    float btnH = 48.0f;
    float btnY = cardY + 15.0f;
    auto actionBtn = DrawNode::create();
    actionBtn->drawSolidRect(Vec2(32, btnY), Vec2(16 + cardW - 16, btnY + btnH), Color4F(0.0f, 0.71f, 0.50f, 1.0f));
    addChild(actionBtn, 2);

    _actionBtnLabel = Label::createWithSystemFont("GET OTP ➔", "Arial", 15);
    _actionBtnLabel->setColor(Color3B::WHITE);
    _actionBtnLabel->setPosition(Vec2(_designWidth / 2, btnY + btnH / 2));
    addChild(_actionBtnLabel, 3);

    // 3. Social / Quick Login Options (Google & Guest)
    float guestBtnY = cardY - 70.0f;
    auto guestBtn = DrawNode::create();
    guestBtn->drawSolidRect(Vec2(16, guestBtnY), Vec2(16 + cardW, guestBtnY + 48), Color4F(0.42f, 0.13f, 0.88f, 1.0f));
    addChild(guestBtn, 1);

    auto guestText = Label::createWithSystemFont("⚡ INSTANT GUEST LOGIN", "Arial", 15);
    guestText->setColor(Color3B::WHITE);
    guestText->setPosition(Vec2(_designWidth / 2, guestBtnY + 24));
    addChild(guestText, 2);

    float gBtnY = guestBtnY - 60.0f;
    auto gBtn = DrawNode::create();
    gBtn->drawSolidRect(Vec2(16, gBtnY), Vec2(16 + cardW, gBtnY + 48), Color4F(0.25f, 0.05f, 0.35f, 0.9f));
    gBtn->drawRect(Vec2(16, gBtnY), Vec2(16 + cardW, gBtnY + 48), Color4F(0.5f, 0.2f, 0.7f, 1.0f));
    addChild(gBtn, 1);

    auto gText = Label::createWithSystemFont("G  SIGN IN WITH GOOGLE", "Arial", 14);
    gText->setColor(Color3B::WHITE);
    gText->setPosition(Vec2(_designWidth / 2, gBtnY + 24));
    addChild(gText, 2);

    // Terms statement
    auto termsLbl = Label::createWithSystemFont("By continuing, you agree to our Terms of Service & Fair Play.", "Arial", 11);
    termsLbl->setColor(Color3B(0x88, 0x77, 0x99));
    termsLbl->setPosition(Vec2(_designWidth / 2, 35));
    addChild(termsLbl, 1);

    // Touch Handling
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, btnY, btnH, guestBtnY, gBtnY, cardW](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());

        // Action Button
        if (loc.y >= btnY && loc.y <= (btnY + btnH) && loc.x >= 32 && loc.x <= (16 + cardW - 16)) {
            if (!_isOtpSent) handleSendOtp();
            else handleVerifyOtp();
            return true;
        }

        // Guest Login
        if (loc.y >= guestBtnY && loc.y <= (guestBtnY + 48) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            handleGuestLogin();
            return true;
        }

        // Google Login
        if (loc.y >= gBtnY && loc.y <= (gBtnY + 48) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            handleGoogleLogin();
            return true;
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LoginScene::handleSendOtp() {
    _isOtpSent = true;
    _phoneDisplayLabel->setString("OTP sent: 1234");
    _phoneDisplayLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _actionBtnLabel->setString("VERIFY & PLAY ➔");
    SceneManager::getInstance()->showToast("OTP sent: 1234", Color3B(0x00, 0xFF, 0x87));
}

void LoginScene::handleVerifyOtp() {
    std::string mockToken = "mock_jwt_token_" + std::to_string(time(nullptr));
    std::string userId = "user_999x_" + std::to_string(rand() % 10000);
    TokenManager::saveSession(mockToken, userId, "Player " + userId.substr(10), _phoneNumber, "avatars/avatar_1.png");

    SceneManager::getInstance()->showToast("Login Successful!", Color3B(0x00, 0xFF, 0x87));
    scheduleOnce([](float dt) {
        SceneManager::getInstance()->showScene(AppSceneType::HOME);
    }, 0.8f, "login_home");
}

void LoginScene::handleGuestLogin() {
    std::string mockToken = "guest_jwt_token_" + std::to_string(time(nullptr));
    std::string userId = "guest_" + std::to_string(1000 + rand() % 9000);
    TokenManager::saveSession(mockToken, userId, "Guest " + userId.substr(6), "", "avatars/avatar_2.png");

    SceneManager::getInstance()->showToast("Logged in as Guest!", Color3B(0x00, 0xFF, 0x87));
    scheduleOnce([](float dt) {
        SceneManager::getInstance()->showScene(AppSceneType::HOME);
    }, 0.8f, "guest_home");
}

void LoginScene::handleGoogleLogin() {
    SceneManager::getInstance()->showToast("Connecting to Google Auth...", Color3B(0x6C, 0x20, 0xE0));
    scheduleOnce([this](float dt) {
        handleGuestLogin();
    }, 1.0f, "google_auth");
}
