#include "Network/ApiClient.h"
#include "Core/Storage/TokenManager.h"
#include "App/SceneManager.h"

using namespace cocos2d;
using namespace cocos2d::network;

std::string ApiClient::serverBaseUrl = "https://ingames.onrender.com/api";

void ApiClient::get(const std::string& endpoint, const ApiCallback& callback) {
    sendRequest(HttpRequest::Type::GET, endpoint, "", callback);
}

void ApiClient::post(const std::string& endpoint, const std::string& jsonBody, const ApiCallback& callback) {
    sendRequest(HttpRequest::Type::POST, endpoint, jsonBody, callback);
}

void ApiClient::sendRequest(HttpRequest::Type type,
                           const std::string& endpoint,
                           const std::string& jsonBody,
                           const ApiCallback& callback) {
    auto request = new HttpRequest();
    std::string url = serverBaseUrl + endpoint;
    request->setUrl(url.c_str());
    request->setRequestType(type);

    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json");
    headers.push_back("Accept: application/json");

    if (TokenManager::isAuthenticated()) {
        headers.push_back("Authorization: Bearer " + TokenManager::getToken());
    }
    request->setHeaders(headers);

    if (type == HttpRequest::Type::POST && !jsonBody.empty()) {
        request->setRequestData(jsonBody.c_str(), jsonBody.length());
    }

    request->setResponseCallback([callback](HttpClient* client, HttpResponse* response) {
        handleResponse(client, response, callback);
    });

    HttpClient::getInstance()->send(request);
    request->release();
}

void ApiClient::handleResponse(HttpClient* client, HttpResponse* response, const ApiCallback& callback) {
    ApiResponse apiResp;
    if (!response) {
        apiResp.success = false;
        apiResp.statusCode = 0;
        apiResp.errorMessage = "No response from server";
        if (callback) callback(apiResp);
        return;
    }

    apiResp.statusCode = response->getResponseCode();
    std::vector<char>* buffer = response->getResponseData();
    if (buffer && !buffer->empty()) {
        apiResp.rawData.assign(buffer->begin(), buffer->end());
    }

    if (!response->isSucceed()) {
        apiResp.success = false;
        const char* errBuf = response->getErrorBuffer();
        if (errBuf) {
            apiResp.errorMessage = errBuf;
        }
        if (apiResp.errorMessage.empty()) {
            apiResp.errorMessage = "Network request failed";
        }
        if (callback) callback(apiResp);
        return;
    }

    if (!apiResp.rawData.empty()) {
        apiResp.jsonDoc.Parse(apiResp.rawData.c_str());
        if (!apiResp.jsonDoc.HasParseError()) {
            if (apiResp.jsonDoc.IsObject() && apiResp.jsonDoc.HasMember("success") && apiResp.jsonDoc["success"].IsBool()) {
                apiResp.success = apiResp.jsonDoc["success"].GetBool();
            } else {
                apiResp.success = (apiResp.statusCode >= 200 && apiResp.statusCode < 300);
            }

            if (apiResp.jsonDoc.IsObject() && apiResp.jsonDoc.HasMember("error")) {
                const auto& err = apiResp.jsonDoc["error"];
                if (err.IsObject()) {
                    if (err.HasMember("message") && err["message"].IsString() && err["message"].GetString()) {
                        apiResp.errorMessage = err["message"].GetString();
                    }
                    if (err.HasMember("code") && err["code"].IsString() && err["code"].GetString()) {
                        apiResp.errorCode = err["code"].GetString();
                    }
                } else if (err.IsString() && err.GetString()) {
                    apiResp.errorMessage = err.GetString();
                }
            }
        } else {
            apiResp.success = (apiResp.statusCode >= 200 && apiResp.statusCode < 300);
        }
    } else {
        apiResp.success = (apiResp.statusCode >= 200 && apiResp.statusCode < 300);
    }

    // 401 Session expired handling
    if (apiResp.statusCode == 401) {
        TokenManager::clearSession();
    }

    if (callback) {
        callback(apiResp);
    }
}
