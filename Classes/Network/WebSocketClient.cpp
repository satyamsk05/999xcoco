#include "Network/WebSocketClient.h"
#include "Core/Storage/TokenManager.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace cocos2d;
using namespace cocos2d::network;
using namespace rapidjson;

WebSocketClient* WebSocketClient::_instance = nullptr;

WebSocketClient* WebSocketClient::getInstance() {
    if (!_instance) {
        _instance = new WebSocketClient();
    }
    return _instance;
}

WebSocketClient::WebSocketClient()
    : _ws(nullptr),
      _url("wss://ingames.onrender.com/ws"),
      _state(WsConnectionState::DISCONNECTED),
      _eventCallback(nullptr),
      _reconnectAttempts(0),
      _reconnectDelay(1.0f) {
}

WebSocketClient::~WebSocketClient() {
    disconnect();
}

void WebSocketClient::connect(const std::string& url) {
    if (!url.empty()) _url = url;
    if (_state == WsConnectionState::CONNECTED || _state == WsConnectionState::CONNECTING) {
        return;
    }

    _state = WsConnectionState::CONNECTING;
    if (_ws) {
        _ws->closeAsync();
        _ws = nullptr;
    }

    _ws = new WebSocket();
    std::vector<std::string> protocols;
    if (!_ws->init(*this, _url, &protocols)) {
        CCLOG("WebSocket initialization failed");
        _state = WsConnectionState::DISCONNECTED;
        scheduleReconnect();
    }
}

void WebSocketClient::disconnect() {
    Director::getInstance()->getScheduler()->unschedule("ws_heartbeat", this);
    Director::getInstance()->getScheduler()->unschedule("ws_reconnect", this);

    if (_ws) {
        _ws->closeAsync();
        _ws = nullptr;
    }
    _state = WsConnectionState::DISCONNECTED;
}

void WebSocketClient::sendEvent(const std::string& eventType, const std::string& jsonPayload) {
    if (_state != WsConnectionState::CONNECTED || !_ws) return;

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("type");
    writer.String(eventType.c_str());
    writer.Key("payload");
    writer.String(jsonPayload.c_str());
    if (TokenManager::isAuthenticated()) {
        writer.Key("token");
        writer.String(TokenManager::getToken().c_str());
    }
    writer.EndObject();

    _ws->send(s.GetString());
}

void WebSocketClient::onOpen(WebSocket* ws) {
    CCLOG("WebSocket Connected!");
    _state = WsConnectionState::CONNECTED;
    _reconnectAttempts = 0;
    _reconnectDelay = 1.0f;

    // Send auth handshake
    if (TokenManager::isAuthenticated()) {
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("type");
        writer.String("AUTH_INIT");
        writer.Key("token");
        writer.String(TokenManager::getToken().c_str());
        writer.EndObject();
        ws->send(s.GetString());
    }

    // Schedule 15-second heartbeat ping
    Director::getInstance()->getScheduler()->schedule(
        CC_CALLBACK_1(WebSocketClient::sendHeartbeat, this), this, 15.0f, false, "ws_heartbeat");
}

void WebSocketClient::onMessage(WebSocket* ws, const WebSocket::Data& data) {
    if (data.isBinary) return;

    std::string msg(data.bytes, data.len);
    Document doc;
    doc.Parse(msg.c_str());

    if (!doc.HasParseError() && doc.IsObject()) {
        std::string eventType = "";
        if (doc.HasMember("type") && doc["type"].IsString()) {
            eventType = doc["type"].GetString();
        }
        if (_eventCallback) {
            _eventCallback(eventType, doc);
        }
    }
}

void WebSocketClient::onClose(WebSocket* ws) {
    CCLOG("WebSocket Closed");
    _state = WsConnectionState::DISCONNECTED;
    _ws = nullptr;
    Director::getInstance()->getScheduler()->unschedule("ws_heartbeat", this);
    scheduleReconnect();
}

void WebSocketClient::onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("WebSocket Error occurred: %d", static_cast<int>(error));
    _state = WsConnectionState::DISCONNECTED;
    _ws = nullptr;
    Director::getInstance()->getScheduler()->unschedule("ws_heartbeat", this);
    scheduleReconnect();
}

void WebSocketClient::scheduleReconnect() {
    _state = WsConnectionState::RECONNECTING;
    _reconnectAttempts++;
    _reconnectDelay = std::min(_reconnectDelay * 1.5f, 16.0f);

    Director::getInstance()->getScheduler()->schedule(
        [this](float dt) {
            Director::getInstance()->getScheduler()->unschedule("ws_reconnect", this);
            this->connect();
        },
        this, _reconnectDelay, 0, 0.0f, false, "ws_reconnect");
}

void WebSocketClient::sendHeartbeat(float dt) {
    if (_state == WsConnectionState::CONNECTED && _ws) {
        _ws->send("{\"type\":\"PING\"}");
    }
}
