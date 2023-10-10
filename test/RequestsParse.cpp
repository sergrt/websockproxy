#include "gtest/gtest.h"

#include "Method.h"
#include "Requests.h"

////////////////////////////////////////////////
// GetRequest

struct GetRequestTestParam {
    std::string json;

    struct {
        std::string url;
        std::string path;
        httplib::Headers headers;
    } expected;
};


const std::vector<GetRequestTestParam> kGetRequestTestParams = {
    {
        R"({ "url" : "http://httpbin.org", "path" : "/get", "method" : "GET", "headers" : { "A": "A1", "B" : "B1" } })",
        { "http://httpbin.org", "/get", {{"A","A1"}, {"B","B1"}} }
    },
    {
        R"({ "url" : "http://httpbin.org", "path" : "/get", "method" : "GET"})",
        { "http://httpbin.org", "/get" }
    },
    {
        R"({ "url" : "http://httpbin.org", "method" : "GET"})",
        { "http://httpbin.org", "/" }
    }
};

class GetRequestTestFixture :public ::testing::TestWithParam<GetRequestTestParam> {};

TEST_P(GetRequestTestFixture, GetRequest) {
    
    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    EXPECT_NE(dynamic_cast<GetRequest*>(request.get()), nullptr);
}

INSTANTIATE_TEST_CASE_P(GetRequestTest, GetRequestTestFixture, ::testing::ValuesIn(kGetRequestTestParams));


////////////////////////////////////////////////
// HeadRequest

using HeadRequestTestParam = GetRequestTestParam;

const std::vector<HeadRequestTestParam> kHeadRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/get", "method" : "HEAD", "headers" : { "A": "A1", "B" : "B1" } })",
        { "http://httpbin.org", "/get", {{"A","A1"}, {"B","B1"}} }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/get", "method" : "HEAD"})",
        { "http://httpbin.org", "/get" }
    },
    {
        R"({"url" : "http://httpbin.org", "method" : "HEAD"})",
        { "http://httpbin.org", "/" }
    }
};

class HeadRequestTestFixture :public ::testing::TestWithParam<HeadRequestTestParam> {};

TEST_P(HeadRequestTestFixture, HeadRequest) {

    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    EXPECT_NE(dynamic_cast<HeadRequest*>(request.get()), nullptr);
}

INSTANTIATE_TEST_CASE_P(HeadRequestTest, HeadRequestTestFixture, ::testing::ValuesIn(kHeadRequestTestParams));


////////////////////////////////////////////////
// PostRequest

struct PostRequestTestParam {
    std::string json;

    struct {
        std::string url;
        std::string path;
        httplib::Headers headers;
        std::string body;
        std::string content_type;
        httplib::MultipartFormDataItems form_data;
    } expected;
};

const std::vector<PostRequestTestParam> kPostRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/post", "method" : "POST",
            "headers" : { "A": "A1", "B" : "B1" },
            "form_data" : [
                {"name" : "ABC", "content" : "content1", "filename" : "fname1", "content_type" : "text/plain"},
                {"name" : "DEF", "content" : "content2", "filename" : "", "content_type" : "image/jpeg"}]
        })",
        {
            "http://httpbin.org", "/post",
            {{"A","A1"}, {"B","B1"}},
            "",
            "",
            {
                {"ABC", "content1", "fname1", "text/plain"},
                {"DEF", "content2", "", "image/jpeg"}
            }
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/post", "method" : "POST",
            "headers" : { "A": "A1", "B" : "B1" },
            "body" : "ABC", "content_type" : "text/plain"
        })",
        {
            "http://httpbin.org", "/post",
            {{"A","A1"}, {"B","B1"}},
            "ABC",
            "text/plain"
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/post", "method" : "POST",
            "headers" : { "A": "A1", "B" : "B1" }
        })",
        {
            "http://httpbin.org", "/post",
            {{"A","A1"}, {"B","B1"}},
        }
    },
};

class PostRequestTestFixture :public ::testing::TestWithParam<PostRequestTestParam> {};

TEST_P(PostRequestTestFixture, PostRequest) {

    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    auto post_request = dynamic_cast<PostRequest*>(request.get());
    ASSERT_NE(post_request, nullptr);
    EXPECT_EQ(post_request->Body(), expected.body);
    EXPECT_EQ(post_request->ContentType(), expected.content_type);

    ASSERT_EQ(post_request->FormData().size(), expected.form_data.size());
    for (size_t i = 0; i < post_request->FormData().size(); ++i) {
        EXPECT_EQ(post_request->FormData()[i].content, expected.form_data[i].content);
        EXPECT_EQ(post_request->FormData()[i].content_type, expected.form_data[i].content_type);
        EXPECT_EQ(post_request->FormData()[i].filename, expected.form_data[i].filename);
        EXPECT_EQ(post_request->FormData()[i].name, expected.form_data[i].name);
    }
}

INSTANTIATE_TEST_CASE_P(PostRequestTest, PostRequestTestFixture, ::testing::ValuesIn(kPostRequestTestParams));


////////////////////////////////////////////////
// PutRequest

using PutRequestTestParam = PostRequestTestParam;

const std::vector<PutRequestTestParam> kPutRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/put", "method" : "PUT",
            "headers" : { "A": "A1", "B" : "B1" },
            "form_data" : [
                {"name" : "ABC", "content" : "content1", "filename" : "fname1", "content_type" : "text/plain"},
                {"name" : "DEF", "content" : "content2", "filename" : "", "content_type" : "image/jpeg"}]
        })",
        {
            "http://httpbin.org", "/put",
            {{"A","A1"}, {"B","B1"}},
            "",
            "",
            {
                {"ABC", "content1", "fname1", "text/plain"},
                {"DEF", "content2", "", "image/jpeg"}
            }
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/put", "method" : "PUT",
            "headers" : { "A": "A1", "B" : "B1" },
            "body" : "ABC", "content_type" : "text/plain"
        })",
        {
            "http://httpbin.org", "/put",
            {{"A","A1"}, {"B","B1"}},
            "ABC",
            "text/plain"
        }
    },
};

class PutRequestTestFixture :public ::testing::TestWithParam<PutRequestTestParam> {};

TEST_P(PutRequestTestFixture, PutRequest) {
    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    auto put_request = dynamic_cast<PutRequest*>(request.get());
    ASSERT_NE(put_request, nullptr);
    EXPECT_EQ(put_request->Body(), expected.body);
    EXPECT_EQ(put_request->ContentType(), expected.content_type);
    ASSERT_EQ(put_request->FormData().size(), expected.form_data.size());
    for (size_t i = 0; i < put_request->FormData().size(); ++i) {
        EXPECT_EQ(put_request->FormData()[i].content, expected.form_data[i].content);
        EXPECT_EQ(put_request->FormData()[i].content_type, expected.form_data[i].content_type);
        EXPECT_EQ(put_request->FormData()[i].filename, expected.form_data[i].filename);
        EXPECT_EQ(put_request->FormData()[i].name, expected.form_data[i].name);
    }
}

INSTANTIATE_TEST_CASE_P(PutRequestTest, PutRequestTestFixture, ::testing::ValuesIn(kPutRequestTestParams));

////////////////////////////////////////////////
// DeleteRequest

struct DeleteRequestTestParam {
    std::string json;

    struct {
        std::string url;
        std::string path;
        httplib::Headers headers;
        std::string body;
        std::string content_type;
    } expected;
};

const std::vector<DeleteRequestTestParam> kDeleteRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/delete", "method" : "DELETE",
            "headers" : { "A": "A1", "B" : "B1" },
            "body" : "ABC", "content_type" : "text/plain"
        })",
        {
            "http://httpbin.org", "/delete",
            {{"A","A1"}, {"B","B1"}},
            "ABC", "text/plain"
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/delete", "method" : "DELETE",
            "headers" : { "A": "A1", "B" : "B1" }
        })",
        {
            "http://httpbin.org", "/delete",
            {{"A","A1"}, {"B","B1"}}
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/delete", "method" : "DELETE" })",
        {
            "http://httpbin.org", "/delete"
        }
    },
};

class DeleteRequestTestFixture :public ::testing::TestWithParam<DeleteRequestTestParam> {};

TEST_P(DeleteRequestTestFixture, DeleteRequest) {

    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    auto delete_request = dynamic_cast<DeleteRequest*>(request.get());
    ASSERT_NE(delete_request, nullptr);
    EXPECT_EQ(delete_request->Body(), expected.body);
    EXPECT_EQ(delete_request->ContentType(), expected.content_type);
}

INSTANTIATE_TEST_CASE_P(DeleteRequestTest, DeleteRequestTestFixture, ::testing::ValuesIn(kDeleteRequestTestParams));

////////////////////////////////////////////////
// OptionsRequest

using OptionsRequestTestParam = GetRequestTestParam;

const std::vector<OptionsRequestTestParam> kOptionsRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/options", "method" : "OPTIONS", "headers" : { "A": "A1", "B" : "B1" } })",
        { "http://httpbin.org", "/options", {{"A","A1"}, {"B","B1"}} }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/options", "method" : "OPTIONS"})",
        { "http://httpbin.org", "/options" }
    },
    {
        R"({"url" : "http://httpbin.org", "method" : "OPTIONS"})",
        { "http://httpbin.org", "/" }
    }
};

class OptionsRequestTestFixture :public ::testing::TestWithParam<OptionsRequestTestParam> {};

TEST_P(OptionsRequestTestFixture, OptionsRequest) {

    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    EXPECT_NE(dynamic_cast<OptionsRequest*>(request.get()), nullptr);
}

INSTANTIATE_TEST_CASE_P(OptionsRequestTest, OptionsRequestTestFixture, ::testing::ValuesIn(kOptionsRequestTestParams));


////////////////////////////////////////////////
// PatchRequest

using PatchRequestTestParam = DeleteRequestTestParam;

const std::vector<PatchRequestTestParam> kPatchRequestTestParams = {
    {
        R"({"url" : "http://httpbin.org", "path" : "/patch", "method" : "PATCH",
            "headers" : { "A": "A1", "B" : "B1" },
            "body" : "ABC", "content_type" : "text/plain"
        })",
        {
            "http://httpbin.org", "/patch",
            {{"A","A1"}, {"B","B1"}},
            "ABC", "text/plain"
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/patch", "method" : "PATCH",
            "headers" : { "A": "A1", "B" : "B1" }
        })",
        {
            "http://httpbin.org", "/patch",
            {{"A","A1"}, {"B","B1"}}
        }
    },
    {
        R"({"url" : "http://httpbin.org", "path" : "/patch", "method" : "PATCH" })",
        {
            "http://httpbin.org", "/patch"
        }
    },
};

class PatchRequestTestFixture :public ::testing::TestWithParam<PatchRequestTestParam> {};

TEST_P(PatchRequestTestFixture, PatchRequest) {

    const auto request = MakeRequest(GetParam().json);
    const auto expected = GetParam().expected;
    EXPECT_EQ(request->Url(), expected.url);
    EXPECT_EQ(request->Path(), expected.path);
    EXPECT_EQ(request->Headers(), expected.headers);
    auto patch_request = dynamic_cast<PatchRequest*>(request.get());
    ASSERT_NE(patch_request, nullptr);
    EXPECT_EQ(patch_request->Body(), expected.body);
    EXPECT_EQ(patch_request->ContentType(), expected.content_type);
}

INSTANTIATE_TEST_CASE_P(PatchRequestTest, PatchRequestTestFixture, ::testing::ValuesIn(kPatchRequestTestParams));
