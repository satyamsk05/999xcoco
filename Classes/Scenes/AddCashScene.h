#ifndef __ADD_CASH_SCENE_H__
#define __ADD_CASH_SCENE_H__

#include "cocos2d.h"
#include "Core/MoneyPaise.h"
#include <vector>

class AddCashScene : public cocos2d::Scene {
public:
    static AddCashScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(AddCashScene);

private:
    float _designWidth;
    float _designHeight;
    MoneyPaise _selectedAmount;
    std::string _selectedPaymentMethod;

    cocos2d::Label* _amountDisplayLabel;
    std::vector<cocos2d::DrawNode*> _presetNodes;

    void setupUI();
    void setAmount(int64_t paise);
    void proceedToPayment();
};

#endif // __ADD_CASH_SCENE_H__
