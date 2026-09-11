#include "UI/TopHeader.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

TopHeader* TopHeader::create(float width) {
    TopHeader* ret = new (std::nothrow) TopHeader();
    if (ret && ret->init(width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TopHeader::init(float width) {
    if (!Node::init()) return false;
    _width = width;
    setContentSize(Size(width, 70));

    // Header Background
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(0, 0), Vec2(width, 70), Color4F(AppColors::backgroundStart4B));
    addChild(draw, 0);

    // Left Avatar Section (Gold ring)
    float avatarRadius = 24.0f;
    Vec2 avatarCenter(36, 35);
    draw->drawSolidCircle(avatarCenter, avatarRadius + 2.0f, 0, 32, Color4F(0.88f, 0.70f, 0.10f, 1.0f)); // Gold border
    draw->drawSolidCircle(avatarCenter, avatarRadius, 0, 32, Color4F(0.15f, 0.05f, 0.25f, 1.0f));

    _avatarSprite = Sprite::create("avatars/avatar_1.png");
    if (_avatarSprite) {
        _avatarSprite->setPosition(avatarCenter);
        float scale = (avatarRadius * 2.0f) / _avatarSprite->getContentSize().width;
        _avatarSprite->setScale(scale);
        addChild(_avatarSprite, 2);
    }

    // User Name & Subtext
    _nameLabel = Label::createWithSystemFont("Player", "Arial", 16);
    _nameLabel->setColor(AppColors::profileName);
    _nameLabel->setAnchorPoint(Vec2(0, 0.5f));
    _nameLabel->setPosition(Vec2(70, 46));
    addChild(_nameLabel, 2);

    _subtextLabel = Label::createWithSystemFont("VIP Member", "Arial", 12);
    _subtextLabel->setColor(AppColors::profileSubtext);
    _subtextLabel->setAnchorPoint(Vec2(0, 0.5f));
    _subtextLabel->setPosition(Vec2(70, 24));
    addChild(_subtextLabel, 2);

    // Right Balance Pill (Width ~150, Height ~40)
    float pillWidth = 145.0f;
    float pillHeight = 38.0f;
    float pillX = width - pillWidth - 16.0f;
    float pillY = 16.0f;

    // Balance Pill Background
    draw->drawSolidRect(Vec2(pillX, pillY), Vec2(pillX + pillWidth, pillY + pillHeight), Color4F(0.12f, 0.02f, 0.20f, 0.95f));
    draw->drawRect(Vec2(pillX, pillY), Vec2(pillX + pillWidth, pillY + pillHeight), Color4F(0.31f, 0.06f, 0.43f, 1.0f));

    // Balance Label
    _balanceLabel = Label::createWithSystemFont("₹0.00", "Arial", 14);
    _balanceLabel->setColor(Color3B::WHITE);
    _balanceLabel->setAnchorPoint(Vec2(0, 0.5f));
    _balanceLabel->setPosition(Vec2(pillX + 10, pillY + pillHeight / 2));
    addChild(_balanceLabel, 2);

    // Green Plus Button on Right of Pill
    float btnSize = 32.0f;
    float btnX = pillX + pillWidth - btnSize - 3.0f;
    float btnY = pillY + 3.0f;
    draw->drawSolidRect(Vec2(btnX, btnY), Vec2(btnX + btnSize, btnY + btnSize), Color4F(0.0f, 0.71f, 0.50f, 1.0f)); // Green

    auto plusLabel = Label::createWithSystemFont("+", "Arial", 22);
    plusLabel->setColor(Color3B::WHITE);
    plusLabel->setPosition(Vec2(btnX + btnSize / 2, btnY + btnSize / 2));
    addChild(plusLabel, 3);

    // Touch Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this, pillX, pillY, pillWidth, pillHeight](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.x >= 0 && loc.x <= 180 && loc.y >= 0 && loc.y <= 70) {
            if (_onProfileClick) _onProfileClick();
            return true;
        }
        if (loc.x >= pillX && loc.x <= (pillX + pillWidth) && loc.y >= pillY && loc.y <= (pillY + pillHeight)) {
            if (_onAddCashClick) _onAddCashClick();
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

static std::string normalizeAvatarPath(const std::string& raw) {
    if (raw.empty()) return "avatars/avatar_1.png";
    std::string p = raw;
    size_t pos = p.find("Assets/Avatar/");
    if (pos != std::string::npos) {
        p.replace(pos, 14, "avatars/");
    }
    pos = p.find("Assets/avatars/");
    if (pos != std::string::npos) {
        p.replace(pos, 15, "avatars/");
    }
    pos = p.find("Avatar/");
    if (pos != std::string::npos) {
        p.replace(pos, 7, "avatars/");
    }
    if (p.rfind("avatar_", 0) == 0) {
        p = "avatars/" + p;
    }
    if (cocos2d::FileUtils::getInstance()->isFileExist(p)) {
        return p;
    }
    return "avatars/avatar_1.png";
}

void TopHeader::updateUserData(const std::string& name, const std::string& avatarPath, const MoneyPaise& balance) {
    if (_nameLabel) _nameLabel->setString(name);
    if (_balanceLabel) _balanceLabel->setString(balance.formatted());
    if (_avatarSprite) {
        std::string safePath = normalizeAvatarPath(avatarPath);
        if (FileUtils::getInstance()->isFileExist(safePath)) {
            _avatarSprite->setTexture(safePath);
        }
    }
}
