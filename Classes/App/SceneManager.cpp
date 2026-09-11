#include "App/SceneManager.h"
#include "Scenes/MainScene.h"
#include "Scenes/WalletScene.h"
#include "Scenes/AddCashScene.h"
#include "Scenes/WithdrawScene.h"
#include "Scenes/TransactionsScene.h"
#include "Scenes/ProfileScene.h"
#include "Scenes/SettingsScene.h"
#include "Scenes/ShareScene.h"
#include "Scenes/LoginScene.h"
#include "Scenes/InfoScene.h"
#include "Games/SevenUpDown/SevenUpDownScene.h"
#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

SceneManager* SceneManager::_instance = nullptr;

SceneManager* SceneManager::getInstance() {
    if (!_instance) {
        _instance = new SceneManager();
    }
    return _instance;
}

SceneManager::SceneManager()
    : _currentScene(AppSceneType::HOME),
      _activeModal(nullptr),
      _keyboardListener(nullptr) {
}

SceneManager::~SceneManager() {
}

void SceneManager::init() {
    TokenManager::init();

    // Android Hardware Back Key Listener
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            handleBackKey();
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_keyboardListener, 1);
}

void SceneManager::showScene(AppSceneType type, bool pushToStack) {
    if (pushToStack && _currentScene != type) {
        _historyStack.push_back(_currentScene);
    }
    _currentScene = type;

    Scene* scene = createSceneForType(type);
    if (scene) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene, Color3B(0x1F, 0x01, 0x30)));
    }
}

#include <chrono>

void SceneManager::popScene() {
    if (!_historyStack.empty()) {
        AppSceneType prev = _historyStack.back();
        _historyStack.pop_back();
        showScene(prev, false);
    } else {
        if (_currentScene != AppSceneType::HOME) {
            showScene(AppSceneType::HOME, false);
        } else {
            static long long lastBackTime = 0;
            long long now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            if (now - lastBackTime < 2000 && lastBackTime != 0) {
                Director::getInstance()->end();
            } else {
                lastBackTime = now;
                showToast("Press back again to exit", Color3B(0xFF, 0xC1, 0x07));
            }
        }
    }
}

void SceneManager::handleBackKey() {
    if (_activeModal) {
        closeModal();
        return;
    }
    popScene();
}

void SceneManager::showToast(const std::string& message, const Color3B& color) {
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float toastW = std::min(visibleSize.width - 40.0f, 320.0f);
    float toastH = 44.0f;

    auto toastNode = Node::create();
    toastNode->setPosition(Vec2(visibleSize.width / 2, 110.0f));
    runningScene->addChild(toastNode, 1000);

    auto draw = DrawNode::create();
    draw->drawSolidRect(Vec2(-toastW / 2, -toastH / 2), Vec2(toastW / 2, toastH / 2), Color4F(0.12f, 0.02f, 0.18f, 0.95f));
    draw->drawRect(Vec2(-toastW / 2, -toastH / 2), Vec2(toastW / 2, toastH / 2), Color4F(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
    toastNode->addChild(draw, 0);

    auto label = Label::createWithSystemFont(message, "Arial", 13);
    label->setColor(color);
    label->setPosition(Vec2::ZERO);
    toastNode->addChild(label, 1);

    toastNode->setScale(0.8f);
    toastNode->setOpacity(0);
    toastNode->runAction(Sequence::create(
        Spawn::create(ScaleTo::create(0.15f, 1.0f), FadeIn::create(0.15f), nullptr),
        DelayTime::create(2.0f),
        Spawn::create(ScaleTo::create(0.15f, 0.8f), FadeOut::create(0.15f), nullptr),
        RemoveSelf::create(),
        nullptr
    ));
}

void SceneManager::showModal(Node* contentNode) {
    if (!contentNode) return;
    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) return;

    closeModal();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _activeModal = Node::create();
    _activeModal->setPosition(Vec2::ZERO);
    runningScene->addChild(_activeModal, 999);

    auto overlay = DrawNode::create();
    overlay->drawSolidRect(Vec2::ZERO, Vec2(visibleSize.width, visibleSize.height), Color4F(0.0f, 0.0f, 0.0f, 0.7f));
    _activeModal->addChild(overlay, 0);

    contentNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _activeModal->addChild(contentNode, 1);
}

void SceneManager::closeModal() {
    if (_activeModal) {
        _activeModal->removeFromParentAndCleanup(true);
        _activeModal = nullptr;
    }
}

Scene* SceneManager::createSceneForType(AppSceneType type) {
    switch (type) {
        case AppSceneType::HOME:
            return MainScene::createScene();
        case AppSceneType::WALLET:
            return WalletScene::createScene();
        case AppSceneType::ADD_CASH:
            return AddCashScene::createScene();
        case AppSceneType::WITHDRAW:
            return WithdrawScene::createScene();
        case AppSceneType::TRANSACTIONS:
            return TransactionsScene::createScene();
        case AppSceneType::PROFILE:
            return ProfileScene::createScene();
        case AppSceneType::SETTINGS:
            return SettingsScene::createScene();
        case AppSceneType::SHARE:
            return ShareScene::createScene();
        case AppSceneType::SEVEN_UP_DOWN:
            return SevenUpDownScene::createScene();
        case AppSceneType::LOGIN:
            return LoginScene::createScene();
        case AppSceneType::ABOUT_US:
        case AppSceneType::CONTACT_US:
        case AppSceneType::FAIR_PLAY:
        case AppSceneType::HELP_CENTRE:
        case AppSceneType::REPORTED_ISSUES:
            return InfoScene::createScene(type);
        default:
            return MainScene::createScene();
    }
}
