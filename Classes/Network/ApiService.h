#ifndef __API_SERVICE_H__
#define __API_SERVICE_H__

#include "Network/ApiClient.h"
#include "Core/MoneyPaise.h"
#include <string>
#include <vector>
#include <functional>

struct GameCardModel {
    std::string id;
    std::string title;
    std::string imagePath;
    std::string gameUrl;
    bool isAvailable;
    std::string accentColorHex;
    MoneyPaise minEntryFee;
    MoneyPaise prizePool;
};

struct UserProfileModel {
    std::string id;
    std::string username;
    std::string phoneNumber;
    std::string avatarPath;
    WalletBalance balance;
    bool isKycVerified;
    bool isPhoneVerified;
};

struct TransactionModel {
    std::string id;
    std::string type; // DEPOSIT, WITHDRAWAL, BET_WIN, BET_LOSS
    MoneyPaise amount;
    std::string status; // COMPLETED, PENDING, FAILED
    std::string createdAt;
    std::string description;
};

class ApiService {
public:
    static void getUserProfile(std::function<void(bool success, const UserProfileModel& profile)> callback);
    static void updateUserProfile(const std::string& username, const std::string& avatarPath, std::function<void(bool success)> callback);
    static void getGamesList(std::function<void(bool success, const std::vector<GameCardModel>& games)> callback);
    static void getTransactions(int page, int limit, std::function<void(bool success, const std::vector<TransactionModel>& transactions)> callback);
    static void addCash(const MoneyPaise& amount, const std::string& paymentMethod, std::function<void(bool success, const std::string& orderId)> callback);
    static void withdrawCash(const MoneyPaise& amount, const std::string& upiId, std::function<void(bool success, const std::string& message)> callback);
};

#endif // __API_SERVICE_H__
