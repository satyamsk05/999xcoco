#ifndef __APP_COLORS_H__
#define __APP_COLORS_H__

#include "cocos2d.h"

class AppColors {
public:
    // Main Background Colors (#1F0130 -> #0F0016)
    static const cocos2d::Color3B backgroundStart;
    static const cocos2d::Color3B backgroundEnd;
    static const cocos2d::Color4B backgroundStart4B;
    static const cocos2d::Color4B backgroundEnd4B;

    // Profile & Header
    static const cocos2d::Color3B profileName;      // #FFFFFF
    static const cocos2d::Color3B profileSubtext;   // #CAA772 (rgba 202, 167, 114)
    static const cocos2d::Color3B avatarBg;         // #B1B2B5
    static const cocos2d::Color3B avatarBorder;     // #E1B219 (Gold)

    // Green Action Buttons (Add Cash / Deposit / Confirm)
    static const cocos2d::Color3B greenButtonStart; // #00B57F (rgba 0, 181, 127)
    static const cocos2d::Color3B greenButtonEnd;   // #009A69 (rgba 0, 154, 105)

    // Ticker Gradient Stops
    static const cocos2d::Color4B tickerStop1; // #1F0130
    static const cocos2d::Color4B tickerStop2; // #3F0A44
    static const cocos2d::Color4B tickerStop3; // #380B3F
    static const cocos2d::Color4B tickerStop4; // #1F0130

    // Card Borders & Indicators
    static const cocos2d::Color3B cardBorder;       // #4F106D
    static const cocos2d::Color3B cardBg;           // #29043B
    static const cocos2d::Color3B liveIndicator;    // #00FF87 (Bright Green)
    static const cocos2d::Color3B accentPurple;     // #6C20E0
    static const cocos2d::Color3B darkPurple;       // #20084B

    // Bottom Navigation Bar
    static const cocos2d::Color4B bottomNavStart;   // #531171 (rgba 83, 17, 113)
    static const cocos2d::Color4B bottomNavEnd;     // #320346 (rgba 50, 3, 70)
    static const cocos2d::Color3B navActive;        // #E1B219
    static const cocos2d::Color3B navInactive;      // #A08AB0

    // 7 Up Down Game Colors
    static const cocos2d::Color3B downColor;        // #2979FF (Blue: 2-6)
    static const cocos2d::Color3B luckySevenColor;  // #FFD700 (Gold: 7)
    static const cocos2d::Color3B upColor;          // #FF1744 (Red: 8-12)
    static const cocos2d::Color3B tableBg;          // #120324
};

#endif // __APP_COLORS_H__
