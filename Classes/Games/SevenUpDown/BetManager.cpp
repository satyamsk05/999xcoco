#include "Games/SevenUpDown/BetManager.h"

BetManager* BetManager::_instance = nullptr;

BetManager* BetManager::getInstance() {
    if (!_instance) {
        _instance = new BetManager();
    }
    return _instance;
}

BetManager::BetManager()
    : _selectedChipPaise(1000), // Default ₹10 = 1000 paise
      _currentPhase(GamePhase::BETTING_OPEN) {
}

void BetManager::reset() {
    _lastRoundBets = _activeBets;
    _activeBets.clear();
    _currentPhase = GamePhase::BETTING_OPEN;
}

bool BetManager::selectChip(int64_t chipPaise) {
    if (chipPaise > 0) {
        _selectedChipPaise = chipPaise;
        return true;
    }
    return false;
}

bool BetManager::placeBet(BetSpot spot, MoneyPaise userBalance) {
    if (!isBettingAllowed()) return false;

    MoneyPaise proposedTotal = getTotalBetAmount() + MoneyPaise(_selectedChipPaise);
    if (proposedTotal > userBalance) {
        return false; // Insufficient balance
    }

    _activeBets[spot] += MoneyPaise(_selectedChipPaise);
    return true;
}

void BetManager::clearCurrentBets() {
    if (isBettingAllowed()) {
        _activeBets.clear();
    }
}

void BetManager::doubleBets() {
    if (!isBettingAllowed()) return;
    for (auto& pair : _activeBets) {
        pair.second += pair.second;
    }
}

MoneyPaise BetManager::getTotalBetAmount() const {
    MoneyPaise total(0);
    for (const auto& pair : _activeBets) {
        total += pair.second;
    }
    return total;
}

MoneyPaise BetManager::getBetOnSpot(BetSpot spot) const {
    auto it = _activeBets.find(spot);
    if (it != _activeBets.end()) {
        return it->second;
    }
    return MoneyPaise(0);
}

bool BetManager::isWinningSpot(BetSpot spot, int totalSum) {
    switch (spot) {
        case BetSpot::DOWN_2_6:
            return (totalSum >= 2 && totalSum <= 6);
        case BetSpot::LUCKY_7:
            return (totalSum == 7);
        case BetSpot::UP_8_12:
            return (totalSum >= 8 && totalSum <= 12);
        case BetSpot::NUM_2: return totalSum == 2;
        case BetSpot::NUM_3: return totalSum == 3;
        case BetSpot::NUM_4: return totalSum == 4;
        case BetSpot::NUM_5: return totalSum == 5;
        case BetSpot::NUM_6: return totalSum == 6;
        case BetSpot::NUM_8: return totalSum == 8;
        case BetSpot::NUM_9: return totalSum == 9;
        case BetSpot::NUM_10: return totalSum == 10;
        case BetSpot::NUM_11: return totalSum == 11;
        case BetSpot::NUM_12: return totalSum == 12;
        default:
            return false;
    }
}

MoneyPaise BetManager::calculatePayout(int dice1, int dice2) {
    int totalSum = dice1 + dice2;
    MoneyPaise totalPayout(0);

    for (const auto& pair : _activeBets) {
        BetSpot spot = pair.first;
        MoneyPaise bet = pair.second;

        if (spot == BetSpot::DOWN_2_6 && totalSum >= 2 && totalSum <= 6) {
            totalPayout += MoneyPaise(bet.value * 2); // 2X
        } else if (spot == BetSpot::UP_8_12 && totalSum >= 8 && totalSum <= 12) {
            totalPayout += MoneyPaise(bet.value * 2); // 2X
        } else if (spot == BetSpot::LUCKY_7 && totalSum == 7) {
            totalPayout += MoneyPaise(bet.value * 5); // 5X
        }
    }

    return totalPayout;
}
