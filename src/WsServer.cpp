#include "WsServer.h"

#include "HttpClient.h"
#include "Payload.h"
#include "Requests.h"

#include <nlohmann/json.hpp>

constexpr size_t kMaxCapacity = 16;
constexpr size_t kMaxPayloadSizeBytes = 65535;

namespace {

std::string MakeResponseJson(int status, const std::string& body) {
    nlohmann::json json;
    json["status"] = status;
    json["body"] = body;
    return json.dump();
}

}  // namespace

WsServer::WsServer(const std::string& address, uint16_t port) {
    using namespace std::placeholders;
    CROW_WEBSOCKET_ROUTE(app_, "/")
        .max_payload(kMaxPayloadSizeBytes)
        .onaccept(std::bind(&WsServer::AcceptHandler, this, _1, _2))
        .onopen(std::bind(&WsServer::OpenHandler, this, _1))
        .onclose(std::bind(&WsServer::CloseHandler, this, _1))
        .onmessage(std::bind(&WsServer::MessageHandler, this, _1, _2, _3))
        .onerror(std::bind(&WsServer::ErrorHandler, this, _1, _2));

    run_future_ = app_.bindaddr(address).port(port).multithreaded().run_async();
    app_.wait_for_server_start();
}

WsServer::~WsServer() {
    try {
        if (run_future_.valid()) {
            app_.stop();
            run_future_.wait();
        }
    } catch (std::exception& e) {
        CROW_LOG_INFO << "~WsServer(): exception: " << e.what();
    }
}

bool WsServer::AcceptHandler(const crow::request& /*req*/, void** /*userdata*/) {
    auto lock = std::lock_guard(capacity_guard_);
    if (capacity_ >= kMaxCapacity) {
        CROW_LOG_INFO << "AcceptHandler(): Can't accept connection, capacity exceeded";
        return false;
    }

    ++capacity_;
    CROW_LOG_INFO << "AcceptHandler(): Accept connection, capacity: " << capacity_;
    return true;
}

void WsServer::OpenHandler(crow::websocket::connection& /*conn*/) {
    CROW_LOG_DEBUG << "OpenHandler() called";
}

void WsServer::CloseHandler(crow::websocket::connection& /*conn*/) {
    auto lock = std::lock_guard(capacity_guard_);
    --capacity_;
    CROW_LOG_INFO << "CloseHandler(): current capacity: " << capacity_;
}

void WsServer::MessageHandler(crow::websocket::connection& conn, const std::string& data, bool is_binary) {
    CROW_LOG_INFO << "MessageHandler(): message received: " << (is_binary ? "<blob>" : data);

    try {
        const auto request = MakeRequest(data);
        auto http_client = HttpClient(request->Url());
        const auto [status, body] = request->Accept(http_client);
        const auto response = MakeResponseJson(status, body);
        conn.send_text(response);
    } catch (std::exception& e) {
        const std::string err_msg = "MessageHandler(): payload processing failed: " + std::string(e.what());
        CROW_LOG_INFO << err_msg;
        conn.send_text(err_msg);
    }
}

void WsServer::ErrorHandler(crow::websocket::connection& /*conn*/, const std::string& error_message) {
    CROW_LOG_ERROR << "ErrorHandler(): error message: " << error_message;
}
