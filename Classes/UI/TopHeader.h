#ifndef __TOP_HEADER_H__
#define __TOP_HEADER_H__

#include "cocos2d.h"
#include "Core/MoneyPaise.h"
#include <functional>

class TopHeader : public cocos2d::Node {
public:
    static TopHeader* create(float width);
    virtual bool init(float width);

    void updateUserData(const std::string& name, const std::string& avatarPath, const MoneyPaise& balance);
    void setOnProfileClick(const std::function<void()>& callback) { _onProfileClick = callback; }
    void setOnAddCashClick(const std::function<void()>& callback) { _onAddCashClick = callback; }

private:
    float _width;
    cocos2d::Sprite* _avatarSprite;
    cocos2d::Label* _nameLabel;
    cocos2d::Label* _subtextLabel;
    cocos2d::Label* _balanceLabel;
    std::function<void()> _onProfileClick;
    std::function<void()> _onAddCashClick;
};

#endif // __TOP_HEADER_H__
