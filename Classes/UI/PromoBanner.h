#ifndef __PROMO_BANNER_H__
#define __PROMO_BANNER_H__

#include "cocos2d.h"
#include <functional>

class PromoBanner : public cocos2d::Node {
public:
    static PromoBanner* create(float width, float height);
    virtual bool init(float width, float height);

    void setOnTap(const std::function<void()>& callback) { _onTap = callback; }

private:
    float _width;
    float _height;
    std::function<void()> _onTap;
};

#endif // __PROMO_BANNER_H__
