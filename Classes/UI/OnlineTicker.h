#ifndef __ONLINE_TICKER_H__
#define __ONLINE_TICKER_H__

#include "cocos2d.h"

class OnlineTicker : public cocos2d::Node {
public:
    static OnlineTicker* create(float width);
    virtual bool init(float width);

    void setPlayerCount(int count);

private:
    float _width;
    cocos2d::Label* _tickerLabel;
    cocos2d::Sprite* _liveDot;
    int _currentCount;

    void startLivePulsing();
    void scheduleCountVariation();
};

#endif // __ONLINE_TICKER_H__
