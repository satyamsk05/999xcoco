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

static const std::string SALT_KEY = "999x_sEcUrE_tOkEn_sAlT_2026_!#%";

static std::string encryptData(const std::string& input) {
    if (input.empty()) return "";
    std::string output = "";
    output.reserve(input.length() * 2);
    static const char hexChars[] = "0123456789ABCDEF";
    for (size_t i = 0; i < input.length(); ++i) {
        char masked = input[i] ^ SALT_KEY[i % SALT_KEY.length()];
        output.push_back(hexChars[(masked >> 4) & 0x0F]);
        output.push_back(hexChars[masked & 0x0F]);
    }
    return output;
}

static std::string decryptData(const std::string& hexInput) {
    if (hexInput.empty() || hexInput.length() % 2 != 0) return "";
    std::string output = "";
    output.reserve(hexInput.length() / 2);
    for (size_t i = 0; i < hexInput.length(); i += 2) {
        char high = hexInput[i];
        char low = hexInput[i + 1];
        int hVal = (high >= '0' && high <= '9') ? (high - '0') : (high >= 'A' && high <= 'F' ? high - 'A' + 10 : high - 'a' + 10);
        int lVal = (low >= '0' && low <= '9') ? (low - '0') : (low >= 'A' && low <= 'F' ? low - 'A' + 10 : low - 'a' + 10);
        char masked = static_cast<char>((hVal << 4) | lVal);
        size_t charIndex = i / 2;
        output.push_back(masked ^ SALT_KEY[charIndex % SALT_KEY.length()]);
    }
    return output;
}

void TokenManager::init() {
    auto def = UserDefault::getInstance();
    std::string encToken = def->getStringForKey(KEY_TOKEN, "");
    _cachedToken = decryptData(encToken);
    if (_cachedToken.empty() && !encToken.empty()) {
        // Fallback for unencrypted legacy format
        _cachedToken = encToken;
    }
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
    def->setStringForKey(KEY_TOKEN, encryptData(_cachedToken));
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
