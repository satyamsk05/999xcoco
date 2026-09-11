#include "UI/OnlineTicker.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

OnlineTicker* OnlineTicker::create(float width) {
    OnlineTicker* ret = new (std::nothrow) OnlineTicker();
    if (ret && ret->init(width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool OnlineTicker::init(float width) {
    if (!Node::init()) return false;
    _width = width;
    _currentCount = 1428;
    setContentSize(Size(width, 36));

    // Gradient Background (LayerColor / DrawNode)
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(0, 0), Vec2(width, 36), Color4F(0.24f, 0.04f, 0.27f, 0.85f));
    draw->drawRect(Vec2(0, 0), Vec2(width, 36), Color4F(0.31f, 0.06f, 0.43f, 0.6f));
    addChild(draw, 0);

    // Live Pulsating Green Dot
    auto dotDraw = DrawNode::create();
    dotDraw->drawSolidCircle(Vec2(24, 18), 5.0f, 0, 16, Color4F(0.0f, 1.0f, 0.53f, 1.0f)); // #00FF87
    addChild(dotDraw, 2);

    auto pulse = Sequence::create(
        ScaleTo::create(0.8f, 1.3f),
        ScaleTo::create(0.8f, 1.0f),
        nullptr
    );
    dotDraw->runAction(RepeatForever::create(pulse));

    // Ticker Label
    char buf[64];
    snprintf(buf, sizeof(buf), "%d Players Online Playing Real Games", _currentCount);
    _tickerLabel = Label::createWithSystemFont(buf, "Arial", 13);
    _tickerLabel->setColor(Color3B(0x00, 0xFF, 0x87));
    _tickerLabel->setAnchorPoint(Vec2(0, 0.5f));
    _tickerLabel->setPosition(Vec2(38, 18));
    addChild(_tickerLabel, 2);

    scheduleCountVariation();
    return true;
}

void OnlineTicker::setPlayerCount(int count) {
    _currentCount = count;
    char buf[64];
    snprintf(buf, sizeof(buf), "%d Players Online Playing Real Games", _currentCount);
    if (_tickerLabel) _tickerLabel->setString(buf);
}

void OnlineTicker::scheduleCountVariation() {
    schedule([this](float dt) {
        int delta = (rand() % 7) - 3;
        _currentCount = std::max(1100, _currentCount + delta);
        char buf[64];
        snprintf(buf, sizeof(buf), "%d Players Online Playing Real Games", _currentCount);
        if (_tickerLabel) _tickerLabel->setString(buf);
    }, 4.0f, "ticker_update");
}
