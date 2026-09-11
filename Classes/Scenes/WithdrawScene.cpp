#include "Scenes/WithdrawScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Network/ApiService.h"

using namespace cocos2d;

WithdrawScene* WithdrawScene::createScene() {
    return WithdrawScene::create();
}

bool WithdrawScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _winningsBalance = MoneyPaise(25000); // Default ₹250
    _withdrawAmount = MoneyPaise(20000);  // Default ₹200
    _upiId = "player@okaxis";

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void WithdrawScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("INSTANT WITHDRAWAL", "Arial", 18);
    title->setColor(Color3B::WHITE);
    title->setPosition(Vec2(_designWidth / 2, topY + 30));
    addChild(title, 2);

    auto backListener = EventListenerTouchOneByOne::create();
    backListener->setSwallowTouches(true);
    backListener->onTouchBegan = [this, topY](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= topY && loc.x <= 100) {
            SceneManager::getInstance()->showScene(AppSceneType::WALLET);
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backListener, backBtn);

    // 1. Withdrawable Winnings Card
    float cardW = _designWidth - 32.0f;
    float card1Y = topY - 120.0f;
    auto card1 = DrawNode::create();
    card1->drawSolidRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 100), Color4F(0.20f, 0.04f, 0.30f, 0.95f));
    card1->drawRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 100), Color4F(0.45f, 0.15f, 0.65f, 1.0f));
    addChild(card1, 1);

    auto winTitle = Label::createWithSystemFont("AVAILABLE WITHDRAWABLE WINNINGS", "Arial", 12);
    winTitle->setColor(AppColors::profileSubtext);
    winTitle->setPosition(Vec2(_designWidth / 2, card1Y + 75));
    addChild(winTitle, 2);

    _winningsDisplayLabel = Label::createWithSystemFont("₹250.00", "Arial", 28);
    _winningsDisplayLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _winningsDisplayLabel->setPosition(Vec2(_designWidth / 2, card1Y + 38));
    addChild(_winningsDisplayLabel, 2);

    // 2. Withdrawal Amount Card
    float card2Y = card1Y - 120.0f;
    auto card2 = DrawNode::create();
    card2->drawSolidRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 100), Color4F(0.15f, 0.02f, 0.22f, 0.95f));
    card2->drawRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 100), Color4F(0.35f, 0.08f, 0.48f, 1.0f));
    addChild(card2, 1);

    auto amtTitle = Label::createWithSystemFont("Withdrawal Amount (Min ₹100)", "Arial", 12);
    amtTitle->setColor(AppColors::profileSubtext);
    amtTitle->setPosition(Vec2(_designWidth / 2, card2Y + 75));
    addChild(amtTitle, 2);

    _amountLabel = Label::createWithSystemFont("₹200", "Arial", 26);
    _amountLabel->setColor(Color3B::WHITE);
    _amountLabel->setPosition(Vec2(_designWidth / 2, card2Y + 38));
    addChild(_amountLabel, 2);

    // 3. UPI ID Card
    float card3Y = card2Y - 110.0f;
    auto card3 = DrawNode::create();
    card3->drawSolidRect(Vec2(16, card3Y), Vec2(16 + cardW, card3Y + 90), Color4F(0.15f, 0.02f, 0.22f, 0.95f));
    card3->drawRect(Vec2(16, card3Y), Vec2(16 + cardW, card3Y + 90), Color4F(0.35f, 0.08f, 0.48f, 1.0f));
    addChild(card3, 1);

    auto upiTitle = Label::createWithSystemFont("Linked UPI ID / VPA", "Arial", 12);
    upiTitle->setColor(AppColors::profileSubtext);
    upiTitle->setPosition(Vec2(_designWidth / 2, card3Y + 68));
    addChild(upiTitle, 2);

    _upiLabel = Label::createWithSystemFont(_upiId, "Arial", 16);
    _upiLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _upiLabel->setPosition(Vec2(_designWidth / 2, card3Y + 32));
    addChild(_upiLabel, 2);

    // 4. Submit Withdrawal Button
    float btnH = 54.0f;
    float btnY = card3Y - 80.0f;
    auto submitBtn = DrawNode::create();
    submitBtn->drawSolidRect(Vec2(16, btnY), Vec2(16 + cardW, btnY + btnH), Color4F(0.42f, 0.13f, 0.88f, 1.0f));
    addChild(submitBtn, 1);

    auto submitText = Label::createWithSystemFont("CONFIRM WITHDRAWAL ➔", "Arial", 16);
    submitText->setColor(Color3B::WHITE);
    submitText->setPosition(Vec2(_designWidth / 2, btnY + btnH / 2));
    addChild(submitText, 2);

    // Touch Handling
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, btnY, btnH, cardW](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= btnY && loc.y <= (btnY + btnH) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            submitWithdrawal();
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void WithdrawScene::submitWithdrawal() {
    if (_withdrawAmount.value < 10000) { // Min ₹100
        SceneManager::getInstance()->showToast("Minimum withdrawal is ₹100", Color3B(0xFF, 0x52, 0x52));
        return;
    }

    if (_withdrawAmount > _winningsBalance) {
        SceneManager::getInstance()->showToast("Amount exceeds available winnings balance!", Color3B(0xFF, 0x52, 0x52));
        return;
    }

    SceneManager::getInstance()->showToast("Processing instant transfer...", Color3B(0x6C, 0x20, 0xE0));

    ApiService::withdrawCash(_withdrawAmount, _upiId, [](bool success, const std::string& msg) {
        SceneManager::getInstance()->showToast(msg, Color3B(0x00, 0xFF, 0x87));
        Director::getInstance()->getScheduler()->schedule([](float dt) {
            SceneManager::getInstance()->showScene(AppSceneType::WALLET);
        }, Director::getInstance(), 1.5f, 0, 0.0f, false, "withdraw_back");
    });
}
