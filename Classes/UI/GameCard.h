#ifndef __GAME_CARD_H__
#define __GAME_CARD_H__

#include "cocos2d.h"
#include "Network/ApiService.h"
#include <functional>

class GameCard : public cocos2d::Node {
public:
    static GameCard* create(const GameCardModel& model, float width, float height);
    virtual bool init(const GameCardModel& model, float width, float height);

    void setOnTap(const std::function<void(const GameCardModel& model)>& callback) { _onTap = callback; }

private:
    GameCardModel _model;
    float _width;
    float _height;
    std::function<void(const GameCardModel& model)> _onTap;
};

#endif // __GAME_CARD_H__
