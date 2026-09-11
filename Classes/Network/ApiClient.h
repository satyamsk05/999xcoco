#ifndef __API_CLIENT_H__
#define __API_CLIENT_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <string>
#include <functional>

struct ApiResponse {
    bool success;
    long statusCode;
    std::string rawData;
    std::string errorCode;
    std::string errorMessage;
    rapidjson::Document jsonDoc;

    ApiResponse() : success(false), statusCode(0), errorCode(""), errorMessage("") {}
};

typedef std::function<void(const ApiResponse& response)> ApiCallback;

class ApiClient {
public:
    static std::string serverBaseUrl;

    static void get(const std::string& endpoint, const ApiCallback& callback);
    static void post(const std::string& endpoint, const std::string& jsonBody, const ApiCallback& callback);

private:
    static void sendRequest(cocos2d::network::HttpRequest::Type type,
                            const std::string& endpoint,
                            const std::string& jsonBody,
                            const ApiCallback& callback);

    static void handleResponse(cocos2d::network::HttpClient* client,
                               cocos2d::network::HttpResponse* response,
                               const ApiCallback& callback);
};

#endif // __API_CLIENT_H__
