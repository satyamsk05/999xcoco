#ifndef __WITHDRAW_SCENE_H__
#define __WITHDRAW_SCENE_H__

#include "cocos2d.h"
#include "Core/MoneyPaise.h"

class WithdrawScene : public cocos2d::Scene {
public:
    static WithdrawScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(WithdrawScene);

private:
    float _designWidth;
    float _designHeight;
    MoneyPaise _winningsBalance;
    MoneyPaise _withdrawAmount;
    std::string _upiId;

    cocos2d::Label* _winningsDisplayLabel;
    cocos2d::Label* _amountLabel;
    cocos2d::Label* _upiLabel;

    void setupUI();
    void submitWithdrawal();
};

#endif // __WITHDRAW_SCENE_H__
