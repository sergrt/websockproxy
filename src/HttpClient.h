#pragma once

#include "Requests.h"

#include <string>
#include <utility>

class HttpClient final {
public:
    explicit HttpClient(const std::string& url);
    HttpClient(const HttpClient&) = delete;
    HttpClient(HttpClient&&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    HttpClient& operator=(HttpClient&&) = delete;

    std::pair<int, std::string> visit(const GetRequest& request);
    std::pair<int, std::string> visit(const HeadRequest& request);
    std::pair<int, std::string> visit(const PostRequest& request);
    std::pair<int, std::string> visit(const PutRequest& request);
    std::pair<int, std::string> visit(const DeleteRequest& request);
    std::pair<int, std::string> visit(const OptionsRequest& request);
    std::pair<int, std::string> visit(const PatchRequest& request);

private:
    httplib::Client client_;
};
