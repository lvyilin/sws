#ifndef SWS_PARSER_H
#define SWS_PARSER_H

#include <stdio.h>
#include "utility.h"

#define MAX_BODY_SIZE 1048576
#define MAX_URL_SIZE 1024

struct RequestInfo {
    enum HttpMethod method;
    char url_pattern[MAX_URL_SIZE];
    char body[MAX_BODY_SIZE];
};

struct RequestInfo request_parse(char *request, int len);

#endif //SWS_PARSER_H
