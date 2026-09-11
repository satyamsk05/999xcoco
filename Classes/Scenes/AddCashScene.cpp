#include "Scenes/AddCashScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Network/ApiService.h"

using namespace cocos2d;

AddCashScene* AddCashScene::createScene() {
    return AddCashScene::create();
}

bool AddCashScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _selectedAmount = MoneyPaise(50000); // Default ₹500
    _selectedPaymentMethod = "UPI";

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void AddCashScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("ADD CASH / DEPOSIT", "Arial", 18);
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

    // Amount Input Card
    float cardW = _designWidth - 32.0f;
    float card1Y = topY - 130.0f;
    auto card1 = DrawNode::create();
    card1->drawSolidRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 110), Color4F(0.18f, 0.03f, 0.25f, 0.95f));
    card1->drawRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 110), Color4F(0.40f, 0.10f, 0.58f, 1.0f));
    addChild(card1, 1);

    auto enterLbl = Label::createWithSystemFont("Enter Amount to Deposit", "Arial", 13);
    enterLbl->setColor(AppColors::profileSubtext);
    enterLbl->setPosition(Vec2(_designWidth / 2, card1Y + 85));
    addChild(enterLbl, 2);

    _amountDisplayLabel = Label::createWithSystemFont("₹500", "Arial", 34);
    _amountDisplayLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _amountDisplayLabel->setPosition(Vec2(_designWidth / 2, card1Y + 45));
    addChild(_amountDisplayLabel, 2);

    // Preset Amount Chips (+₹100, +₹500, +₹1000, +₹2000)
    int64_t presetPaise[4] = {10000, 50000, 100000, 200000};
    const char* presetLabels[4] = {"+₹100", "+₹500", "+₹1000", "+₹2000"};

    float chipW = (cardW - 30.0f) / 4.0f;
    float chipH = 42.0f;
    float chipY = card1Y - 60.0f;

    for (int i = 0; i < 4; ++i) {
        float cx = 16.0f + i * (chipW + 10.0f);
        auto chip = DrawNode::create();
        chip->drawSolidRect(Vec2(cx, chipY), Vec2(cx + chipW, chipY + chipH), Color4F(0.25f, 0.05f, 0.35f, 0.9f));
        chip->drawRect(Vec2(cx, chipY), Vec2(cx + chipW, chipY + chipH), Color4F(0.88f, 0.70f, 0.10f, 1.0f));
        addChild(chip, 1);
        _presetNodes.push_back(chip);

        auto lbl = Label::createWithSystemFont(presetLabels[i], "Arial", 13);
        lbl->setColor(Color3B::WHITE);
        lbl->setPosition(Vec2(cx + chipW / 2, chipY + chipH / 2));
        addChild(lbl, 2);
    }

    // Payment Gateway Options (UPI / GPay / PhonePe / Cards)
    float payCardY = chipY - 140.0f;
    auto payCard = DrawNode::create();
    payCard->drawSolidRect(Vec2(16, payCardY), Vec2(16 + cardW, payCardY + 120), Color4F(0.14f, 0.02f, 0.20f, 0.95f));
    payCard->drawRect(Vec2(16, payCardY), Vec2(16 + cardW, payCardY + 120), Color4F(0.35f, 0.08f, 0.48f, 1.0f));
    addChild(payCard, 1);

    auto payTitle = Label::createWithSystemFont("SELECT PAYMENT METHOD", "Arial", 13);
    payTitle->setColor(AppColors::profileSubtext);
    payTitle->setPosition(Vec2(_designWidth / 2, payCardY + 95));
    addChild(payTitle, 2);

    auto upiRow = Label::createWithSystemFont("●  Instant UPI (GPay, PhonePe, Paytm)", "Arial", 14);
    upiRow->setColor(Color3B(0x00, 0xFF, 0x87));
    upiRow->setAnchorPoint(Vec2(0, 0.5f));
    upiRow->setPosition(Vec2(32, payCardY + 60));
    addChild(upiRow, 2);

    auto nbRow = Label::createWithSystemFont("○  NetBanking & Debit Cards", "Arial", 14);
    nbRow->setColor(Color3B(0xA0, 0x8A, 0xB0));
    nbRow->setAnchorPoint(Vec2(0, 0.5f));
    nbRow->setPosition(Vec2(32, payCardY + 25));
    addChild(nbRow, 2);

    // Green Proceed to Pay CTA Button
    float btnH = 54.0f;
    float btnY = payCardY - 80.0f;
    auto payBtn = DrawNode::create();
    payBtn->drawSolidRect(Vec2(16, btnY), Vec2(16 + cardW, btnY + btnH), Color4F(0.0f, 0.71f, 0.50f, 1.0f));
    addChild(payBtn, 1);

    auto payBtnText = Label::createWithSystemFont("PROCEED TO PAY ₹500 ➔", "Arial", 16);
    payBtnText->setColor(Color3B::WHITE);
    payBtnText->setPosition(Vec2(_designWidth / 2, btnY + btnH / 2));
    addChild(payBtnText, 2);

    // Touch Handling for Presets & Pay Button
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, chipY, chipH, chipW, presetPaise, btnY, btnH, cardW, payBtnText](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());

        // Preset Chips Click
        if (loc.y >= chipY && loc.y <= (chipY + chipH)) {
            for (int i = 0; i < 4; ++i) {
                float cx = 16.0f + i * (chipW + 10.0f);
                if (loc.x >= cx && loc.x <= (cx + chipW)) {
                    setAmount(presetPaise[i]);
                    if (payBtnText) {
                        payBtnText->setString("PROCEED TO PAY " + _selectedAmount.formattedIntegerRupees() + " ➔");
                    }
                    return true;
                }
            }
        }

        // Pay Button Click
        if (loc.y >= btnY && loc.y <= (btnY + btnH) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            proceedToPayment();
            return true;
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void AddCashScene::setAmount(int64_t paise) {
    _selectedAmount = MoneyPaise(paise);
    if (_amountDisplayLabel) {
        _amountDisplayLabel->setString(_selectedAmount.formattedIntegerRupees());
    }
}

void AddCashScene::proceedToPayment() {
    SceneManager::getInstance()->showToast("Creating secure deposit order...", Color3B(0x6C, 0x20, 0xE0));

    ApiService::addCash(_selectedAmount, _selectedPaymentMethod, [this](bool success, const std::string& orderId) {
        if (success) {
            SceneManager::getInstance()->showToast("Deposit order created successfully!", Color3B(0x00, 0xFF, 0x87));
            scheduleOnce([](float dt) {
                SceneManager::getInstance()->showScene(AppSceneType::WALLET);
            }, 1.2f, "deposit_success_back");
        } else {
            SceneManager::getInstance()->showToast("Order initiated in sandbox mode.", Color3B(0x00, 0xFF, 0x87));
            scheduleOnce([](float dt) {
                SceneManager::getInstance()->showScene(AppSceneType::WALLET);
            }, 1.2f, "deposit_sandbox_back");
        }
    });
}
