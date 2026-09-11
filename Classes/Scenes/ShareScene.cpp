#include "Scenes/ShareScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "UI/BottomNavBar.h"

using namespace cocos2d;

ShareScene* ShareScene::createScene() {
    return ShareScene::create();
}

bool ShareScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _referralCode = "INGAMES999";

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void ShareScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("REFER & EARN", "Arial", 18);
    title->setColor(Color3B::WHITE);
    title->setPosition(Vec2(_designWidth / 2, topY + 30));
    addChild(title, 2);

    auto backListener = EventListenerTouchOneByOne::create();
    backListener->setSwallowTouches(true);
    backListener->onTouchBegan = [this, topY](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= topY && loc.x <= 100) {
            SceneManager::getInstance()->showScene(AppSceneType::HOME);
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backListener, backBtn);

    // 1. Referral Artwork
    float artY = topY - 100.0f;
    auto refArt = Sprite::create("images/refercoin.png");
    if (refArt) {
        refArt->setPosition(Vec2(_designWidth / 2, artY));
        refArt->setScale(0.35f);
        addChild(refArt, 1);
    }

    // 2. Referral Code Card
    float cardW = _designWidth - 32.0f;
    float cardY = artY - 140.0f;
    auto codeCard = DrawNode::create();
    codeCard->drawSolidRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + 110), Color4F(0.18f, 0.03f, 0.26f, 0.95f));
    codeCard->drawRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + 110), Color4F(0.88f, 0.70f, 0.10f, 1.0f));
    addChild(codeCard, 1);

    auto refTitle = Label::createWithSystemFont("YOUR UNIQUE REFERRAL CODE", "Arial", 12);
    refTitle->setColor(AppColors::profileSubtext);
    refTitle->setPosition(Vec2(_designWidth / 2, cardY + 85));
    addChild(refTitle, 2);

    auto codeLbl = Label::createWithSystemFont(_referralCode, "Arial", 28);
    codeLbl->setColor(Color3B(0xFF, 0xD7, 0x00));
    codeLbl->setPosition(Vec2(_designWidth / 2, cardY + 45));
    addChild(codeLbl, 2);

    // 3. Share Action Buttons (Copy & WhatsApp)
    float btnW = (cardW - 16.0f) / 2.0f;
    float btnH = 50.0f;
    float btnY = cardY - 70.0f;

    // Copy Code (Purple)
    auto copyBtn = DrawNode::create();
    copyBtn->drawSolidRect(Vec2(16, btnY), Vec2(16 + btnW, btnY + btnH), Color4F(0.42f, 0.13f, 0.88f, 1.0f));
    addChild(copyBtn, 1);

    auto copyText = Label::createWithSystemFont("📋 COPY CODE", "Arial", 14);
    copyText->setColor(Color3B::WHITE);
    copyText->setPosition(Vec2(16 + btnW / 2, btnY + btnH / 2));
    addChild(copyText, 2);

    // WhatsApp Share (Green)
    auto waBtn = DrawNode::create();
    waBtn->drawSolidRect(Vec2(16 + btnW + 16, btnY), Vec2(16 + cardW, btnY + btnH), Color4F(0.0f, 0.71f, 0.50f, 1.0f));
    addChild(waBtn, 1);

    auto waText = Label::createWithSystemFont("💬 WHATSAPP", "Arial", 14);
    waText->setColor(Color3B::WHITE);
    waText->setPosition(Vec2(16 + btnW + 16 + btnW / 2, btnY + btnH / 2));
    addChild(waText, 2);

    // Touch Handling
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, btnY, btnH, btnW, cardW](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= btnY && loc.y <= (btnY + btnH)) {
            if (loc.x >= 16 && loc.x <= (16 + btnW)) {
                copyReferralCode();
                return true;
            } else if (loc.x >= (16 + btnW + 16) && loc.x <= (16 + cardW)) {
                shareOnWhatsApp();
                return true;
            }
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 4. Bottom Nav Bar (Share is index 3)
    auto bottomNav = BottomNavBar::create(_designWidth, 3);
    bottomNav->setPosition(Vec2(0, 0));
    addChild(bottomNav, 10);

    bottomNav->setOnItemSelected([](int index) {
        switch (index) {
            case 0: SceneManager::getInstance()->showScene(AppSceneType::HOME); break;
            case 1: SceneManager::getInstance()->showScene(AppSceneType::WALLET); break;
            case 2: SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH); break;
            case 3: break; // Already Share
            case 4: SceneManager::getInstance()->showScene(AppSceneType::PROFILE); break;
        }
    });
}

void ShareScene::copyReferralCode() {
    SceneManager::getInstance()->showToast("Referral code copied to clipboard!", Color3B(0x00, 0xFF, 0x87));
}

void ShareScene::shareOnWhatsApp() {
    SceneManager::getInstance()->showToast("Opening WhatsApp Share...", Color3B(0x00, 0xFF, 0x87));
}
