#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>

enum class Method {
    METHOD_GET,
    METHOD_HEAD,
    METHOD_POST,
    METHOD_PUT,
    METHOD_DELETE,
    METHOD_OPTIONS,
    METHOD_PATCH
};

inline std::string ToUpper(const std::string& str) {
    auto upper_str = str;
    std::transform(begin(upper_str), end(upper_str), begin(upper_str), ::toupper);
    return upper_str;
}

inline Method MethodFromString(const std::string& data) {
    const auto data_upper = ToUpper(data);
    if (data_upper == "GET")
        return Method::METHOD_GET;
    else if (data_upper == "HEAD")
        return Method::METHOD_HEAD;
    else if (data_upper == "POST")
        return Method::METHOD_POST;
    else if (data_upper == "PUT")
        return Method::METHOD_PUT;
    else if (data_upper == "DELETE")
        return Method::METHOD_DELETE;
    else if (data_upper == "OPTIONS")
        return Method::METHOD_OPTIONS;
    else if (data_upper == "PATCH")
        return Method::METHOD_PATCH;
    else
        throw std::runtime_error("Unhandled method from string conversion");
}
