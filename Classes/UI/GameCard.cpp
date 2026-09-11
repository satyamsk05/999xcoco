#include "UI/GameCard.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

GameCard* GameCard::create(const GameCardModel& model, float width, float height) {
    GameCard* ret = new (std::nothrow) GameCard();
    if (ret && ret->init(model, width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameCard::init(const GameCardModel& model, float width, float height) {
    if (!Node::init()) return false;
    _model = model;
    _width = width;
    _height = height;
    setContentSize(Size(width, height));
    setAnchorPoint(Vec2(0.5f, 0.5f));

    // Card background
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.16f, 0.02f, 0.23f, 0.95f));
    draw->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.31f, 0.06f, 0.43f, 1.0f)); // #4F106D border
    addChild(draw, 0);

    // Thumbnail Sprite
    auto thumb = Sprite::create(model.imagePath);
    if (!thumb) thumb = Sprite::create("images/7updown.png");
    if (thumb) {
        thumb->setPosition(Vec2(width / 2, height / 2 + 15));
        float maxW = width - 16;
        float maxH = height - 70;
        float s = std::min(maxW / thumb->getContentSize().width, maxH / thumb->getContentSize().height);
        thumb->setScale(s);
        addChild(thumb, 1);
    }

    // LIVE Badge
    if (model.isAvailable) {
        auto liveBadge = DrawNode::create();
        liveBadge->drawSolidRect(Vec2(10, height - 32), Vec2(62, height - 12), Color4F(0.0f, 0.0f, 0.0f, 0.7f));
        liveBadge->drawSolidCircle(Vec2(20, height - 22), 4.0f, 0, 16, Color4F(0.0f, 1.0f, 0.53f, 1.0f));
        addChild(liveBadge, 2);

        auto liveText = Label::createWithSystemFont("LIVE", "Arial", 10);
        liveText->setColor(Color3B(0x00, 0xFF, 0x87));
        liveText->setPosition(Vec2(42, height - 22));
        addChild(liveText, 3);
    } else {
        auto soonBadge = DrawNode::create();
        soonBadge->drawSolidRect(Vec2(10, height - 32), Vec2(90, height - 12), Color4F(0.3f, 0.1f, 0.1f, 0.8f));
        addChild(soonBadge, 2);

        auto soonText = Label::createWithSystemFont("COMING SOON", "Arial", 9);
        soonText->setColor(Color3B(0xFF, 0xAA, 0x00));
        soonText->setPosition(Vec2(50, height - 22));
        addChild(soonText, 3);
    }

    // Title Label
    auto titleLabel = Label::createWithSystemFont(model.title, "Arial", 15);
    titleLabel->setColor(Color3B::WHITE);
    titleLabel->setPosition(Vec2(width / 2, 42));
    addChild(titleLabel, 2);

    // Entry Fee / Prize pill
    auto tagDraw = DrawNode::create();
    tagDraw->drawSolidRect(Vec2(12, 12), Vec2(width - 12, 28), Color4F(0.24f, 0.05f, 0.35f, 0.8f));
    addChild(tagDraw, 2);

    std::string tagStr = "Min: ₹10  |  Prize: 2X";
    auto tagLabel = Label::createWithSystemFont(tagStr, "Arial", 11);
    tagLabel->setColor(Color3B(0x00, 0xE6, 0x76));
    tagLabel->setPosition(Vec2(width / 2, 20));
    addChild(tagLabel, 3);

    // Touch Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.x >= 0 && loc.x <= _width && loc.y >= 0 && loc.y <= _height) {
            runAction(Sequence::create(
                ScaleTo::create(0.08f, 0.94f),
                ScaleTo::create(0.08f, 1.0f),
                CallFunc::create([this]() {
                    if (_onTap) _onTap(_model);
                }),
                nullptr
            ));
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
