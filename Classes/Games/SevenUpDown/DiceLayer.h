#ifndef __DICE_LAYER_H__
#define __DICE_LAYER_H__

#include "cocos2d.h"
#include <functional>

class DiceLayer : public cocos2d::Node {
public:
    static DiceLayer* create(float width, float height);
    virtual bool init(float width, float height);

    void rollDice(int finalDice1, int finalDice2, const std::function<void(int d1, int d2, int sum)>& onComplete);
    void resetDice();

private:
    float _width;
    float _height;

    cocos2d::DrawNode* _dice1Node;
    cocos2d::DrawNode* _dice2Node;
    cocos2d::Label* _dice1Label;
    cocos2d::Label* _dice2Label;
    cocos2d::Label* _sumLabel;

    void drawDiceFace(cocos2d::DrawNode* node, cocos2d::Label* label, int value);
};

#endif // __DICE_LAYER_H__
