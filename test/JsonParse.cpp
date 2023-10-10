#include "gtest/gtest.h"

#include "Method.h"
#include "Requests.h"

const std::vector<std::string> kInvalidJsonTestParams = {
    "",
    "{",
    "{}",
    "{ABC}",
    R"({"ABC"})",
    R"({"ABC" : })"
};

const std::vector<std::string> kJsonMissingRequiredTestParams = {
    R"({"path" : "/", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : "/"})",
    R"({"URL" : "http://httpbin.org", "path" : "/path", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : "PUT"})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : "PUT", "content_type" : "text/plain"})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : "PUT", "body" : "ABC"})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"content" : "content1", "filename" : "fname1", "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "filename" : "fname1", "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : "content1", "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : "content1", "filename" : "fname1"}]})",
};

const std::vector<std::string> kJsonInvalidTypesTestParams = {
    R"({"url" : 123, "path" : "/", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : 123, "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : 123})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : "PUT", "body" : 123, "content_type" : "text/plain"})",
    R"({"url" : "http://httpbin.org", "path" : "/put", "request" : "PUT", "body" : "ABC", "content_type" : 123})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : 123, "content" : "content1", "filename" : "fname1", "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : 123, "filename" : "fname1", "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : "content1", "filename" : 123, "content_type" : "text/plain"}]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : "content1", "filename" : "fname1", "content_type" : 123}]})",
};

class InvalidJsonTestFixture :public ::testing::TestWithParam<std::string> {};

TEST_P(InvalidJsonTestFixture, InvalidJson) {
    EXPECT_THROW(MakeRequest(GetParam()), std::exception);
}

INSTANTIATE_TEST_CASE_P(InvalidJson, InvalidJsonTestFixture, ::testing::ValuesIn(kInvalidJsonTestParams));
INSTANTIATE_TEST_CASE_P(JsonMissingRequired, InvalidJsonTestFixture, ::testing::ValuesIn(kJsonMissingRequiredTestParams));
INSTANTIATE_TEST_CASE_P(JsonInvalidTypes, InvalidJsonTestFixture, ::testing::ValuesIn(kJsonInvalidTypesTestParams));


const std::vector<std::string> kJsonValidTypesTestParams = {
    R"({"url" : "http://httpbin.org", "path" : "/path", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : "/path", "request" : "head"})",
    R"({"url" : "http://HTTPBIN.org", "path" : "/path", "request" : "HEAD"})",
    R"({"url" : "http://httpbin.org", "path" : "/path", "request" : "hEaD"})",
    R"({"request" : "HEAD", "url" : "http://httpbin.org", "path" : "/path"})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"name" : "ABC", "content" : "content1", "filename" : "fname1", "content_type" : "text/plain" }]})",
    R"({"url" : "http://httpbin.org", "path" : "/post", "request" : "POST",
        "form_data" : [{"content_type" : "text/plain", "name" : "ABC", "content" : "content1", "filename" : "fname1" }]})",

    R"({"form_data" : [{"name" : "ABC", "content" : "content1", "filename" : "fname1", "content_type" : "text/plain" }],
        "url" : "http://httpbin.org", "path" : "/post", "request" : "POST"})",
};

class ValidJsonTestFixture :public ::testing::TestWithParam<std::string> {};

TEST_P(ValidJsonTestFixture, ValidJson) {
    EXPECT_NO_THROW(MakeRequest(GetParam()));
}

INSTANTIATE_TEST_CASE_P(ValidJson, ValidJsonTestFixture, ::testing::ValuesIn(kJsonValidTypesTestParams));
