#ifndef __PROFILE_SCENE_H__
#define __PROFILE_SCENE_H__

#include "cocos2d.h"
#include <string>
#include <vector>

class ProfileScene : public cocos2d::Scene {
public:
    static ProfileScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(ProfileScene);

private:
    float _designWidth;
    float _designHeight;
    std::string _selectedAvatar;
    cocos2d::Sprite* _mainAvatarSprite;
    std::vector<std::string> _avatarList;

    void setupUI();
    void selectAvatar(const std::string& path);
};

#endif // __PROFILE_SCENE_H__
