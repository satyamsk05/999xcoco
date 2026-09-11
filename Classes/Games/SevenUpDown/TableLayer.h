#ifndef __TABLE_LAYER_H__
#define __TABLE_LAYER_H__

#include "cocos2d.h"
#include "Games/SevenUpDown/BetManager.h"
#include <functional>

class TableLayer : public cocos2d::Node {
public:
    static TableLayer* create(float width, float height);
    virtual bool init(float width, float height);

    void updateBetDisplays();
    void highlightWinningSpot(int totalSum);
    void resetHighlights();
    void setOnSpotClicked(const std::function<void(BetSpot spot)>& callback) { _onSpotClicked = callback; }

private:
    float _width;
    float _height;
    std::function<void(BetSpot spot)> _onSpotClicked;

    cocos2d::DrawNode* _downArea;
    cocos2d::DrawNode* _sevenArea;
    cocos2d::DrawNode* _upArea;

    cocos2d::Label* _downBetLabel;
    cocos2d::Label* _sevenBetLabel;
    cocos2d::Label* _upBetLabel;
};

#endif // __TABLE_LAYER_H__
