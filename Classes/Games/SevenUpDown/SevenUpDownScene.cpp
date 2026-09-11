#include "Games/SevenUpDown/SevenUpDownScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

SevenUpDownScene* SevenUpDownScene::createScene() {
    return SevenUpDownScene::create();
}

bool SevenUpDownScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _userBalance = MoneyPaise(50000); // Default ₹500
    _remainingSeconds = 15;

    // Fullscreen Gradient Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    // 1. Top Bar with Back Button and Balance
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto gameTitle = Label::createWithSystemFont("7 UP DOWN", "Arial", 18);
    gameTitle->setColor(Color3B::WHITE);
    gameTitle->setPosition(Vec2(_designWidth / 2, topY + 30));
    addChild(gameTitle, 2);

    auto balancePill = Label::createWithSystemFont("Bal: " + _userBalance.formatted(), "Arial", 14);
    balancePill->setColor(Color3B(0x00, 0xFF, 0x87));
    balancePill->setAnchorPoint(Vec2(1.0f, 0.5f));
    balancePill->setPosition(Vec2(_designWidth - 15, topY + 30));
    addChild(balancePill, 2);

    // Back Touch
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

    // 2. History Bar
    _historyBar = HistoryBar::create(_designWidth - 20, 28);
    _historyBar->setPosition(Vec2(10, topY - 35));
    addChild(_historyBar, 2);

    // 3. Round Countdown Timer
    float timerY = topY - 75;
    _timerNode = DrawNode::create();
    _timerNode->drawSolidRect(Vec2(_designWidth / 2 - 70, timerY), Vec2(_designWidth / 2 + 70, timerY + 30), Color4F(0.20f, 0.04f, 0.28f, 0.9f));
    _timerNode->drawRect(Vec2(_designWidth / 2 - 70, timerY), Vec2(_designWidth / 2 + 70, timerY + 30), Color4F(0.88f, 0.70f, 0.10f, 1.0f));
    addChild(_timerNode, 2);

    _timerLabel = Label::createWithSystemFont("Betting: 15s", "Arial", 14);
    _timerLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _timerLabel->setPosition(Vec2(_designWidth / 2, timerY + 15));
    addChild(_timerLabel, 3);

    // 4. Dice Layer (Dome & Rolling Dice)
    float diceLayerH = 140.0f;
    float diceLayerY = timerY - diceLayerH - 10;
    _diceLayer = DiceLayer::create(_designWidth - 20, diceLayerH);
    _diceLayer->setPosition(Vec2(10, diceLayerY));
    addChild(_diceLayer, 2);

    // 5. Betting Table Layer
    float tableH = 190.0f;
    float tableY = diceLayerY - tableH - 12;
    _tableLayer = TableLayer::create(_designWidth - 20, tableH);
    _tableLayer->setPosition(Vec2(10, tableY));
    addChild(_tableLayer, 2);

    _tableLayer->setOnSpotClicked([this, balancePill](BetSpot spot) {
        auto bm = BetManager::getInstance();
        if (bm->placeBet(spot, _userBalance)) {
            _tableLayer->updateBetDisplays();
            MoneyPaise totalBet = bm->getTotalBetAmount();
            MoneyPaise remaining = _userBalance - totalBet;
            balancePill->setString("Bal: " + remaining.formatted());
            if (_totalBetLabel) {
                _totalBetLabel->setString("Total Bet: " + totalBet.formatted());
            }
        } else {
            SceneManager::getInstance()->showToast("Insufficient Balance or Betting Closed!", Color3B(0xE5, 0x39, 0x35));
        }
    });

    // 6. Total Bet Info Bar
    float infoY = tableY - 30;
    _totalBetLabel = Label::createWithSystemFont("Total Bet: ₹0", "Arial", 14);
    _totalBetLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _totalBetLabel->setPosition(Vec2(_designWidth / 2, infoY + 15));
    addChild(_totalBetLabel, 2);

    // 7. Chip Selector & Action Buttons
    float chipH = 65.0f;
    float chipY = 15.0f;
    _chipSelector = ChipSelector::create(_designWidth - 20, chipH);
    _chipSelector->setPosition(Vec2(10, chipY));
    addChild(_chipSelector, 2);

    _chipSelector->setOnChipSelected([](int64_t paise) {
        BetManager::getInstance()->selectChip(paise);
    });

    _chipSelector->setOnClearBets([this, balancePill]() {
        BetManager::getInstance()->clearCurrentBets();
        _tableLayer->updateBetDisplays();
        balancePill->setString("Bal: " + _userBalance.formatted());
        if (_totalBetLabel) _totalBetLabel->setString("Total Bet: ₹0");
    });

    _chipSelector->setOnDoubleBets([this, balancePill]() {
        auto bm = BetManager::getInstance();
        bm->doubleBets();
        _tableLayer->updateBetDisplays();
        MoneyPaise totalBet = bm->getTotalBetAmount();
        MoneyPaise remaining = _userBalance - totalBet;
        balancePill->setString("Bal: " + remaining.formatted());
        if (_totalBetLabel) _totalBetLabel->setString("Total Bet: " + totalBet.formatted());
    });

    // Start first game round
    startRound();

    return true;
}

void SevenUpDownScene::startRound() {
    BetManager::getInstance()->reset();
    _tableLayer->resetHighlights();
    _tableLayer->updateBetDisplays();
    _diceLayer->resetDice();
    if (_totalBetLabel) _totalBetLabel->setString("Total Bet: ₹0");

    _remainingSeconds = 15;
    if (_timerLabel) {
        _timerLabel->setString("Betting: 15s");
        _timerLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    }

    schedule(CC_SCHEDULE_SELECTOR(SevenUpDownScene::updateTimer), 1.0f);
}

void SevenUpDownScene::updateTimer(float dt) {
    _remainingSeconds--;
    if (_remainingSeconds > 0) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Betting: %ds", _remainingSeconds);
        if (_timerLabel) {
            _timerLabel->setString(buf);
            if (_remainingSeconds <= 5) {
                _timerLabel->setColor(Color3B(0xFF, 0x17, 0x44)); // Red warning
                _timerNode->runAction(Sequence::create(ScaleTo::create(0.1f, 1.05f), ScaleTo::create(0.1f, 1.0f), nullptr));
            }
        }
    } else {
        unschedule(CC_SCHEDULE_SELECTOR(SevenUpDownScene::updateTimer));
        onBettingClosed();
    }
}

void SevenUpDownScene::onBettingClosed() {
    BetManager::getInstance()->setGamePhase(GamePhase::BETTING_CLOSED);
    if (_timerLabel) {
        _timerLabel->setString("BETS CLOSED!");
        _timerLabel->setColor(Color3B(0xFF, 0x17, 0x44));
    }

    scheduleOnce([this](float dt) {
        executeDiceRoll();
    }, 0.8f, "exec_roll");
}

void SevenUpDownScene::executeDiceRoll() {
    BetManager::getInstance()->setGamePhase(GamePhase::DICE_ROLLING);

    // Authoritative / Simulated dice result
    int d1 = 1 + (rand() % 6);
    int d2 = 1 + (rand() % 6);

    _diceLayer->rollDice(d1, d2, [this](int finalD1, int finalD2, int sum) {
        handleRoundOutcome(finalD1, finalD2, sum);
    });
}

void SevenUpDownScene::handleRoundOutcome(int d1, int d2, int sum) {
    BetManager::getInstance()->setGamePhase(GamePhase::SHOW_RESULT);
    _historyBar->addOutcome(sum);
    _tableLayer->highlightWinningSpot(sum);

    MoneyPaise totalBet = BetManager::getInstance()->getTotalBetAmount();
    MoneyPaise payout = BetManager::getInstance()->calculatePayout(d1, d2);

    _userBalance = _userBalance - totalBet + payout;

    if (totalBet.value > 0) {
        showResultPopup(sum, payout);
    }

    // Schedule next round after 4.5 seconds
    scheduleOnce([this](float dt) {
        startRound();
    }, 4.5f, "next_round_delay");
}

void SevenUpDownScene::showResultPopup(int sum, MoneyPaise payout) {
    if (payout.value > 0) {
        std::string winMsg = "YOU WON " + payout.formatted() + "!";
        SceneManager::getInstance()->showToast(winMsg, Color3B(0x00, 0xFF, 0x87));
    } else {
        SceneManager::getInstance()->showToast("Better Luck Next Time!", Color3B(0xFF, 0x52, 0x52));
    }
}
