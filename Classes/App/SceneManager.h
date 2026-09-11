#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>

enum class AppSceneType {
    HOME,
    WALLET,
    ADD_CASH,
    WITHDRAW,
    TRANSACTIONS,
    PROFILE,
    SETTINGS,
    SHARE,
    SEVEN_UP_DOWN,
    LOGIN,
    ABOUT_US,
    CONTACT_US,
    FAIR_PLAY,
    HELP_CENTRE,
    REPORTED_ISSUES
};

class SceneManager {
public:
    static SceneManager* getInstance();

    void init();
    void showScene(AppSceneType type, bool pushToStack = true);
    void popScene();
    void showToast(const std::string& message, const cocos2d::Color3B& color = cocos2d::Color3B(0x6C, 0x20, 0xE0));
    void showModal(cocos2d::Node* contentNode);
    void closeModal();
    void handleBackKey();

    AppSceneType getCurrentSceneType() const { return _currentScene; }

private:
    SceneManager();
    ~SceneManager();

    static SceneManager* _instance;
    AppSceneType _currentScene;
    std::vector<AppSceneType> _historyStack;
    cocos2d::Node* _activeModal;
    cocos2d::EventListenerKeyboard* _keyboardListener;

    cocos2d::Scene* createSceneForType(AppSceneType type);
};

#endif // __SCENE_MANAGER_H__
