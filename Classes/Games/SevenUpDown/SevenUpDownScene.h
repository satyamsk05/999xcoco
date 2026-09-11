#ifndef __SEVEN_UP_DOWN_SCENE_H__
#define __SEVEN_UP_DOWN_SCENE_H__

#include "cocos2d.h"
#include "Games/SevenUpDown/BetManager.h"
#include "Games/SevenUpDown/TableLayer.h"
#include "Games/SevenUpDown/DiceLayer.h"
#include "Games/SevenUpDown/ChipSelector.h"
#include "Games/SevenUpDown/HistoryBar.h"
#include "UI/TopHeader.h"

class SevenUpDownScene : public cocos2d::Scene {
public:
    static SevenUpDownScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(SevenUpDownScene);

private:
    float _designWidth;
    float _designHeight;

    TopHeader* _topHeader;
    HistoryBar* _historyBar;
    DiceLayer* _diceLayer;
    TableLayer* _tableLayer;
    ChipSelector* _chipSelector;

    cocos2d::Label* _timerLabel;
    cocos2d::Label* _totalBetLabel;
    cocos2d::DrawNode* _timerNode;

    int _remainingSeconds;
    MoneyPaise _userBalance;

    void startRound();
    void updateTimer(float dt);
    void onBettingClosed();
    void executeDiceRoll();
    void handleRoundOutcome(int d1, int d2, int sum);
    void showResultPopup(int sum, MoneyPaise payout);
};

#endif // __SEVEN_UP_DOWN_SCENE_H__
