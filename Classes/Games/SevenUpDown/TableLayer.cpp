#include "Games/SevenUpDown/TableLayer.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

TableLayer* TableLayer::create(float width, float height) {
    TableLayer* ret = new (std::nothrow) TableLayer();
    if (ret && ret->init(width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TableLayer::init(float width, float height) {
    if (!Node::init()) return false;
    _width = width;
    _height = height;
    setContentSize(Size(width, height));

    // Table Felt Background
    auto felt = DrawNode::create();
    felt->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.08f, 0.02f, 0.15f, 0.98f));
    felt->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.35f, 0.10f, 0.50f, 1.0f));
    addChild(felt, 0);

    float spotW = (width - 32.0f) / 3.0f;
    float spotH = height - 20.0f;
    float y0 = 10.0f;

    // Spot 1: DOWN (2-6) - Left
    float x1 = 10.0f;
    _downArea = DrawNode::create();
    _downArea->drawSolidRect(Vec2(x1, y0), Vec2(x1 + spotW, y0 + spotH), Color4F(0.12f, 0.25f, 0.65f, 0.85f));
    _downArea->drawRect(Vec2(x1, y0), Vec2(x1 + spotW, y0 + spotH), Color4F(0.20f, 0.50f, 1.0f, 1.0f));
    addChild(_downArea, 1);

    auto downTitle = Label::createWithSystemFont("2 - 6", "Arial", 22);
    downTitle->setColor(Color3B(0x29, 0x79, 0xFF));
    downTitle->setPosition(Vec2(x1 + spotW / 2, y0 + spotH - 35));
    addChild(downTitle, 2);

    auto downSubtitle = Label::createWithSystemFont("DOWN", "Arial", 16);
    downSubtitle->setColor(Color3B::WHITE);
    downSubtitle->setPosition(Vec2(x1 + spotW / 2, y0 + spotH - 65));
    addChild(downSubtitle, 2);

    auto downOdds = Label::createWithSystemFont("Pays 2X", "Arial", 12);
    downOdds->setColor(Color3B(0x00, 0xFF, 0x87));
    downOdds->setPosition(Vec2(x1 + spotW / 2, y0 + 40));
    addChild(downOdds, 2);

    _downBetLabel = Label::createWithSystemFont("₹0", "Arial", 13);
    _downBetLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _downBetLabel->setPosition(Vec2(x1 + spotW / 2, y0 + 18));
    addChild(_downBetLabel, 2);

    // Spot 2: LUCKY 7 - Center
    float x2 = x1 + spotW + 6.0f;
    _sevenArea = DrawNode::create();
    _sevenArea->drawSolidRect(Vec2(x2, y0), Vec2(x2 + spotW, y0 + spotH), Color4F(0.55f, 0.40f, 0.05f, 0.85f));
    _sevenArea->drawRect(Vec2(x2, y0), Vec2(x2 + spotW, y0 + spotH), Color4F(1.0f, 0.85f, 0.20f, 1.0f));
    addChild(_sevenArea, 1);

    auto sevenTitle = Label::createWithSystemFont("7", "Arial", 26);
    sevenTitle->setColor(Color3B(0xFF, 0xD7, 0x00));
    sevenTitle->setPosition(Vec2(x2 + spotW / 2, y0 + spotH - 35));
    addChild(sevenTitle, 2);

    auto sevenSubtitle = Label::createWithSystemFont("LUCKY SEVEN", "Arial", 14);
    sevenSubtitle->setColor(Color3B::WHITE);
    sevenSubtitle->setPosition(Vec2(x2 + spotW / 2, y0 + spotH - 65));
    addChild(sevenSubtitle, 2);

    auto sevenOdds = Label::createWithSystemFont("Pays 5X", "Arial", 12);
    sevenOdds->setColor(Color3B(0x00, 0xFF, 0x87));
    sevenOdds->setPosition(Vec2(x2 + spotW / 2, y0 + 40));
    addChild(sevenOdds, 2);

    _sevenBetLabel = Label::createWithSystemFont("₹0", "Arial", 13);
    _sevenBetLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _sevenBetLabel->setPosition(Vec2(x2 + spotW / 2, y0 + 18));
    addChild(_sevenBetLabel, 2);

    // Spot 3: UP (8-12) - Right
    float x3 = x2 + spotW + 6.0f;
    _upArea = DrawNode::create();
    _upArea->drawSolidRect(Vec2(x3, y0), Vec2(x3 + spotW, y0 + spotH), Color4F(0.65f, 0.12f, 0.20f, 0.85f));
    _upArea->drawRect(Vec2(x3, y0), Vec2(x3 + spotW, y0 + spotH), Color4F(1.0f, 0.25f, 0.35f, 1.0f));
    addChild(_upArea, 1);

    auto upTitle = Label::createWithSystemFont("8 - 12", "Arial", 22);
    upTitle->setColor(Color3B(0xFF, 0x17, 0x44));
    upTitle->setPosition(Vec2(x3 + spotW / 2, y0 + spotH - 35));
    addChild(upTitle, 2);

    auto upSubtitle = Label::createWithSystemFont("UP", "Arial", 16);
    upSubtitle->setColor(Color3B::WHITE);
    upSubtitle->setPosition(Vec2(x3 + spotW / 2, y0 + spotH - 65));
    addChild(upSubtitle, 2);

    auto upOdds = Label::createWithSystemFont("Pays 2X", "Arial", 12);
    upOdds->setColor(Color3B(0x00, 0xFF, 0x87));
    upOdds->setPosition(Vec2(x3 + spotW / 2, y0 + 40));
    addChild(upOdds, 2);

    _upBetLabel = Label::createWithSystemFont("₹0", "Arial", 13);
    _upBetLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    _upBetLabel->setPosition(Vec2(x3 + spotW / 2, y0 + 18));
    addChild(_upBetLabel, 2);

    // Touch listener for 3 spots
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, x1, x2, x3, y0, spotW, spotH](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.y >= y0 && loc.y <= (y0 + spotH)) {
            if (loc.x >= x1 && loc.x <= (x1 + spotW)) {
                if (_onSpotClicked) _onSpotClicked(BetSpot::DOWN_2_6);
                return true;
            } else if (loc.x >= x2 && loc.x <= (x2 + spotW)) {
                if (_onSpotClicked) _onSpotClicked(BetSpot::LUCKY_7);
                return true;
            } else if (loc.x >= x3 && loc.x <= (x3 + spotW)) {
                if (_onSpotClicked) _onSpotClicked(BetSpot::UP_8_12);
                return true;
            }
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void TableLayer::updateBetDisplays() {
    auto bm = BetManager::getInstance();
    MoneyPaise downBet = bm->getBetOnSpot(BetSpot::DOWN_2_6);
    MoneyPaise sevenBet = bm->getBetOnSpot(BetSpot::LUCKY_7);
    MoneyPaise upBet = bm->getBetOnSpot(BetSpot::UP_8_12);

    if (_downBetLabel) _downBetLabel->setString(downBet.value > 0 ? downBet.formattedIntegerRupees() : "₹0");
    if (_sevenBetLabel) _sevenBetLabel->setString(sevenBet.value > 0 ? sevenBet.formattedIntegerRupees() : "₹0");
    if (_upBetLabel) _upBetLabel->setString(upBet.value > 0 ? upBet.formattedIntegerRupees() : "₹0");
}

void TableLayer::highlightWinningSpot(int totalSum) {
    resetHighlights();
    DrawNode* target = nullptr;
    if (totalSum >= 2 && totalSum <= 6) target = _downArea;
    else if (totalSum == 7) target = _sevenArea;
    else if (totalSum >= 8 && totalSum <= 12) target = _upArea;

    if (target) {
        target->runAction(RepeatForever::create(Sequence::create(
            FadeTo::create(0.25f, 120),
            FadeTo::create(0.25f, 255),
            nullptr
        )));
    }
}

void TableLayer::resetHighlights() {
    if (_downArea) { _downArea->stopAllActions(); _downArea->setOpacity(255); }
    if (_sevenArea) { _sevenArea->stopAllActions(); _sevenArea->setOpacity(255); }
    if (_upArea) { _upArea->stopAllActions(); _upArea->setOpacity(255); }
}
