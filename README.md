# websockproxy
Simple WebSocket HTTP server
<!--
# Table of contents
   * [Description](#description)
   * [Compilation](#compilation)
   * [Configuration](#configuration)
   * [JSON request format](#json-request-format)
   * [Testing](#testing)
   * [Request examples](#request-examples)
     * [GET](#get)
     * [HEAD](#head)
     * [POST](#post)
        * [With body data](#with-body-data)
        * [With parameters](#with-parameters)
        * [With files](#with-files)
        * [With form data](#with-form-data)
     * [PUT](#put)
        * [With body data](#with-body-data-1)
        * [With form data](#with-form-data-1)
     * [DELETE](#delete)
     * [OPTIONS](#options)
     * [PATCH](#patch)
-->
## Description
This application starts WebSocket server and processes client requests in a "proxy" manner. Client sends json-formatted request, and this server performs HTTP request as described in json. As soon as request executed result is returned to the client.

NB that this is more like "proof-of-concept" sandbox.

## Compilation
C++17 compatible compiler is required. Both Linux and Windows systems are supported. Don't forget to clone with submodules:
```
$ git clone --recurse-submodules https://github.com/sergrt/websockproxy.git
$ cd websockproxy
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

## Configuration
There's not so much to configure:
- Set `kBindAddress` to specify bind address (default is `127.0.0.1`)
- Set `kPort` to specify port (default is `18080`)
- Set `kMaxCapacity` to specify max clients allowed (default is `16`)
- Set `kMaxPayloadSizeBytes` t ospecify max payload for WebSocket server (default is `65536` bytes)

## JSON request format
Json object has required and optional fields:
- url - _required_ - URL, without trailing slash
- path - path, for example, `/get`. Default value = `/`. Also takes parameters, like `/get?name=value`
- method - _required_ - HTTP method, one of the following:
  - GET
  - HEAD
  - POST
  - PUT
  - DELETE
  - OPTIONS
  - PATCH
- headers - object with key-value pairs of headers
- body - request body (where applicable)
- content_type - body content type
- form_data - array of object with multiform data (where applicable):
  - name
  - content
  - filename
  - content_type

Extra fields, if not needed (e. g.  `body` for `HEAD` request) are omitted.

Note that some requests have required data. For example, PUT request cannot be performed without eiter `body` or `form_data` parameters supplied.

## Testing
Testing can be performed using [websocat](https://github.com/vi/websocat) client and [http://httpbin.org](http://httpbin.org) website:
- https://httpbin.org/anything Returns most of the below.
- https://httpbin.org/ip Returns Origin IP.
- https://httpbin.org/user-agent Returns user-agent.
- https://httpbin.org/headers Returns header dict.
- https://httpbin.org/get Returns GET data.
- https://httpbin.org/post Returns POST data.
- https://httpbin.org/put Returns PUT data.
- https://httpbin.org/delete Returns DELETE data
- https://httpbin.org/gzip Returns gzip-encoded data.
- https://httpbin.org/status/:code Returns given HTTP Status code.
- https://httpbin.org/response-headers?key=val Returns given response headers.
- https://httpbin.org/redirect/:n 302 Redirects n times.
- https://httpbin.org/relative-redirect/:n 302 Relative redirects n times.
- https://httpbin.org/cookies Returns cookie data.
- https://httpbin.org/cookies/set/:name/:value Sets a simple cookie.
- https://httpbin.org/basic-auth/:user/:passwd Challenges HTTPBasic Auth.
- https://httpbin.org/hidden-basic-auth/:user/:passwd 404'd BasicAuth.
- https://httpbin.org/digest-auth/:qop/:user/:passwd Challenges HTTP Digest Auth.
- https://httpbin.org/stream/:n Streams n–100 lines.
- https://httpbin.org/delay/:n Delays responding for n–10 seconds.

## Request examples
Some examples of requests:

### `GET`
```json
{
    "url": "http://httpbin.org",
    "path": "/get",
    "method": "GET"
}
```
```json
{
    "url": "http://httpbin.org",
    "path": "/get",
    "method": "GET",
    "headers": {
        "H1": "V1",
        "H2": "V2"
    }
}
```

### `HEAD`
```json
{
    "url": "http://httpbin.org",
    "path": "/",
    "method": "HEAD",
    "headers": {
        "H1": "V1",
        "H2": "V2"
    }
}
```

### `POST`
#### With body data
```json
{
    "url": "http://httpbin.org",
    "path": "/post",
    "method": "POST",
    "body": "[DEBUG] Post body",
    "content_type": "text/plain"
}
```
#### With parameters
```json
{
    "url": "http://httpbin.org",
    "path": "/post",
    "method": "POST",
    "body": "type=debug&data=post_debug",
    "content_type": "application/x-www-form-urlencoded"
}
```
#### With files
```json
{
    "url": "http://httpbin.org",
    "path": "/post",
    "method": "POST",
    "form_data": [
        {
            "name": "name1",
            "content": "content1",
            "filename": "fname1",
            "content_type": "text/plain"
        },
        {
            "name": "name2",
            "content": "content2",
            "filename": "fname2",
            "content_type": "image/jpeg"
        }
    ]
}
```
#### With form data
```json
{
    "url": "http://httpbin.org",
    "path": "/post",
    "method": "POST",
    "form_data": [
        {
            "name": "name1",
            "content": "content1",
            "content_type": "text/plain"
        },
        {
            "name": "name2",
            "content": "content2",
            "content_type": "image/jpeg"
        }
    ]
}
```

### `PUT`
#### With body data
```json
{
    "url": "http://httpbin.org",
    "path": "/put",
    "method": "PUT",
    "body": "[DEBUG] Put body",
    "content_type": "text/plain"
}
```
#### With form data
```json
{
    "url": "http://httpbin.org",
    "path": "/put",
    "method": "PUT",
    "form_data": [
        {
            "name": "name1",
            "content": "content1",
            "filename": "fname1",
            "content_type": "text/plain"
        },
        {
            "name": "name2",
            "content": "content2",
            "content_type": "image/jpeg"
        }
    ]
}
```

### `DELETE`
```json
{
    "url": "http://httpbin.org",
    "path": "/delete",
    "method": "DELETE",
    "body": "[DEBUG] Delete body",
    "content_type": "text/plain"
}
```

### `OPTIONS`
```json
{
    "url": "http://google.com",
    "path": "/anything",
    "method": "OPTIONS",
    "headers": {
        "H1": "V1",
        "H2": "V2"
    }
}
```
    
### `PATCH`
```json
{
    "url": "http://httpbin.org",
    "path": "/patch",
    "method": "PATCH",
    "body": "[DEBUG] Delete body",
    "content_type": "text/plain"
}
```
