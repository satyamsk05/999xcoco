#include "AppDelegate.h"
#include "App/SceneManager.h"
#include "Scenes/MainScene.h"
#include "Scenes/LoginScene.h"
#include "Core/Storage/TokenManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(390, 844); // Standard modern mobile portrait viewport

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("999x Game", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("999x Game");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    // Set portrait design resolution with FIXED_WIDTH for responsive Android aspect ratios
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

    register_all_packages();

    // Initialize Navigation & Session
    SceneManager::getInstance()->init();

    // Launch MainScene
    auto scene = MainScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
