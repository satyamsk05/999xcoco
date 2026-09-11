#ifndef __WALLET_SCENE_H__
#define __WALLET_SCENE_H__

#include "cocos2d.h"
#include "Core/MoneyPaise.h"

class WalletScene : public cocos2d::Scene {
public:
    static WalletScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(WalletScene);

private:
    float _designWidth;
    float _designHeight;
    WalletBalance _balance;

    cocos2d::Label* _totalBalanceLabel;
    cocos2d::Label* _depositBalanceLabel;
    cocos2d::Label* _winningBalanceLabel;
    cocos2d::Label* _bonusBalanceLabel;

    void setupUI();
    void refreshBalance();
};

#endif // __WALLET_SCENE_H__
