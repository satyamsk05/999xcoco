#ifndef __INFO_SCENE_H__
#define __INFO_SCENE_H__

#include "cocos2d.h"
#include "App/SceneManager.h"
#include <string>

class InfoScene : public cocos2d::Scene {
public:
    static InfoScene* createScene(AppSceneType type);
    virtual bool init(AppSceneType type);

private:
    float _designWidth;
    float _designHeight;
    AppSceneType _type;

    void setupUI();
};

#endif // __INFO_SCENE_H__
