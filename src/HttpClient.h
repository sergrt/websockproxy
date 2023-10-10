#pragma once

#include "Requests.h"

#include <string>
#include <utility>

class HttpClient {
public:
    std::pair<int, std::string> visit(const GetRequest& request);
    std::pair<int, std::string> visit(const HeadRequest& request);
    std::pair<int, std::string> visit(const PostRequest& request);
    std::pair<int, std::string> visit(const PutRequest& request);
    std::pair<int, std::string> visit(const DeleteRequest& request);
    std::pair<int, std::string> visit(const OptionsRequest& request);
    std::pair<int, std::string> visit(const PatchRequest& request);
};
