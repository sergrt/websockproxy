#include "Requests.h"

#include "HttpClient.h"
#include "Method.h"

#include <nlohmann/json.hpp>

#include <optional>
#include <stdexcept>

Request::Request(std::string url, std::string path, httplib::Headers headers)
    : url_(std::move(url)), path_(std::move(path)), headers_(std::move(headers)) {}

httplib::Headers Request::Headers() const {
    return headers_;
}

std::string Request::Path() const {
    return path_.empty() ? "/" : path_;
}

std::string Request::Url() const {
    return url_;
}

std::unique_ptr<Request> MakeRequest(const std::string& data) {
    const auto json = nlohmann::json::parse(data);
    if (!json.contains("url"))
        throw std::runtime_error("MakeRequest(): invalid json - no URL specified");

    const auto url = json["url"].get<std::string>();
    const auto path = json.value("path", "/");
    const auto method = MethodFromString(json.at("request"));

    httplib::Headers headers;
    if (json.contains("headers")) {
        for (const auto& [key, value] : json["headers"].items()) {
            headers.emplace(key, value);
        }
    }

    std::optional<Payload> payload;
    if (json.contains("body") && json.contains("content_type")) {
        payload = {json["body"].get<std::string>(), json["content_type"].get<std::string>()};
    }

    std::optional<httplib::MultipartFormDataItems> form_data;
    if (json.contains("form_data")) {
        form_data = httplib::MultipartFormDataItems{};
        for (const auto& obj : json["form_data"]) {
            httplib::MultipartFormData item;
            item.name = obj.at("name");
            item.content = obj.at("content");
            item.filename = obj.at("filename");
            item.content_type = obj.at("content_type");
            form_data->push_back(std::move(item));
        }
    }

    if (method == Method::METHOD_GET) {
        return std::make_unique<GetRequest>(url, path, headers);
    } else if (method == Method::METHOD_HEAD) {
        return std::make_unique<HeadRequest>(url, path, headers);
    } else if (method == Method::METHOD_POST) {
        if (form_data)
            return std::make_unique<PostRequest>(url, path, headers, *form_data);
        else if (payload)
            return std::make_unique<PostRequest>(url, path, headers, *payload);
        else
            return std::make_unique<PostRequest>(url, path, headers);
    } else if (method == Method::METHOD_PUT) {
        if (form_data)
            return std::make_unique<PutRequest>(url, path, headers, *form_data);
        else if (payload)
            return std::make_unique<PutRequest>(url, path, headers, *payload);
        else
            throw std::runtime_error("MakeRequest(): PUT method should put something");
    } else if (method == Method::METHOD_DELETE) {
        return std::make_unique<DeleteRequest>(url, path, headers, payload);
    } else if (method == Method::METHOD_OPTIONS) {
        return std::make_unique<OptionsRequest>(url, path, headers);
    } else if (method == Method::METHOD_PATCH) {
        return std::make_unique<PatchRequest>(url, path, headers, payload);
    } else {
        throw std::runtime_error("MakeRequest(): unhandled method");
    }

    return nullptr;
}


GetRequest::GetRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

std::pair<int, std::string> GetRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


HeadRequest::HeadRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

std::pair<int, std::string> HeadRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


PostRequest::PostRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

PostRequest::PostRequest(std::string url, std::string path, httplib::Headers headers, Payload payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

PostRequest::PostRequest(std::string url, std::string path, httplib::Headers headers, httplib::MultipartFormDataItems form_data)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithMultipartFormData(std::move(form_data)) {
}

std::pair<int, std::string> PostRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


PutRequest::PutRequest(std::string url, std::string path, httplib::Headers headers, Payload payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

PutRequest::PutRequest(std::string url, std::string path, httplib::Headers headers, httplib::MultipartFormDataItems form_data)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithMultipartFormData(std::move(form_data)) {
}

std::pair<int, std::string> PutRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


DeleteRequest::DeleteRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

std::pair<int, std::string> DeleteRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


OptionsRequest::OptionsRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

std::pair<int, std::string> OptionsRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}


PatchRequest::PatchRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

std::pair<int, std::string> PatchRequest::Accept(HttpClient& http_client) {
    return http_client.visit(*this);
}
