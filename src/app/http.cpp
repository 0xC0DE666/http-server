#include "./http.hpp"

// ####################
// HTTP
// ####################

// METHOD
const string Http::Method::HEAD = "HEAD";
const string Http::Method::OPTIONS = "OPTIONS";
const string Http::Method::CONNECT = "CONNECT";
const string Http::Method::TRACE = "TRACE";

const string Http::Method::GET = "GET";
const string Http::Method::POST = "POST";
const string Http::Method::PUT = "PUT";
const string Http::Method::PATCH = "PATCH";
const string Http::Method::DELETE = "DELETE";

const int Http::Method::MAX_LENGTH = 7;

// VERSION
const Http::Version Http::Version::v0_9 = Http::Version("HTTP/0.9", 0, 9);
const Http::Version Http::Version::v1_0 = Http::Version("HTTP/1.0", 1, 0);
const Http::Version Http::Version::v1_1 = Http::Version("HTTP/1.1", 1, 1);
const Http::Version Http::Version::v2_0 = Http::Version("HTTP/2", 2, 0);
const Http::Version Http::Version::v3_0 = Http::Version("HTTP/3", 3, 0);

// STATUS
// 400 client
const Http::Status Http::Status::BAD_REQUEST = Http::Status(400, "Bad Request");
const Http::Status Http::Status::NOT_FOUND = Http::Status(404, "Not Found");
const Http::Status Http::Status::METHOD_NOT_ALLOWED = Http::Status(405, "Method Not Allowed");
const Http::Status Http::Status::URI_TOO_LONG = Http::Status(414, "URI Too Long");
// 500 server
const Http::Status Http::Status::INTERNAL_SERVER_ERROR = Http::Status(500, "Internal Server Error");
const Http::Status Http::Status::NOT_IMPLEMENTED = Http::Status(501, "Not Implemented");
const Http::Status Http::Status::VERSION_NOT_SUPPORTED = Http::Status(505, "Version Not Supported");
