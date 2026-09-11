#include "Network/ApiService.h"
#include "Core/Storage/TokenManager.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace rapidjson;

static double getDoubleSafe(const Value& val, const char* key, double def = 0.0) {
    if (val.HasMember(key)) {
        const auto& m = val[key];
        if (m.IsDouble()) return m.GetDouble();
        if (m.IsInt64()) return static_cast<double>(m.GetInt64());
        if (m.IsInt()) return static_cast<double>(m.GetInt());
    }
    return def;
}

static std::string getStringSafe(const Value& val, const char* key, const std::string& def = "") {
    if (val.HasMember(key) && val[key].IsString() && val[key].GetString()) {
        return val[key].GetString();
    }
    return def;
}

static bool getBoolSafe(const Value& val, const char* key, bool def = false) {
    if (val.HasMember(key) && val[key].IsBool()) {
        return val[key].GetBool();
    }
    return def;
}

void ApiService::getUserProfile(std::function<void(bool success, const UserProfileModel& profile)> callback) {
    ApiClient::get("/user/profile", [callback](const ApiResponse& resp) {
        UserProfileModel profile;
        if (resp.success && resp.jsonDoc.IsObject()) {
            const Value& data = resp.jsonDoc.HasMember("data") && resp.jsonDoc["data"].IsObject()
                                    ? resp.jsonDoc["data"]
                                    : resp.jsonDoc;

            profile.id = getStringSafe(data, "id");
            profile.username = getStringSafe(data, "username", TokenManager::getUserName());
            profile.phoneNumber = getStringSafe(data, "phoneNumber", TokenManager::getUserPhone());
            profile.avatarPath = getStringSafe(data, "avatarPath", TokenManager::getUserAvatar());

            if (data.HasMember("depositBalance")) profile.balance.deposit = MoneyPaise::fromRupees(getDoubleSafe(data, "depositBalance"));
            if (data.HasMember("winningBalance")) profile.balance.winning = MoneyPaise::fromRupees(getDoubleSafe(data, "winningBalance"));
            if (data.HasMember("bonusBalance")) profile.balance.bonus = MoneyPaise::fromRupees(getDoubleSafe(data, "bonusBalance"));
            if (data.HasMember("totalBalance")) profile.balance.total = MoneyPaise::fromRupees(getDoubleSafe(data, "totalBalance"));
            else profile.balance.total = profile.balance.deposit + profile.balance.winning + profile.balance.bonus;

            profile.isKycVerified = getBoolSafe(data, "isKycVerified", true);
            profile.isPhoneVerified = getBoolSafe(data, "isPhoneVerified", true);

            if (callback) callback(true, profile);
            return;
        }

        // Fallback default profile if offline / cached
        profile.username = TokenManager::getUserName();
        profile.phoneNumber = TokenManager::getUserPhone();
        profile.avatarPath = TokenManager::getUserAvatar();
        profile.balance = WalletBalance(MoneyPaise(50000), MoneyPaise(25000), MoneyPaise(25000), MoneyPaise(0)); // ₹500 default
        if (callback) callback(false, profile);
    });
}

void ApiService::updateUserProfile(const std::string& username, const std::string& avatarPath, std::function<void(bool success)> callback) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    if (!username.empty()) {
        writer.Key("username");
        writer.String(username.c_str());
    }
    if (!avatarPath.empty()) {
        writer.Key("avatarPath");
        writer.String(avatarPath.c_str());
    }
    writer.EndObject();

    ApiClient::post("/user/update-profile", s.GetString(), [username, avatarPath, callback](const ApiResponse& resp) {
        if (resp.success) {
            if (!username.empty()) TokenManager::setUserName(username);
            if (!avatarPath.empty()) TokenManager::setUserAvatar(avatarPath);
            if (callback) callback(true);
        } else {
            if (callback) callback(false);
        }
    });
}

void ApiService::getGamesList(std::function<void(bool success, const std::vector<GameCardModel>& games)> callback) {
    ApiClient::get("/games", [callback](const ApiResponse& resp) {
        std::vector<GameCardModel> games;
        if (resp.success && resp.jsonDoc.IsObject() && resp.jsonDoc.HasMember("data") && resp.jsonDoc["data"].IsArray()) {
            const Value& arr = resp.jsonDoc["data"];
            if (arr.Size() > 0) {
                for (SizeType i = 0; i < arr.Size(); ++i) {
                    const Value& item = arr[i];
                    if (!item.IsObject()) continue;
                    GameCardModel g;
                    g.id = getStringSafe(item, "id");
                    g.title = getStringSafe(item, "title");
                    g.imagePath = getStringSafe(item, "imagePath");
                    g.gameUrl = getStringSafe(item, "gameUrl");
                    g.isAvailable = getBoolSafe(item, "isAvailable", false);
                    g.accentColorHex = getStringSafe(item, "accentColor", "#00FF87");
                    games.push_back(g);
                }
                if (callback) callback(true, games);
                return;
            }
        }

        // Built-in verified games catalog matching Flutter source
        GameCardModel g1;
        g1.id = "seven_up_down";
        g1.title = "7 Up Down";
        g1.imagePath = "images/7updown.png";
        g1.isAvailable = true;
        g1.minEntryFee = MoneyPaise(1000); // ₹10
        g1.prizePool = MoneyPaise(2000);   // ₹20
        g1.accentColorHex = "#00FF87";
        games.push_back(g1);

        GameCardModel g2;
        g2.id = "classic_dice";
        g2.title = "Classic Dice";
        g2.imagePath = "images/classic_dice.png";
        g2.isAvailable = true;
        g2.minEntryFee = MoneyPaise(1000);
        g2.prizePool = MoneyPaise(2000);
        g2.accentColorHex = "#FFB300";
        games.push_back(g2);

        GameCardModel g3;
        g3.id = "dragon_tiger";
        g3.title = "Dragon Tiger";
        g3.imagePath = "images/dtgame.png";
        g3.isAvailable = true;
        g3.minEntryFee = MoneyPaise(2000);
        g3.prizePool = MoneyPaise(4000);
        g3.accentColorHex = "#E040FB";
        games.push_back(g3);

        GameCardModel g4;
        g4.id = "mines";
        g4.title = "Mines";
        g4.imagePath = "images/mines.png";
        g4.isAvailable = false;
        g4.accentColorHex = "#00E5FF";
        games.push_back(g4);

        GameCardModel g5;
        g5.id = "double";
        g5.title = "Double";
        g5.imagePath = "images/double.png";
        g5.isAvailable = false;
        g5.accentColorHex = "#FF5252";
        games.push_back(g5);

        if (callback) callback(false, games);
    });
}

void ApiService::getTransactions(int page, int limit, std::function<void(bool success, const std::vector<TransactionModel>& transactions)> callback) {
    char endpoint[128];
    snprintf(endpoint, sizeof(endpoint), "/wallet/transactions?page=%d&limit=%d", page, limit);

    ApiClient::get(endpoint, [callback](const ApiResponse& resp) {
        std::vector<TransactionModel> txs;
        if (resp.success && resp.jsonDoc.IsObject() && resp.jsonDoc.HasMember("data") && resp.jsonDoc["data"].IsArray()) {
            const Value& arr = resp.jsonDoc["data"];
            for (SizeType i = 0; i < arr.Size(); ++i) {
                const Value& item = arr[i];
                if (!item.IsObject()) continue;
                TransactionModel t;
                t.id = getStringSafe(item, "id");
                t.type = getStringSafe(item, "type");
                t.amount = MoneyPaise::fromRupees(getDoubleSafe(item, "amount"));
                t.status = getStringSafe(item, "status");
                t.createdAt = getStringSafe(item, "createdAt");
                t.description = getStringSafe(item, "description");
                txs.push_back(t);
            }
            if (callback) callback(true, txs);
            return;
        }

        // Default mock transactions if offline
        TransactionModel t1;
        t1.id = "TX1001";
        t1.type = "DEPOSIT";
        t1.amount = MoneyPaise(50000); // ₹500
        t1.status = "COMPLETED";
        t1.createdAt = "Today, 02:45 PM";
        t1.description = "UPI Deposit";
        txs.push_back(t1);

        TransactionModel t2;
        t2.id = "TX1002";
        t2.type = "BET_WIN";
        t2.amount = MoneyPaise(19600); // ₹196
        t2.status = "COMPLETED";
        t2.createdAt = "Yesterday, 08:30 PM";
        t2.description = "7 Up Down Win (Up 8-12)";
        txs.push_back(t2);

        if (callback) callback(false, txs);
    });
}

void ApiService::addCash(const MoneyPaise& amount, const std::string& paymentMethod, std::function<void(bool success, const std::string& orderId)> callback) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("amount");
    writer.Double(amount.toRupees());
    writer.Key("amountPaise");
    writer.Int64(amount.value);
    writer.Key("paymentMethod");
    writer.String(paymentMethod.c_str());
    writer.EndObject();

    ApiClient::post("/wallet/deposit", s.GetString(), [callback](const ApiResponse& resp) {
        if (resp.success) {
            std::string orderId = "ORDER_" + std::to_string(time(nullptr));
            if (callback) callback(true, orderId);
        } else {
            if (callback) callback(false, resp.errorMessage);
        }
    });
}

void ApiService::withdrawCash(const MoneyPaise& amount, const std::string& upiId, std::function<void(bool success, const std::string& message)> callback) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("amount");
    writer.Double(amount.toRupees());
    writer.Key("amountPaise");
    writer.Int64(amount.value);
    writer.Key("upiId");
    writer.String(upiId.c_str());
    writer.EndObject();

    ApiClient::post("/wallet/withdraw", s.GetString(), [callback](const ApiResponse& resp) {
        if (resp.success) {
            if (callback) callback(true, "Withdrawal request submitted successfully!");
        } else {
            if (callback) callback(false, resp.errorMessage.empty() ? "Withdrawal failed" : resp.errorMessage);
        }
    });
}
