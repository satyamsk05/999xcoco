#ifndef __TRANSACTIONS_SCENE_H__
#define __TRANSACTIONS_SCENE_H__

#include "cocos2d.h"
#include "Network/ApiService.h"
#include <vector>

class TransactionsScene : public cocos2d::Scene {
public:
    static TransactionsScene* createScene();
    virtual bool init() override;

    CREATE_FUNC(TransactionsScene);

private:
    float _designWidth;
    float _designHeight;
    std::string _currentFilter; // All, DEPOSIT, WITHDRAWAL, BET_WIN
    std::vector<TransactionModel> _allTransactions;
    cocos2d::Node* _listContainer;

    void setupUI();
    void loadTransactions();
    void renderList();
};

#endif // __TRANSACTIONS_SCENE_H__
