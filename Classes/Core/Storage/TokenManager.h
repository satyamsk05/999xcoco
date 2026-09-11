#ifndef __TOKEN_MANAGER_H__
#define __TOKEN_MANAGER_H__

#include <string>
#include "cocos2d.h"

class TokenManager {
public:
    static void init();

    static bool isAuthenticated();
    static std::string getToken();
    static std::string getUserId();
    static std::string getUserName();
    static std::string getUserPhone();
    static std::string getUserAvatar();

    static void setUserName(const std::string& name);
    static void setUserAvatar(const std::string& avatarPath);

    static void saveSession(const std::string& token,
                            const std::string& userId,
                            const std::string& username = "",
                            const std::string& phone = "",
                            const std::string& avatar = "");

    static void clearSession();

private:
    static std::string _cachedToken;
    static std::string _cachedUserId;
    static std::string _cachedUserName;
    static std::string _cachedUserPhone;
    static std::string _cachedUserAvatar;

    static const char* KEY_TOKEN;
    static const char* KEY_USER_ID;
    static const char* KEY_USER_NAME;
    static const char* KEY_USER_PHONE;
    static const char* KEY_USER_AVATAR;
};

#endif // __TOKEN_MANAGER_H__
