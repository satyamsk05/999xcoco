#ifndef __HISTORY_BAR_H__
#define __HISTORY_BAR_H__

#include "cocos2d.h"
#include <vector>

class HistoryBar : public cocos2d::Node {
public:
    static HistoryBar* create(float width, float height);
    virtual bool init(float width, float height);

    void addOutcome(int sum);
    void setHistory(const std::vector<int>& history);

private:
    float _width;
    float _height;
    std::vector<int> _history;
    cocos2d::Node* _itemsContainer;

    void refreshVisuals();
};

#endif // __HISTORY_BAR_H__
