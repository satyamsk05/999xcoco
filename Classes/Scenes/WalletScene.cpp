#include "Scenes/WalletScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Network/ApiService.h"
#include "UI/BottomNavBar.h"

using namespace cocos2d;

WalletScene* WalletScene::createScene() {
    return WalletScene::create();
}

bool WalletScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();
    refreshBalance();

    return true;
}

void WalletScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("WALLET & BALANCE", "Arial", 18);
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

    // 1. Total Balance Card
    float cardW = _designWidth - 32.0f;
    float card1Y = topY - 140.0f;
    auto card1 = DrawNode::create();
    card1->drawSolidRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 120), Color4F(0.24f, 0.04f, 0.35f, 0.95f));
    card1->drawRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 120), Color4F(0.45f, 0.15f, 0.65f, 1.0f));
    addChild(card1, 1);

    auto totalTitle = Label::createWithSystemFont("TOTAL BALANCE", "Arial", 13);
    totalTitle->setColor(AppColors::profileSubtext);
    totalTitle->setPosition(Vec2(_designWidth / 2, card1Y + 95));
    addChild(totalTitle, 2);

    _totalBalanceLabel = Label::createWithSystemFont("₹0.00", "Arial", 32);
    _totalBalanceLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _totalBalanceLabel->setPosition(Vec2(_designWidth / 2, card1Y + 55));
    addChild(_totalBalanceLabel, 2);

    // 2. Balance Breakdown (Deposit, Winnings, Bonus)
    float card2Y = card1Y - 160.0f;
    auto card2 = DrawNode::create();
    card2->drawSolidRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 140), Color4F(0.15f, 0.02f, 0.22f, 0.95f));
    card2->drawRect(Vec2(16, card2Y), Vec2(16 + cardW, card2Y + 140), Color4F(0.31f, 0.06f, 0.43f, 1.0f));
    addChild(card2, 1);

    // Deposit Row
    auto depLbl = Label::createWithSystemFont("Deposit Balance:", "Arial", 14);
    depLbl->setAnchorPoint(Vec2(0, 0.5f));
    depLbl->setPosition(Vec2(32, card2Y + 105));
    addChild(depLbl, 2);

    _depositBalanceLabel = Label::createWithSystemFont("₹0.00", "Arial", 15);
    _depositBalanceLabel->setColor(Color3B::WHITE);
    _depositBalanceLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _depositBalanceLabel->setPosition(Vec2(16 + cardW - 16, card2Y + 105));
    addChild(_depositBalanceLabel, 2);

    // Winnings Row
    auto winLbl = Label::createWithSystemFont("Winnings (Withdrawable):", "Arial", 14);
    winLbl->setAnchorPoint(Vec2(0, 0.5f));
    winLbl->setPosition(Vec2(32, card2Y + 70));
    addChild(winLbl, 2);

    _winningBalanceLabel = Label::createWithSystemFont("₹0.00", "Arial", 15);
    _winningBalanceLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _winningBalanceLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _winningBalanceLabel->setPosition(Vec2(16 + cardW - 16, card2Y + 70));
    addChild(_winningBalanceLabel, 2);

    // Bonus Row
    auto bonLbl = Label::createWithSystemFont("Bonus / Rewards:", "Arial", 14);
    bonLbl->setAnchorPoint(Vec2(0, 0.5f));
    bonLbl->setPosition(Vec2(32, card2Y + 35));
    addChild(bonLbl, 2);

    _bonusBalanceLabel = Label::createWithSystemFont("₹0.00", "Arial", 15);
    _bonusBalanceLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _bonusBalanceLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
    _bonusBalanceLabel->setPosition(Vec2(16 + cardW - 16, card2Y + 35));
    addChild(_bonusBalanceLabel, 2);

    // 3. Action Buttons (ADD CASH & WITHDRAW)
    float btnW = (cardW - 16.0f) / 2.0f;
    float btnH = 50.0f;
    float btnY = card2Y - 70.0f;

    // Add Cash (Green)
    auto addBtn = DrawNode::create();
    addBtn->drawSolidRect(Vec2(16, btnY), Vec2(16 + btnW, btnY + btnH), Color4F(0.0f, 0.71f, 0.50f, 1.0f));
    addChild(addBtn, 1);

    auto addText = Label::createWithSystemFont("+ ADD CASH", "Arial", 15);
    addText->setColor(Color3B::WHITE);
    addText->setPosition(Vec2(16 + btnW / 2, btnY + btnH / 2));
    addChild(addText, 2);

    // Withdraw (Purple)
    auto withBtn = DrawNode::create();
    withBtn->drawSolidRect(Vec2(16 + btnW + 16, btnY), Vec2(16 + cardW, btnY + btnH), Color4F(0.42f, 0.13f, 0.88f, 1.0f));
    addChild(withBtn, 1);

    auto withText = Label::createWithSystemFont("WITHDRAW", "Arial", 15);
    withText->setColor(Color3B::WHITE);
    withText->setPosition(Vec2(16 + btnW + 16 + btnW / 2, btnY + btnH / 2));
    addChild(withText, 2);

    // Transactions History CTA
    float txBtnY = btnY - 70.0f;
    auto txBtn = DrawNode::create();
    txBtn->drawSolidRect(Vec2(16, txBtnY), Vec2(16 + cardW, txBtnY + 45), Color4F(0.18f, 0.04f, 0.25f, 0.9f));
    txBtn->drawRect(Vec2(16, txBtnY), Vec2(16 + cardW, txBtnY + 45), Color4F(0.4f, 0.15f, 0.55f, 0.8f));
    addChild(txBtn, 1);

    auto txText = Label::createWithSystemFont("VIEW TRANSACTION HISTORY ➔", "Arial", 13);
    txText->setColor(Color3B(0xFF, 0xD7, 0x00));
    txText->setPosition(Vec2(_designWidth / 2, txBtnY + 22));
    addChild(txText, 2);

    // Touch Listeners
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this, btnY, btnW, btnH, cardW, txBtnY](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        // Add Cash Click
        if (loc.y >= btnY && loc.y <= (btnY + btnH) && loc.x >= 16 && loc.x <= (16 + btnW)) {
            SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH);
            return true;
        }
        // Withdraw Click
        if (loc.y >= btnY && loc.y <= (btnY + btnH) && loc.x >= (16 + btnW + 16) && loc.x <= (16 + cardW)) {
            SceneManager::getInstance()->showScene(AppSceneType::WITHDRAW);
            return true;
        }
        // Transactions Click
        if (loc.y >= txBtnY && loc.y <= (txBtnY + 45) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            SceneManager::getInstance()->showScene(AppSceneType::TRANSACTIONS);
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // 4. Bottom Nav Bar (Wallet is index 1)
    auto bottomNav = BottomNavBar::create(_designWidth, 1);
    bottomNav->setPosition(Vec2(0, 0));
    addChild(bottomNav, 10);

    bottomNav->setOnItemSelected([](int index) {
        switch (index) {
            case 0: SceneManager::getInstance()->showScene(AppSceneType::HOME); break;
            case 1: break; // Already Wallet
            case 2: SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH); break;
            case 3: SceneManager::getInstance()->showScene(AppSceneType::SHARE); break;
            case 4: SceneManager::getInstance()->showScene(AppSceneType::PROFILE); break;
        }
    });
}

void WalletScene::refreshBalance() {
    ApiService::getUserProfile([this](bool success, const UserProfileModel& profile) {
        _balance = profile.balance;
        if (_totalBalanceLabel) _totalBalanceLabel->setString(_balance.total.formatted());
        if (_depositBalanceLabel) _depositBalanceLabel->setString(_balance.deposit.formatted());
        if (_winningBalanceLabel) _winningBalanceLabel->setString(_balance.winning.formatted());
        if (_bonusBalanceLabel) _bonusBalanceLabel->setString(_balance.bonus.formatted());
    });
}
