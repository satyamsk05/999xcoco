#include "UI/BottomNavBar.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

BottomNavBar* BottomNavBar::create(float width, int selectedIndex) {
    BottomNavBar* ret = new (std::nothrow) BottomNavBar();
    if (ret && ret->init(width, selectedIndex)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool BottomNavBar::init(float width, int selectedIndex) {
    if (!Node::init()) return false;
    _width = width;
    _selectedIndex = selectedIndex;
    setContentSize(Size(width, 70));

    // Nav Bar Background
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(0, 0), Vec2(width, 70), Color4F(0.20f, 0.02f, 0.28f, 0.98f));
    draw->drawLine(Vec2(0, 70), Vec2(width, 70), Color4F(0.33f, 0.07f, 0.44f, 1.0f));
    addChild(draw, 0);

    // 5 Nav Items
    const char* titles[5] = {"Home", "Wallet", "Add Cash", "Share", "Profile"};
    const char* iconPaths[5] = {
        "icons/home_solid.png",
        "icons/credit_card_solid.png",
        "icons/wallet_add.png",
        "icons/earn_points.png",
        "icons/user_circle.png"
    };

    float itemWidth = width / 5.0f;
    for (int i = 0; i < 5; ++i) {
        auto itemNode = Node::create();
        itemNode->setContentSize(Size(itemWidth, 70));
        itemNode->setPosition(Vec2(i * itemWidth, 0));
        addChild(itemNode, 1);

        // Center item (Add Cash) is floating / emphasized
        if (i == 2) {
            auto centerCircle = DrawNode::create();
            centerCircle->drawSolidCircle(Vec2(itemWidth / 2, 42), 24.0f, 0, 32, Color4F(0.0f, 0.71f, 0.50f, 1.0f));
            centerCircle->drawCircle(Vec2(itemWidth / 2, 42), 25.0f, 0, 32, false, Color4F(0.88f, 0.70f, 0.10f, 1.0f));
            itemNode->addChild(centerCircle, 1);
        }

        auto icon = Sprite::create(iconPaths[i]);
        if (icon) {
            float iconY = (i == 2) ? 42.0f : 44.0f;
            icon->setPosition(Vec2(itemWidth / 2, iconY));
            icon->setScale(0.8f);
            itemNode->addChild(icon, 2);
        }

        auto label = Label::createWithSystemFont(titles[i], "Arial", 11);
        label->setPosition(Vec2(itemWidth / 2, 16));
        itemNode->addChild(label, 2);

        NavItemUI uiItem;
        uiItem.container = itemNode;
        uiItem.icon = icon;
        uiItem.label = label;
        _items.push_back(uiItem);
    }

    updateSelectionVisuals();

    // Touch listener for 5 items
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, itemWidth](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.x >= 0 && loc.x <= _width && loc.y >= 0 && loc.y <= 70) {
            int clickedIndex = std::min(4, static_cast<int>(loc.x / itemWidth));
            setSelectedIndex(clickedIndex);
            if (_onItemSelected) {
                _onItemSelected(clickedIndex);
            }
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void BottomNavBar::setSelectedIndex(int index) {
    _selectedIndex = index;
    updateSelectionVisuals();
}

void BottomNavBar::updateSelectionVisuals() {
    for (size_t i = 0; i < _items.size(); ++i) {
        bool isSelected = (static_cast<int>(i) == _selectedIndex);
        if (_items[i].label) {
            _items[i].label->setColor(isSelected ? AppColors::navActive : AppColors::navInactive);
        }
        if (_items[i].icon) {
            _items[i].icon->setColor(isSelected ? AppColors::navActive : AppColors::navInactive);
            _items[i].icon->setScale(isSelected ? 0.95f : 0.8f);
        }
    }
}
