#ifndef SWS_UTILS_H_
#define SWS_UTILS_H_

#include <string.h>

extern const int HttpMethodNum;
extern const char *HttpMethodStr[];

enum HttpMethod {
    GET = 0,
    POST = 1,
    HEAD = 2,
    PUT = 3,
    DELETE = 4,
    Invalid = 100
};

struct ResponseHeader {
    char date[1024];
    char server[1024];
    char last_modified[1024];
    char content_type[1024];
    long long content_length;
};

int starts_with(const char *pre, const char *str, int str_len);

#endif //SWS_UTILS_H_
