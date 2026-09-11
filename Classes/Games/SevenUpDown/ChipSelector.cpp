#include "Games/SevenUpDown/ChipSelector.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

ChipSelector* ChipSelector::create(float width, float height) {
    ChipSelector* ret = new (std::nothrow) ChipSelector();
    if (ret && ret->init(width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ChipSelector::init(float width, float height) {
    if (!Node::init()) return false;
    _width = width;
    _height = height;
    _selectedPaise = 1000; // ₹10
    setContentSize(Size(width, height));

    // Define 6 Chip levels
    _chips.push_back({1000, "10", Color3B(0x00, 0x89, 0x7B)});      // Teal
    _chips.push_back({5000, "50", Color3B(0x1E, 0x88, 0xE5)});      // Blue
    _chips.push_back({10000, "100", Color3B(0x8E, 0x24, 0xAA)});    // Purple
    _chips.push_back({50000, "500", Color3B(0xE5, 0x39, 0x35)});    // Red
    _chips.push_back({100000, "1K", Color3B(0xFB, 0x8C, 0x00)});    // Orange
    _chips.push_back({500000, "5K", Color3B(0x39, 0x49, 0xAB)});    // Indigo

    // Background tray
    auto tray = DrawNode::create();
    tray->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.10f, 0.02f, 0.16f, 0.95f));
    tray->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.35f, 0.10f, 0.48f, 1.0f));
    addChild(tray, 0);

    float chipRadius = 22.0f;
    float startX = 35.0f;
    float spacing = (width - 150.0f) / 5.0f;

    for (size_t i = 0; i < _chips.size(); ++i) {
        float cx = startX + i * spacing;
        float cy = height / 2;

        auto chipNode = DrawNode::create();
        chipNode->setPosition(Vec2(cx, cy));
        addChild(chipNode, 1);
        _chipNodes.push_back(chipNode);

        auto lbl = Label::createWithSystemFont(_chips[i].label, "Arial", 13);
        lbl->setColor(Color3B::WHITE);
        lbl->setPosition(Vec2(cx, cy));
        addChild(lbl, 2);
    }

    // Action Buttons on Right: CLEAR & 2X
    float btnW = 50.0f;
    float btnH = 34.0f;
    float clearX = width - 110.0f;
    float doubleX = width - 55.0f;
    float btnY = height / 2 - btnH / 2;

    auto clearBtn = DrawNode::create();
    clearBtn->drawSolidRect(Vec2(clearX, btnY), Vec2(clearX + btnW, btnY + btnH), Color4F(0.4f, 0.1f, 0.1f, 0.9f));
    addChild(clearBtn, 1);

    auto clearLbl = Label::createWithSystemFont("CLEAR", "Arial", 10);
    clearLbl->setColor(Color3B::WHITE);
    clearLbl->setPosition(Vec2(clearX + btnW / 2, btnY + btnH / 2));
    addChild(clearLbl, 2);

    auto doubleBtn = DrawNode::create();
    doubleBtn->drawSolidRect(Vec2(doubleX, btnY), Vec2(doubleX + btnW, btnY + btnH), Color4F(0.1f, 0.45f, 0.2f, 0.9f));
    addChild(doubleBtn, 1);

    auto doubleLbl = Label::createWithSystemFont("2X", "Arial", 12);
    doubleLbl->setColor(Color3B::WHITE);
    doubleLbl->setPosition(Vec2(doubleX + btnW / 2, btnY + btnH / 2));
    addChild(doubleLbl, 2);

    updateSelectionVisuals();

    // Touch Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, startX, spacing, chipRadius, clearX, doubleX, btnY, btnW, btnH](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        float cy = _height / 2;

        // Check chips
        for (size_t i = 0; i < _chips.size(); ++i) {
            float cx = startX + i * spacing;
            if (loc.distance(Vec2(cx, cy)) <= (chipRadius + 6.0f)) {
                setSelectedPaise(_chips[i].paiseValue);
                if (_onChipSelected) _onChipSelected(_chips[i].paiseValue);
                return true;
            }
        }

        // Check Clear
        if (loc.x >= clearX && loc.x <= (clearX + btnW) && loc.y >= btnY && loc.y <= (btnY + btnH)) {
            if (_onClearBets) _onClearBets();
            return true;
        }

        // Check Double
        if (loc.x >= doubleX && loc.x <= (doubleX + btnW) && loc.y >= btnY && loc.y <= (btnY + btnH)) {
            if (_onDoubleBets) _onDoubleBets();
            return true;
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void ChipSelector::setSelectedPaise(int64_t paise) {
    _selectedPaise = paise;
    updateSelectionVisuals();
}

void ChipSelector::updateSelectionVisuals() {
    float chipRadius = 20.0f;
    for (size_t i = 0; i < _chips.size(); ++i) {
        bool isSelected = (_chips[i].paiseValue == _selectedPaise);
        auto node = _chipNodes[i];
        node->clear();

        Color4F chipColor(_chips[i].color.r / 255.0f, _chips[i].color.g / 255.0f, _chips[i].color.b / 255.0f, 1.0f);
        node->drawSolidCircle(Vec2::ZERO, isSelected ? chipRadius + 3.0f : chipRadius, 0, 32, chipColor);

        if (isSelected) {
            node->drawCircle(Vec2::ZERO, chipRadius + 5.0f, 0, 32, false, Color4F(1.0f, 0.85f, 0.20f, 1.0f)); // Gold Ring
        } else {
            node->drawCircle(Vec2::ZERO, chipRadius, 0, 32, false, Color4F(1.0f, 1.0f, 1.0f, 0.4f));
        }
    }
}
