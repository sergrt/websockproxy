#pragma once

#include "crow.h"

#include <future>
#include <mutex>
#include <string>

class WsServer {
public:
    WsServer(const std::string& address, uint16_t port);
    WsServer(const WsServer&) = delete;
    WsServer(WsServer&&) = delete;
    WsServer& operator=(const WsServer&) = delete;
    WsServer& operator=(WsServer&&) = delete;

    ~WsServer();

    void Stop();

private:
    bool AcceptHandler(const crow::request& req, void** userdata);
    void OpenHandler(crow::websocket::connection& conn);
    void CloseHandler(crow::websocket::connection& conn);
    void MessageHandler(crow::websocket::connection& conn, const std::string& data, bool is_binary);
    void ErrorHandler(crow::websocket::connection& conn, const std::string& error_message);

    std::future<void> run_future_;  // Crow async holder
    crow::SimpleApp app_;
    std::mutex capacity_guard_;
    size_t capacity_ = 0;
};
