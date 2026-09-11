#ifndef __BET_MANAGER_H__
#define __BET_MANAGER_H__

#include "Core/MoneyPaise.h"
#include <map>
#include <string>
#include <vector>

enum class BetSpot {
    DOWN_2_6,   // 2X
    LUCKY_7,    // 5X
    UP_8_12,    // 2X
    NUM_2, NUM_3, NUM_4, NUM_5, NUM_6,
    NUM_8, NUM_9, NUM_10, NUM_11, NUM_12
};

enum class GamePhase {
    BETTING_OPEN,
    BETTING_CLOSED,
    DICE_ROLLING,
    SHOW_RESULT,
    PAYOUT
};

struct BetPlacement {
    BetSpot spot;
    MoneyPaise amount;
};

class BetManager {
public:
    static BetManager* getInstance();

    void reset();
    bool selectChip(int64_t chipPaise);
    int64_t getSelectedChipPaise() const { return _selectedChipPaise; }

    bool placeBet(BetSpot spot, MoneyPaise userBalance);
    void clearCurrentBets();
    void doubleBets();

    MoneyPaise getTotalBetAmount() const;
    MoneyPaise getBetOnSpot(BetSpot spot) const;
    const std::map<BetSpot, MoneyPaise>& getAllBets() const { return _activeBets; }

    void setGamePhase(GamePhase phase) { _currentPhase = phase; }
    GamePhase getGamePhase() const { return _currentPhase; }
    bool isBettingAllowed() const { return _currentPhase == GamePhase::BETTING_OPEN; }

    // Settlement Calculation
    MoneyPaise calculatePayout(int dice1, int dice2);
    static bool isWinningSpot(BetSpot spot, int totalSum);

private:
    BetManager();
    static BetManager* _instance;

    int64_t _selectedChipPaise;
    GamePhase _currentPhase;
    std::map<BetSpot, MoneyPaise> _activeBets;
    std::map<BetSpot, MoneyPaise> _lastRoundBets;
};

#endif // __BET_MANAGER_H__
