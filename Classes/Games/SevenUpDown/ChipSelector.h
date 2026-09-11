#ifndef __CHIP_SELECTOR_H__
#define __CHIP_SELECTOR_H__

#include "cocos2d.h"
#include <vector>
#include <functional>

struct ChipItem {
    int64_t paiseValue;
    std::string label;
    cocos2d::Color3B color;
};

class ChipSelector : public cocos2d::Node {
public:
    static ChipSelector* create(float width, float height);
    virtual bool init(float width, float height);

    void setSelectedPaise(int64_t paise);
    void setOnChipSelected(const std::function<void(int64_t paise)>& callback) { _onChipSelected = callback; }
    void setOnClearBets(const std::function<void()>& callback) { _onClearBets = callback; }
    void setOnDoubleBets(const std::function<void()>& callback) { _onDoubleBets = callback; }

private:
    float _width;
    float _height;
    int64_t _selectedPaise;
    std::vector<ChipItem> _chips;
    std::vector<cocos2d::DrawNode*> _chipNodes;
    std::function<void(int64_t paise)> _onChipSelected;
    std::function<void()> _onClearBets;
    std::function<void()> _onDoubleBets;

    void updateSelectionVisuals();
};

#endif // __CHIP_SELECTOR_H__
