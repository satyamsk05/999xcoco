#ifndef __SHARE_SCENE_H__
#define __SHARE_SCENE_H__

#include "cocos2d.h"

class ShareScene : public cocos2d::Scene {
public:
    static ShareScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(ShareScene);

private:
    float _designWidth;
    float _designHeight;
    std::string _referralCode;

    void setupUI();
    void copyReferralCode();
    void shareOnWhatsApp();
};

#endif // __SHARE_SCENE_H__
