#include "Games/SevenUpDown/HistoryBar.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

HistoryBar* HistoryBar::create(float width, float height) {
    HistoryBar* ret = new (std::nothrow) HistoryBar();
    if (ret && ret->init(width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool HistoryBar::init(float width, float height) {
    if (!Node::init()) return false;
    _width = width;
    _height = height;
    setContentSize(Size(width, height));

    // Bar Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.12f, 0.02f, 0.18f, 0.9f));
    bg->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.30f, 0.08f, 0.42f, 0.8f));
    addChild(bg, 0);

    auto title = Label::createWithSystemFont("HISTORY:", "Arial", 11);
    title->setColor(AppColors::profileSubtext);
    title->setAnchorPoint(Vec2(0, 0.5f));
    title->setPosition(Vec2(10, height / 2));
    addChild(title, 1);

    _itemsContainer = Node::create();
    _itemsContainer->setPosition(Vec2(75, 0));
    addChild(_itemsContainer, 1);

    // Initial mock history
    _history = {4, 11, 7, 3, 9, 7, 5, 10, 8, 2};
    refreshVisuals();

    return true;
}

void HistoryBar::addOutcome(int sum) {
    _history.insert(_history.begin(), sum);
    if (_history.size() > 12) {
        _history.pop_back();
    }
    refreshVisuals();
}

void HistoryBar::setHistory(const std::vector<int>& history) {
    _history = history;
    refreshVisuals();
}

void HistoryBar::refreshVisuals() {
    if (!_itemsContainer) return;
    _itemsContainer->removeAllChildren();

    float spacing = 26.0f;
    float r = 11.0f;
    float cy = _height / 2;

    for (size_t i = 0; i < _history.size(); ++i) {
        int sum = _history[i];
        float cx = i * spacing;

        Color4F pillColor;
        if (sum >= 2 && sum <= 6) {
            pillColor = Color4F(0.16f, 0.47f, 1.0f, 1.0f); // Blue (Down)
        } else if (sum == 7) {
            pillColor = Color4F(1.0f, 0.84f, 0.0f, 1.0f); // Gold (Seven)
        } else {
            pillColor = Color4F(1.0f, 0.09f, 0.27f, 1.0f); // Red (Up)
        }

        auto node = DrawNode::create();
        node->drawSolidCircle(Vec2(cx, cy), r, 0, 16, pillColor);
        _itemsContainer->addChild(node, 1);

        auto lbl = Label::createWithSystemFont(std::to_string(sum), "Arial", 10);
        lbl->setColor((sum == 7) ? Color3B::BLACK : Color3B::WHITE);
        lbl->setPosition(Vec2(cx, cy));
        _itemsContainer->addChild(lbl, 2);
    }
}
