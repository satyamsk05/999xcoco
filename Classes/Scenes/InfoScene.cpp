#include "Scenes/InfoScene.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

InfoScene* InfoScene::createScene(AppSceneType type) {
    InfoScene* ret = new (std::nothrow) InfoScene();
    if (ret && ret->init(type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool InfoScene::init(AppSceneType type) {
    if (!Scene::init()) return false;
    _type = type;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void InfoScene::setupUI() {
    std::string titleStr = "INFORMATION";
    std::string contentHeader = "999x Game";
    std::string contentBody = "Welcome to 999x Game Platform.\nExperience fair, secure, and instant real-money gaming.";

    switch (_type) {
        case AppSceneType::ABOUT_US:
            titleStr = "ABOUT US";
            contentHeader = "India's Premier Skill Gaming Platform";
            contentBody = "999x provides a world-class, 100% legal, and secure entertainment destination.\n\nAll games comply with applicable skill gaming regulations and standards.";
            break;
        case AppSceneType::CONTACT_US:
            titleStr = "CONTACT US";
            contentHeader = "24/7 Player Support";
            contentBody = "Email: support@999xgame.com\nTelegram: @ingames_support\nWhatsApp: +91 98765 43210\n\nOur support team responds within minutes.";
            break;
        case AppSceneType::FAIR_PLAY:
            titleStr = "FAIR PLAY POLICY";
            contentHeader = "100% Certified RNG & Integrity";
            contentBody = "We employ certified Random Number Generators (RNG) to ensure transparent and unbiased game rounds.\n\nCollusion and automated scripts are strictly prohibited.";
            break;
        case AppSceneType::HELP_CENTRE:
            titleStr = "HELP CENTRE";
            contentHeader = "Frequently Asked Questions";
            contentBody = "Q: How do I deposit cash?\nA: Tap Add Cash, select amount & pay via UPI.\n\nQ: How fast are withdrawals?\nA: Withdrawals to UPI are processed instantly.";
            break;
        case AppSceneType::REPORTED_ISSUES:
            titleStr = "REPORTED ISSUES";
            contentHeader = "Support Tickets";
            contentBody = "No pending issues found.\n\nAll your previous transactions and rounds completed normally.";
            break;
        default: break;
    }

    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont(titleStr, "Arial", 18);
    title->setColor(Color3B::WHITE);
    title->setPosition(Vec2(_designWidth / 2, topY + 30));
    addChild(title, 2);

    auto backListener = EventListenerTouchOneByOne::create();
    backListener->setSwallowTouches(true);
    backListener->onTouchBegan = [this, topY](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= topY && loc.x <= 100) {
            SceneManager::getInstance()->showScene(AppSceneType::SETTINGS);
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backListener, backBtn);

    // Content Card
    float cardW = _designWidth - 32.0f;
    float cardH = 340.0f;
    float cardY = topY - cardH - 20.0f;

    auto card = DrawNode::create();
    card->drawSolidRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + cardH), Color4F(0.15f, 0.02f, 0.22f, 0.95f));
    card->drawRect(Vec2(16, cardY), Vec2(16 + cardW, cardY + cardH), Color4F(0.35f, 0.08f, 0.48f, 1.0f));
    addChild(card, 1);

    auto headLbl = Label::createWithSystemFont(contentHeader, "Arial", 16);
    headLbl->setColor(Color3B(0x00, 0xFF, 0x87));
    headLbl->setPosition(Vec2(_designWidth / 2, cardY + cardH - 40));
    addChild(headLbl, 2);

    auto bodyLbl = Label::createWithSystemFont(contentBody, "Arial", 14);
    bodyLbl->setColor(Color3B::WHITE);
    bodyLbl->setDimensions(cardW - 32, 240);
    bodyLbl->setAnchorPoint(Vec2(0.5f, 1.0f));
    bodyLbl->setPosition(Vec2(_designWidth / 2, cardY + cardH - 75));
    addChild(bodyLbl, 2);
}
