#include "Core/Storage/TokenManager.h"

using namespace cocos2d;

std::string TokenManager::_cachedToken = "";
std::string TokenManager::_cachedUserId = "";
std::string TokenManager::_cachedUserName = "Player";
std::string TokenManager::_cachedUserPhone = "";
std::string TokenManager::_cachedUserAvatar = "avatars/avatar_1.png";

const char* TokenManager::KEY_TOKEN = "auth_jwt_token";
const char* TokenManager::KEY_USER_ID = "auth_user_id";
const char* TokenManager::KEY_USER_NAME = "auth_user_name";
const char* TokenManager::KEY_USER_PHONE = "auth_user_phone";
const char* TokenManager::KEY_USER_AVATAR = "auth_user_avatar";

void TokenManager::init() {
    auto def = UserDefault::getInstance();
    _cachedToken = def->getStringForKey(KEY_TOKEN, "");
    _cachedUserId = def->getStringForKey(KEY_USER_ID, "");
    _cachedUserName = def->getStringForKey(KEY_USER_NAME, "Player");
    _cachedUserPhone = def->getStringForKey(KEY_USER_PHONE, "");
    _cachedUserAvatar = def->getStringForKey(KEY_USER_AVATAR, "avatars/avatar_1.png");
}

bool TokenManager::isAuthenticated() {
    return !_cachedToken.empty() && !_cachedUserId.empty();
}

std::string TokenManager::getToken() {
    return _cachedToken;
}

std::string TokenManager::getUserId() {
    return _cachedUserId;
}

std::string TokenManager::getUserName() {
    return _cachedUserName.empty() ? "Player" : _cachedUserName;
}

std::string TokenManager::getUserPhone() {
    return _cachedUserPhone;
}

std::string TokenManager::getUserAvatar() {
    return _cachedUserAvatar.empty() ? "avatars/avatar_1.png" : _cachedUserAvatar;
}

void TokenManager::setUserName(const std::string& name) {
    _cachedUserName = name;
    UserDefault::getInstance()->setStringForKey(KEY_USER_NAME, name);
    UserDefault::getInstance()->flush();
}

void TokenManager::setUserAvatar(const std::string& avatarPath) {
    _cachedUserAvatar = avatarPath;
    UserDefault::getInstance()->setStringForKey(KEY_USER_AVATAR, avatarPath);
    UserDefault::getInstance()->flush();
}

void TokenManager::saveSession(const std::string& token,
                              const std::string& userId,
                              const std::string& username,
                              const std::string& phone,
                              const std::string& avatar) {
    _cachedToken = token;
    _cachedUserId = userId;
    if (!username.empty()) _cachedUserName = username;
    if (!phone.empty()) _cachedUserPhone = phone;
    if (!avatar.empty()) _cachedUserAvatar = avatar;

    auto def = UserDefault::getInstance();
    def->setStringForKey(KEY_TOKEN, _cachedToken);
    def->setStringForKey(KEY_USER_ID, _cachedUserId);
    def->setStringForKey(KEY_USER_NAME, _cachedUserName);
    def->setStringForKey(KEY_USER_PHONE, _cachedUserPhone);
    def->setStringForKey(KEY_USER_AVATAR, _cachedUserAvatar);
    def->flush();
}

void TokenManager::clearSession() {
    _cachedToken = "";
    _cachedUserId = "";
    _cachedUserName = "Player";
    _cachedUserPhone = "";
    _cachedUserAvatar = "avatars/avatar_1.png";

    auto def = UserDefault::getInstance();
    def->deleteValueForKey(KEY_TOKEN);
    def->deleteValueForKey(KEY_USER_ID);
    def->deleteValueForKey(KEY_USER_NAME);
    def->deleteValueForKey(KEY_USER_PHONE);
    def->deleteValueForKey(KEY_USER_AVATAR);
    def->flush();
}
