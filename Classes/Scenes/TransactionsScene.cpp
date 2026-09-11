#include "Scenes/TransactionsScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

TransactionsScene* TransactionsScene::createScene() {
    return TransactionsScene::create();
}

bool TransactionsScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _currentFilter = "All";

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();
    loadTransactions();

    return true;
}

void TransactionsScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("TRANSACTIONS", "Arial", 18);
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

    // Filter Tabs (All, Deposit, Withdrawal, Bets)
    const char* filters[4] = {"All", "Deposit", "Withdraw", "Bets"};
    float tabW = (_designWidth - 32.0f) / 4.0f;
    float tabY = topY - 45.0f;

    for (int i = 0; i < 4; ++i) {
        float tx = 16.0f + i * tabW;
        auto tabDraw = DrawNode::create();
        tabDraw->drawSolidRect(Vec2(tx, tabY), Vec2(tx + tabW - 4, tabY + 32), Color4F(0.20f, 0.04f, 0.28f, 0.9f));
        tabDraw->drawRect(Vec2(tx, tabY), Vec2(tx + tabW - 4, tabY + 32), Color4F(0.45f, 0.15f, 0.65f, 0.8f));
        addChild(tabDraw, 2);

        auto lbl = Label::createWithSystemFont(filters[i], "Arial", 12);
        lbl->setColor((i == 0) ? Color3B(0xFF, 0xD7, 0x00) : Color3B(0xA0, 0x8A, 0xB0));
        lbl->setPosition(Vec2(tx + (tabW - 4) / 2, tabY + 16));
        addChild(lbl, 3);
    }

    // List Container
    _listContainer = Node::create();
    _listContainer->setPosition(Vec2(0, 0));
    addChild(_listContainer, 2);
}

void TransactionsScene::loadTransactions() {
    ApiService::getTransactions(1, 20, [this](bool success, const std::vector<TransactionModel>& txs) {
        _allTransactions = txs;
        renderList();
    });
}

void TransactionsScene::renderList() {
    if (!_listContainer) return;
    _listContainer->removeAllChildren();

    float startY = _designHeight - 160.0f;
    float itemH = 65.0f;
    float cardW = _designWidth - 32.0f;

    for (size_t i = 0; i < _allTransactions.size(); ++i) {
        const auto& t = _allTransactions[i];
        float y = startY - i * (itemH + 10.0f);
        if (y < 20) break; // Keep inside screen

        auto card = DrawNode::create();
        card->drawSolidRect(Vec2(16, y), Vec2(16 + cardW, y + itemH), Color4F(0.14f, 0.02f, 0.20f, 0.95f));
        card->drawRect(Vec2(16, y), Vec2(16 + cardW, y + itemH), Color4F(0.31f, 0.06f, 0.43f, 0.8f));
        _listContainer->addChild(card, 1);

        // Title / Description
        auto desc = Label::createWithSystemFont(t.description, "Arial", 14);
        desc->setColor(Color3B::WHITE);
        desc->setAnchorPoint(Vec2(0, 0.5f));
        desc->setPosition(Vec2(28, y + 44));
        _listContainer->addChild(desc, 2);

        // Date
        auto date = Label::createWithSystemFont(t.createdAt, "Arial", 11);
        date->setColor(AppColors::profileSubtext);
        date->setAnchorPoint(Vec2(0, 0.5f));
        date->setPosition(Vec2(28, y + 20));
        _listContainer->addChild(date, 2);

        // Amount
        bool isPositive = (t.type == "DEPOSIT" || t.type == "BET_WIN");
        std::string amtStr = (isPositive ? "+" : "-") + t.amount.formatted();
        auto amt = Label::createWithSystemFont(amtStr, "Arial", 15);
        amt->setColor(isPositive ? Color3B(0x00, 0xFF, 0x87) : Color3B(0xFF, 0x52, 0x52));
        amt->setAnchorPoint(Vec2(1.0f, 0.5f));
        amt->setPosition(Vec2(16 + cardW - 16, y + 40));
        _listContainer->addChild(amt, 2);

        // Status Badge
        auto status = Label::createWithSystemFont(t.status, "Arial", 10);
        status->setColor(Color3B(0x00, 0xFF, 0x87));
        status->setAnchorPoint(Vec2(1.0f, 0.5f));
        status->setPosition(Vec2(16 + cardW - 16, y + 18));
        _listContainer->addChild(status, 2);
    }
}
