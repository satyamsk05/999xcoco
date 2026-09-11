#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"

class SettingsScene : public cocos2d::Scene {
public:
    static SettingsScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(SettingsScene);

private:
    float _designWidth;
    float _designHeight;
    bool _soundEnabled;
    bool _musicEnabled;
    bool _vibrationEnabled;

    cocos2d::Label* _soundLabel;
    cocos2d::Label* _musicLabel;
    cocos2d::Label* _vibLabel;

    void setupUI();
    void toggleSound();
    void toggleMusic();
    void toggleVibration();
};

#endif // __SETTINGS_SCENE_H__
