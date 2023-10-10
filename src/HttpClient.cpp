#include "HttpClient.h"

namespace {

std::pair<int, std::string> FormatResult(const httplib::Result& result) {
    if (result.error() != httplib::Error::Success)
        return {static_cast<int>(result.error()), "Failed"};

    return {result->status, result->body};
}

}  // namespace

std::pair<int, std::string> HttpClient::visit(const GetRequest& request) {
    httplib::Client client(request.Url());
    const auto res = client.Get(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const HeadRequest& request) {
    httplib::Client client(request.Url());
    const auto res = client.Head(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const PostRequest& request) {
    httplib::Client client(request.Url());
    httplib::Result res;
    if (request.HasFormData())
        res = client.Post(request.Path(), request.Headers(), request.FormData());
    else if (request.HasPayload())
        res = client.Post(request.Path(), request.Headers(), request.Body(), request.ContentType());
    else
        res = client.Post(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const PutRequest& request) {
    httplib::Client client(request.Url());
    httplib::Result res;
    if (request.HasFormData())
        res = client.Put(request.Path(), request.Headers(), request.FormData());
    else if (request.HasPayload())
        res = client.Put(request.Path(), request.Headers(), request.Body(), request.ContentType());
    else
        throw std::runtime_error("visit(const PutRequest&): ill-formed PUT object");
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const DeleteRequest& request) {
    httplib::Client client(request.Url());
    const auto res = client.Delete(request.Path(), request.Headers(), request.Body(), request.ContentType());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const OptionsRequest& request) {
    httplib::Client client(request.Url());
    const auto res = client.Options(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::visit(const PatchRequest& request) {
    httplib::Client client(request.Url());
    const auto res = client.Patch(request.Path(), request.Headers(), request.Body(), request.ContentType());
    return FormatResult(res);
}
