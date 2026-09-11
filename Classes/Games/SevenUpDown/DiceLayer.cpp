#include "Games/SevenUpDown/DiceLayer.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

DiceLayer* DiceLayer::create(float width, float height) {
    DiceLayer* ret = new (std::nothrow) DiceLayer();
    if (ret && ret->init(width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DiceLayer::init(float width, float height) {
    if (!Node::init()) return false;
    _width = width;
    _height = height;
    setContentSize(Size(width, height));

    // Dome Background
    auto dome = DrawNode::create();
    dome->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.12f, 0.03f, 0.18f, 0.95f));
    dome->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.40f, 0.15f, 0.55f, 1.0f));
    addChild(dome, 0);

    float diceSize = 64.0f;
    float d1X = width / 2 - 45.0f;
    float d2X = width / 2 + 45.0f;
    float diceY = height / 2 + 10.0f;

    // Dice 1
    _dice1Node = DrawNode::create();
    _dice1Node->setPosition(Vec2(d1X, diceY));
    addChild(_dice1Node, 1);

    _dice1Label = Label::createWithSystemFont("?", "Arial", 36);
    _dice1Label->setColor(Color3B(0x1F, 0x01, 0x30));
    _dice1Label->setPosition(Vec2(d1X, diceY));
    addChild(_dice1Label, 2);

    // Dice 2
    _dice2Node = DrawNode::create();
    _dice2Node->setPosition(Vec2(d2X, diceY));
    addChild(_dice2Node, 1);

    _dice2Label = Label::createWithSystemFont("?", "Arial", 36);
    _dice2Label->setColor(Color3B(0x1F, 0x01, 0x30));
    _dice2Label->setPosition(Vec2(d2X, diceY));
    addChild(_dice2Label, 2);

    // Sum Label Pill
    _sumLabel = Label::createWithSystemFont("Place Your Bets!", "Arial", 16);
    _sumLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _sumLabel->setPosition(Vec2(width / 2, 22.0f));
    addChild(_sumLabel, 2);

    drawDiceFace(_dice1Node, _dice1Label, 3);
    drawDiceFace(_dice2Node, _dice2Label, 4);

    return true;
}

void DiceLayer::drawDiceFace(DrawNode* node, Label* label, int value) {
    if (!node || !label) return;
    node->clear();
    float s = 28.0f;
    node->drawSolidRect(Vec2(-s, -s), Vec2(s, s), Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    node->drawRect(Vec2(-s, -s), Vec2(s, s), Color4F(0.88f, 0.70f, 0.10f, 1.0f));
    label->setString(std::to_string(value));
}

void DiceLayer::rollDice(int finalDice1, int finalDice2, const std::function<void(int d1, int d2, int sum)>& onComplete) {
    if (_sumLabel) {
        _sumLabel->setString("Rolling Dice...");
        _sumLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
    }

    // Shake and rotate dice
    auto rotate1 = RotateBy::create(0.1f, 90.0f);
    auto rotate2 = RotateBy::create(0.1f, -90.0f);

    _dice1Node->runAction(Repeat::create(rotate1, 15));
    _dice2Node->runAction(Repeat::create(rotate2, 15));

    // Schedule random number flickers
    schedule([this](float dt) {
        int r1 = 1 + (rand() % 6);
        int r2 = 1 + (rand() % 6);
        drawDiceFace(_dice1Node, _dice1Label, r1);
        drawDiceFace(_dice2Node, _dice2Label, r2);
    }, 0.08f, 18, 0.0f, "dice_flicker");

    // Settle on final outcome after 1.6 seconds
    scheduleOnce([this, finalDice1, finalDice2, onComplete](float dt) {
        _dice1Node->setRotation(0);
        _dice2Node->setRotation(0);
        drawDiceFace(_dice1Node, _dice1Label, finalDice1);
        drawDiceFace(_dice2Node, _dice2Label, finalDice2);

        int sum = finalDice1 + finalDice2;
        std::string outcomeStr = "Sum: " + std::to_string(sum) + " (";
        if (sum >= 2 && sum <= 6) outcomeStr += "DOWN 2-6)";
        else if (sum == 7) outcomeStr += "LUCKY 7)";
        else outcomeStr += "UP 8-12)";

        if (_sumLabel) {
            _sumLabel->setString(outcomeStr);
            if (sum == 7) _sumLabel->setColor(Color3B(0xFF, 0xD7, 0x00));
            else if (sum < 7) _sumLabel->setColor(Color3B(0x29, 0x79, 0xFF));
            else _sumLabel->setColor(Color3B(0xFF, 0x17, 0x44));
        }

        if (onComplete) {
            onComplete(finalDice1, finalDice2, sum);
        }
    }, 1.6f, "dice_settle");
}

void DiceLayer::resetDice() {
    if (_sumLabel) {
        _sumLabel->setString("Place Your Bets!");
        _sumLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    }
}
