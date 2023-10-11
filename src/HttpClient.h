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

    ~HttpClient() = default;

    std::pair<int, std::string> Visit(const GetRequest& request);
    std::pair<int, std::string> Visit(const HeadRequest& request);
    std::pair<int, std::string> Visit(const PostRequest& request);
    std::pair<int, std::string> Visit(const PutRequest& request);
    std::pair<int, std::string> Visit(const DeleteRequest& request);
    std::pair<int, std::string> Visit(const OptionsRequest& request);
    std::pair<int, std::string> Visit(const PatchRequest& request);

private:
    httplib::Client client_;
};
