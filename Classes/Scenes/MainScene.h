#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "UI/TopHeader.h"
#include "UI/OnlineTicker.h"
#include "UI/PromoBanner.h"
#include "UI/BottomNavBar.h"
#include "UI/GameCard.h"
#include "Network/ApiService.h"
#include <vector>

class MainScene : public cocos2d::Scene {
public:
    static MainScene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(MainScene);

    void refreshData();

private:
    float _designWidth;
    float _designHeight;

    TopHeader* _topHeader;
    OnlineTicker* _ticker;
    PromoBanner* _banner;
    BottomNavBar* _bottomNav;
    cocos2d::Node* _contentLayer;
    cocos2d::Node* _gamesContainer;

    UserProfileModel _userProfile;
    std::vector<GameCardModel> _games;

    void setupUI();
    void loadGamesList();
    void showComingSoon(const std::string& title);
};

#endif // __MAIN_SCENE_H__
