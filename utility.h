#ifndef SWS_UTILS_H_
#define SWS_UTILS_H_

#define MAX_RESPONSE_BUFFER (1024*256)
#define MAX_RESPONSE_BODY_BUFFER ((1024*256)-720)

#include <string.h>

extern const int HttpMethodNum;
extern const char *HttpMethodStr[];
extern const char *IndexFile;
enum HttpMethod {
    GET = 0,
    POST = 1,
    HEAD = 2,
    PUT = 3,
    DELETE = 4,
    Invalid = 100
};

enum HttpStatus {
    OK = 200,
    NOT_FOUND = 404
};

// for response header
enum ContentSource {
    EmptyContent = 0,
    File,
    Directory,
    Other
};

struct Response {
    char http_version[16];
    enum HttpStatus status_code;
    char status_msg[16];
    char connection[16];
    char date[64];
    char server[16];
    char last_modified[256];
    char content_type[256];
    unsigned long long content_length;
    char body[MAX_RESPONSE_BODY_BUFFER];
    enum ContentSource content_source;
};


#define MAX_BODY_SIZE 1048576
#define MAX_URL_SIZE 1024
struct RequestInfo {
    char ip_address[46];
    char date[64];
    char first_line[64];
    enum HttpMethod method;
    char url_pattern[MAX_URL_SIZE];
    char body[MAX_BODY_SIZE];
};

struct ResponseInfo {
    char status_msg[16];
    unsigned long long content_length;
};


int starts_with(const char *pre, const char *str, int str_len);

#endif //SWS_UTILS_H_
