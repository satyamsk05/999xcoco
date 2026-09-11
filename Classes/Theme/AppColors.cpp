#include "Theme/AppColors.h"

using namespace cocos2d;

// Main Background Colors (#1F0130 -> #0F0016)
const Color3B AppColors::backgroundStart(0x1F, 0x01, 0x30);
const Color3B AppColors::backgroundEnd(0x0F, 0x00, 0x16);
const Color4B AppColors::backgroundStart4B(0x1F, 0x01, 0x30, 0xFF);
const Color4B AppColors::backgroundEnd4B(0x0F, 0x00, 0x16, 0xFF);

// Profile & Header
const Color3B AppColors::profileName(0xFF, 0xFF, 0xFF);
const Color3B AppColors::profileSubtext(0xCA, 0xA7, 0x72); // rgba(202, 167, 114, 1)
const Color3B AppColors::avatarBg(0xB1, 0xB2, 0xB5);
const Color3B AppColors::avatarBorder(0xE1, 0xB2, 0x19); // Gold

// Green Action Buttons
const Color3B AppColors::greenButtonStart(0x00, 0xB5, 0x7F);
const Color3B AppColors::greenButtonEnd(0x00, 0x9A, 0x69);

// Ticker Gradient Stops
const Color4B AppColors::tickerStop1(0x1F, 0x01, 0x30, 0xFF);
const Color4B AppColors::tickerStop2(0x3F, 0x0A, 0x44, 0xFF);
const Color4B AppColors::tickerStop3(0x38, 0x0B, 0x3F, 0xFF);
const Color4B AppColors::tickerStop4(0x1F, 0x01, 0x30, 0xFF);

// Card Borders & Indicators
const Color3B AppColors::cardBorder(0x4F, 0x10, 0x6D);
const Color3B AppColors::cardBg(0x29, 0x04, 0x3B);
const Color3B AppColors::liveIndicator(0x00, 0xFF, 0x87);
const Color3B AppColors::accentPurple(0x6C, 0x20, 0xE0);
const Color3B AppColors::darkPurple(0x20, 0x08, 0x4B);

// Bottom Navigation Bar
const Color4B AppColors::bottomNavStart(0x53, 0x11, 0x71, 0xFF);
const Color4B AppColors::bottomNavEnd(0x32, 0x03, 0x46, 0xFF);
const Color3B AppColors::navActive(0xE1, 0xB2, 0x19);
const Color3B AppColors::navInactive(0xA0, 0x8A, 0xB0);

// 7 Up Down Game Colors
const Color3B AppColors::downColor(0x29, 0x79, 0xFF);       // Blue: 2-6
const Color3B AppColors::luckySevenColor(0xFF, 0xD7, 0x00); // Gold: 7
const Color3B AppColors::upColor(0xFF, 0x17, 0x44);         // Red: 8-12
const Color3B AppColors::tableBg(0x12, 0x03, 0x24);
