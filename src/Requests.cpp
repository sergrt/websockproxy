#include "Requests.h"

#include "HttpClient.h"
#include "Method.h"

#include <nlohmann/json.hpp>

#include <stdexcept>

namespace {

httplib::Headers ExtractHeaders(const nlohmann::json& json) {
    httplib::Headers headers;
    if (json.contains("headers")) {
        for (const auto& [key, value] : json["headers"].items()) {
            headers.emplace(key, value);
        }
    }
    return headers;
}

std::optional<Payload> ExtractPayload(const nlohmann::json& json) {
    std::optional<Payload> payload;
    if (json.contains("body") && json.contains("content_type")) {
        payload = {json["body"].get<std::string>(), json["content_type"].get<std::string>()};
    }
    return payload;
}

std::optional<httplib::MultipartFormDataItems> ExtractFormData(const nlohmann::json& json) {
    std::optional<httplib::MultipartFormDataItems> form_data;
    if (json.contains("form_data")) {
        form_data = httplib::MultipartFormDataItems{};
        for (const auto& obj : json["form_data"]) {
            httplib::MultipartFormData item;
            item.name = obj.at("name");
            item.content = obj.at("content");
            item.filename = obj.value("filename", std::string());
            item.content_type = obj.at("content_type");
            form_data->push_back(std::move(item));
        }
    }
    return form_data;
}

}  // namespace

std::unique_ptr<Request> MakeRequest(const std::string& data) {
    const auto json = nlohmann::json::parse(data);
    const auto url = json.at("url").get<std::string>();
    const auto path = json.value("path", "/");
    const auto method = MethodFromString(json.at("method"));
    const auto headers = ExtractHeaders(json);
    const auto payload  =ExtractPayload(json);
    const auto form_data = ExtractFormData(json);

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


Request::Request(std::string url, std::string path, httplib::Headers headers)
    : url_(std::move(url))
    , path_(std::move(path))
    , headers_(std::move(headers)) {
}

httplib::Headers Request::Headers() const {
    return headers_;
}

std::string Request::Path() const {
    return path_.empty() ? "/" : path_;
}

std::string Request::Url() const {
    return url_;
}


GetRequest::GetRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

Response GetRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}


HeadRequest::HeadRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

Response HeadRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
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

Response PostRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}


PutRequest::PutRequest(std::string url, std::string path, httplib::Headers headers, Payload payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

PutRequest::PutRequest(std::string url, std::string path, httplib::Headers headers, httplib::MultipartFormDataItems form_data)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithMultipartFormData(std::move(form_data)) {
}

Response PutRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}


DeleteRequest::DeleteRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

Response DeleteRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}


OptionsRequest::OptionsRequest(std::string url, std::string path, httplib::Headers headers)
    : Request(std::move(url), std::move(path), std::move(headers)) {
}

Response OptionsRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}


PatchRequest::PatchRequest(std::string url, std::string path, httplib::Headers headers, std::optional<Payload> payload)
    : Request(std::move(url), std::move(path), std::move(headers))
    , WithPayload(std::move(payload)) {
}

Response PatchRequest::Accept(HttpClient& http_client) {
    return http_client.Visit(*this);
}
