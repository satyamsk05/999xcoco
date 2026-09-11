#ifndef __BOTTOM_NAV_BAR_H__
#define __BOTTOM_NAV_BAR_H__

#include "cocos2d.h"
#include <vector>
#include <functional>

class BottomNavBar : public cocos2d::Node {
public:
    static BottomNavBar* create(float width, int selectedIndex = 0);
    virtual bool init(float width, int selectedIndex);

    void setSelectedIndex(int index);
    void setOnItemSelected(const std::function<void(int index)>& callback) { _onItemSelected = callback; }

private:
    float _width;
    int _selectedIndex;
    std::function<void(int index)> _onItemSelected;

    struct NavItemUI {
        cocos2d::Node* container;
        cocos2d::Label* label;
        cocos2d::Sprite* icon;
    };
    std::vector<NavItemUI> _items;

    void updateSelectionVisuals();
};

#endif // __BOTTOM_NAV_BAR_H__
