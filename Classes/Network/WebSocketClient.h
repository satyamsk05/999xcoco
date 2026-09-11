#ifndef __WEBSOCKET_CLIENT_H__
#define __WEBSOCKET_CLIENT_H__

#include "cocos2d.h"
#include "network/WebSocket.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <string>
#include <functional>

enum class WsConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    RECONNECTING
};

typedef std::function<void(const std::string& eventType, const rapidjson::Document& data)> WsEventCallback;

class WebSocketClient : public cocos2d::network::WebSocket::Delegate {
public:
    static WebSocketClient* getInstance();

    void connect(const std::string& url = "");
    void disconnect();
    void sendEvent(const std::string& eventType, const std::string& jsonPayload);
    void setEventCallback(const WsEventCallback& callback) { _eventCallback = callback; }

    WsConnectionState getState() const { return _state; }

    // WebSocket Delegate Methods
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

private:
    WebSocketClient();
    ~WebSocketClient();

    static WebSocketClient* _instance;
    cocos2d::network::WebSocket* _ws;
    std::string _url;
    WsConnectionState _state;
    WsEventCallback _eventCallback;
    int _reconnectAttempts;
    float _reconnectDelay;

    void scheduleReconnect();
    void sendHeartbeat(float dt);
};

#endif // __WEBSOCKET_CLIENT_H__
