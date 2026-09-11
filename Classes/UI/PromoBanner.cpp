#include "UI/PromoBanner.h"
#include "Theme/AppColors.h"

using namespace cocos2d;

PromoBanner* PromoBanner::create(float width, float height) {
    PromoBanner* ret = new (std::nothrow) PromoBanner();
    if (ret && ret->init(width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PromoBanner::init(float width, float height) {
    if (!Node::init()) return false;
    _width = width;
    _height = height;
    setContentSize(Size(width, height));

    // Card background
    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(0.25f, 0.04f, 0.35f, 0.9f));
    draw->drawRect(Vec2(0, 0), Vec2(width, height), Color4F(0.45f, 0.15f, 0.65f, 1.0f));
    addChild(draw, 0);

    // Banner Sprite
    auto banner = Sprite::create("images/banner.png");
    if (!banner) banner = Sprite::create("images/promo.jpg");
    if (banner) {
        banner->setPosition(Vec2(width / 2, height / 2));
        float scaleX = width / banner->getContentSize().width;
        float scaleY = height / banner->getContentSize().height;
        banner->setScale(std::min(scaleX, scaleY) * 0.96f);
        addChild(banner, 1);
    }

    // Touch listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        if (loc.x >= 0 && loc.x <= _width && loc.y >= 0 && loc.y <= _height) {
            runAction(Sequence::create(
                ScaleTo::create(0.08f, 0.96f),
                ScaleTo::create(0.08f, 1.0f),
                CallFunc::create([this]() {
                    if (_onTap) _onTap();
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
