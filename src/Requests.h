#pragma once
#include "httplib.h"

#include <memory>
#include <optional>
#include <string>

class HttpClient;

class Request {
public:
    Request(std::string url, std::string path, httplib::Headers headers = {});
    Request(const Request&) = default;
    Request(Request&&) = default;
    Request& operator=(const Request&) = default;
    Request& operator=(Request&&) = default;

    virtual ~Request() = default;

    virtual std::pair<int, std::string> Accept(HttpClient& http_client) = 0;

    std::string Url() const;
    std::string Path() const;
    httplib::Headers Headers() const;

private:
    std::string url_;
    std::string path_;
    httplib::Headers headers_;
};


// Request factory
std::unique_ptr<Request> MakeRequest(const std::string& data);


struct Payload {
    std::string body;
    std::string content_type;
};

// Payload mixin
class WithPayload {
public:
    explicit WithPayload(std::optional<Payload> payload = {}) : payload_(std::move(payload)) {}
    std::string Body() const {
        return payload_ ? payload_->body : std::string{};
    }
    std::string ContentType() const {
        return payload_ ? payload_->content_type : std::string{};
    }
    bool HasPayload() const {
        return payload_.has_value();
    }

protected:
    std::optional<Payload> payload_;
};


// MultipartFormData mixin
class WithMultipartFormData {
public:
    explicit WithMultipartFormData(std::optional<httplib::MultipartFormDataItems> form_data = {}) : form_data_(std::move(form_data)) {}
    httplib::MultipartFormDataItems FormData() const {
        return form_data_ ? *form_data_ : httplib::MultipartFormDataItems{};
    }
    bool HasFormData() const {
        return form_data_.has_value();
    }

protected:
    std::optional<httplib::MultipartFormDataItems> form_data_;
};

/////////////////////////////////////////////

class GetRequest final : public Request {
public:
    GetRequest(std::string url, std::string path, httplib::Headers headers);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class HeadRequest final : public Request {
public:
    HeadRequest(std::string url, std::string path, httplib::Headers headers);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class PostRequest final : public Request, public WithPayload, public WithMultipartFormData {
public:
    PostRequest(std::string url, std::string path, httplib::Headers headers);
    PostRequest(std::string url, std::string path, httplib::Headers headers, Payload payload);
    PostRequest(std::string url, std::string path, httplib::Headers headers, httplib::MultipartFormDataItems form_data);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class PutRequest final : public Request, public WithPayload, public WithMultipartFormData {
public:
    PutRequest(std::string url, std::string path, httplib::Headers headers, Payload payload);
    PutRequest(std::string url, std::string path, httplib::Headers headers, httplib::MultipartFormDataItems form_data);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class DeleteRequest final : public Request, public WithPayload {
public:
    DeleteRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class OptionsRequest final : public Request {
public:
    OptionsRequest(std::string url, std::string path, httplib::Headers headers);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};

class PatchRequest final : public Request, public WithPayload {
public:
    PatchRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload);
    std::pair<int, std::string> Accept(HttpClient& http_client) override;
};
