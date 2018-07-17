#include "parser.h"

struct RequestInfo request_parse(char *request, int len) {
    struct RequestInfo info;
    info.method = Invalid;
    char buffer[128] = {0};
    int i = 0;
    while (i < len && request[i] != '\n' && request[i] != EOF) {
        buffer[i] = request[i];
        ++i;
    }
    buffer[i] = '\0';

    for (int i = 0; i < HttpMethodNum; ++i) {
        if (starts_with(HttpMethodStr[i], buffer, strlen(buffer))) {
            info.method = i;
            break;
        }
    }
    char buffer2[1024] = {0};
    int j = 0, k = 0;
    while (j < len && request[j] != ' ' && request[j] != EOF) {
        ++j;
    }
    ++j;
    while (j < len && request[j] != ' ' && request[j] != EOF) {
        buffer2[k++] = request[j++];
    }
    buffer2[k] = '\0';
    strcpy(info.url_pattern, buffer2);

    //TODO: post method code here
    return info;
}
