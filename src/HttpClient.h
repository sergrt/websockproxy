#pragma once

#include "Requests.h"
#include "Response.h"

class HttpClient final {
public:
    explicit HttpClient(const std::string& url);
    HttpClient(const HttpClient&) = delete;
    HttpClient(HttpClient&&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    HttpClient& operator=(HttpClient&&) = delete;

    ~HttpClient() = default;

    Response Visit(const GetRequest& request);
    Response Visit(const HeadRequest& request);
    Response Visit(const PostRequest& request);
    Response Visit(const PutRequest& request);
    Response Visit(const DeleteRequest& request);
    Response Visit(const OptionsRequest& request);
    Response Visit(const PatchRequest& request);

private:
    httplib::Client client_;
};
