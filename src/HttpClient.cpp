#include "HttpClient.h"

namespace {

std::pair<int, std::string> FormatResult(const httplib::Result& result) {
    if (result.error() != httplib::Error::Success)
        return {static_cast<int>(result.error()), "Failed"};

    return {result->status, result->body};
}

}  // namespace

HttpClient::HttpClient(const std::string& url)
    : client_(url) {
}

std::pair<int, std::string> HttpClient::Visit(const GetRequest& request) {
    const auto res = client_.Get(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const HeadRequest& request) {
    const auto res = client_.Head(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const PostRequest& request) {
    httplib::Result res;
    if (request.HasFormData())
        res = client_.Post(request.Path(), request.Headers(), request.FormData());
    else if (request.HasPayload())
        res = client_.Post(request.Path(), request.Headers(), request.Body(), request.ContentType());
    else
        res = client_.Post(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const PutRequest& request) {
    httplib::Result res;
    if (request.HasFormData())
        res = client_.Put(request.Path(), request.Headers(), request.FormData());
    else if (request.HasPayload())
        res = client_.Put(request.Path(), request.Headers(), request.Body(), request.ContentType());
    else
        throw std::runtime_error("visit(const PutRequest&): ill-formed PUT object");
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const DeleteRequest& request) {
    const auto res = client_.Delete(request.Path(), request.Headers(), request.Body(), request.ContentType());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const OptionsRequest& request) {
    const auto res = client_.Options(request.Path(), request.Headers());
    return FormatResult(res);
}

std::pair<int, std::string> HttpClient::Visit(const PatchRequest& request) {
    const auto res = client_.Patch(request.Path(), request.Headers(), request.Body(), request.ContentType());
    return FormatResult(res);
}
