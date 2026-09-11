#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include <string>

class LoginScene : public cocos2d::Scene {
public:
    static LoginScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(LoginScene);

private:
    float _designWidth;
    float _designHeight;
    std::string _phoneNumber;
    std::string _otp;
    bool _isOtpSent;

    cocos2d::Label* _phoneDisplayLabel;
    cocos2d::Label* _actionBtnLabel;

    void setupUI();
    void handleSendOtp();
    void handleVerifyOtp();
    void handleGuestLogin();
    void handleGoogleLogin();
};

#endif // __LOGIN_SCENE_H__
