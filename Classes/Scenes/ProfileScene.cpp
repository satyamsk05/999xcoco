#include "Scenes/ProfileScene.h"
#include "App/SceneManager.h"
#include "Theme/AppColors.h"
#include "Core/Storage/TokenManager.h"
#include "Network/ApiService.h"
#include "UI/BottomNavBar.h"

using namespace cocos2d;

ProfileScene* ProfileScene::createScene() {
    return ProfileScene::create();
}

bool ProfileScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _designWidth = visibleSize.width;
    _designHeight = visibleSize.height;
    _selectedAvatar = TokenManager::getUserAvatar();

    _avatarList = {
        "avatars/avatar_1.png",
        "avatars/avatar_2.png",
        "avatars/avatar_3.png",
        "avatars/avatar_7.png",
        "avatars/avatar_8.png",
        "avatars/avatar_9.png"
    };

    // Background
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(_designWidth, _designHeight), Color4F(AppColors::backgroundStart4B));
    addChild(bg, 0);

    setupUI();

    return true;
}

void ProfileScene::setupUI() {
    // Top App Bar
    float topY = _designHeight - 65.0f;
    auto topBar = DrawNode::create();
    topBar->drawSolidRect(Vec2(0, topY), Vec2(_designWidth, _designHeight), Color4F(0.12f, 0.02f, 0.20f, 0.98f));
    addChild(topBar, 1);

    auto backBtn = Label::createWithSystemFont("❮ BACK", "Arial", 14);
    backBtn->setColor(Color3B(0xFF, 0xD7, 0x00));
    backBtn->setPosition(Vec2(45, topY + 30));
    addChild(backBtn, 2);

    auto title = Label::createWithSystemFont("USER PROFILE", "Arial", 18);
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

    // 1. Main Profile Card with Avatar Ring
    float cardW = _designWidth - 32.0f;
    float card1Y = topY - 140.0f;
    auto card1 = DrawNode::create();
    card1->drawSolidRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 120), Color4F(0.18f, 0.03f, 0.26f, 0.95f));
    card1->drawRect(Vec2(16, card1Y), Vec2(16 + cardW, card1Y + 120), Color4F(0.40f, 0.10f, 0.58f, 1.0f));
    addChild(card1, 1);

    // Avatar Gold Ring
    Vec2 center(65, card1Y + 60);
    card1->drawSolidCircle(center, 38.0f, 0, 32, Color4F(0.88f, 0.70f, 0.10f, 1.0f)); // Gold Ring
    card1->drawSolidCircle(center, 35.0f, 0, 32, Color4F(0.12f, 0.02f, 0.20f, 1.0f));

    _mainAvatarSprite = Sprite::create(_selectedAvatar);
    if (_mainAvatarSprite) {
        _mainAvatarSprite->setPosition(center);
        _mainAvatarSprite->setScale(70.0f / _mainAvatarSprite->getContentSize().width);
        addChild(_mainAvatarSprite, 2);
    }

    // Name & Phone
    auto nameLbl = Label::createWithSystemFont(TokenManager::getUserName(), "Arial", 18);
    nameLbl->setColor(Color3B::WHITE);
    nameLbl->setAnchorPoint(Vec2(0, 0.5f));
    nameLbl->setPosition(Vec2(120, card1Y + 75));
    addChild(nameLbl, 2);

    auto kycBadge = Label::createWithSystemFont("✔ Verified VIP Player", "Arial", 12);
    kycBadge->setColor(Color3B(0x00, 0xFF, 0x87));
    kycBadge->setAnchorPoint(Vec2(0, 0.5f));
    kycBadge->setPosition(Vec2(120, card1Y + 45));
    addChild(kycBadge, 2);

    // 2. Avatar Selection Grid (6 Avatars)
    float gridY = card1Y - 140.0f;
    auto gridCard = DrawNode::create();
    gridCard->drawSolidRect(Vec2(16, gridY), Vec2(16 + cardW, gridY + 120), Color4F(0.14f, 0.02f, 0.20f, 0.95f));
    gridCard->drawRect(Vec2(16, gridY), Vec2(16 + cardW, gridY + 120), Color4F(0.31f, 0.06f, 0.43f, 1.0f));
    addChild(gridCard, 1);

    auto gridTitle = Label::createWithSystemFont("SELECT YOUR AVATAR", "Arial", 12);
    gridTitle->setColor(AppColors::profileSubtext);
    gridTitle->setPosition(Vec2(_designWidth / 2, gridY + 100));
    addChild(gridTitle, 2);

    float avatarSpacing = (cardW - 40.0f) / 6.0f;
    for (size_t i = 0; i < _avatarList.size(); ++i) {
        float ax = 26.0f + i * avatarSpacing + avatarSpacing / 2;
        float ay = gridY + 45.0f;

        auto avSprite = Sprite::create(_avatarList[i]);
        if (avSprite) {
            avSprite->setPosition(Vec2(ax, ay));
            avSprite->setScale(38.0f / avSprite->getContentSize().width);
            addChild(avSprite, 2);
        }
    }

    // 3. Quick Settings CTA
    float setBtnY = gridY - 60.0f;
    auto setBtn = DrawNode::create();
    setBtn->drawSolidRect(Vec2(16, setBtnY), Vec2(16 + cardW, setBtnY + 45), Color4F(0.25f, 0.05f, 0.35f, 0.9f));
    setBtn->drawRect(Vec2(16, setBtnY), Vec2(16 + cardW, setBtnY + 45), Color4F(0.45f, 0.15f, 0.65f, 0.8f));
    addChild(setBtn, 1);

    auto setText = Label::createWithSystemFont("⚙ APP & AUDIO SETTINGS ➔", "Arial", 14);
    setText->setColor(Color3B(0xFF, 0xD7, 0x00));
    setText->setPosition(Vec2(_designWidth / 2, setBtnY + 22));
    addChild(setText, 2);

    // Touch Handling for Avatars & Settings
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, gridY, avatarSpacing, setBtnY, cardW](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());

        // Check avatar selection
        if (loc.y >= (gridY + 15) && loc.y <= (gridY + 85)) {
            for (size_t i = 0; i < _avatarList.size(); ++i) {
                float ax = 26.0f + i * avatarSpacing + avatarSpacing / 2;
                if (loc.x >= (ax - 20) && loc.x <= (ax + 20)) {
                    selectAvatar(_avatarList[i]);
                    return true;
                }
            }
        }

        // Check Settings Button
        if (loc.y >= setBtnY && loc.y <= (setBtnY + 45) && loc.x >= 16 && loc.x <= (16 + cardW)) {
            SceneManager::getInstance()->showScene(AppSceneType::SETTINGS);
            return true;
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 4. Bottom Nav Bar (Profile is index 4)
    auto bottomNav = BottomNavBar::create(_designWidth, 4);
    bottomNav->setPosition(Vec2(0, 0));
    addChild(bottomNav, 10);

    bottomNav->setOnItemSelected([](int index) {
        switch (index) {
            case 0: SceneManager::getInstance()->showScene(AppSceneType::HOME); break;
            case 1: SceneManager::getInstance()->showScene(AppSceneType::WALLET); break;
            case 2: SceneManager::getInstance()->showScene(AppSceneType::ADD_CASH); break;
            case 3: SceneManager::getInstance()->showScene(AppSceneType::SHARE); break;
            case 4: break; // Already Profile
        }
    });
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

void ProfileScene::selectAvatar(const std::string& path) {
    std::string safePath = normalizeAvatarPath(path);
    _selectedAvatar = safePath;
    TokenManager::setUserAvatar(safePath);
    if (_mainAvatarSprite && FileUtils::getInstance()->isFileExist(safePath)) {
        _mainAvatarSprite->setTexture(safePath);
    }
    SceneManager::getInstance()->showToast("Avatar updated!", Color3B(0x00, 0xFF, 0x87));
}
