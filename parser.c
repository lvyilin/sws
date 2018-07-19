#include "parser.h"
#include <time.h>

void request_parse(char *request, int len, struct RequestInfo *info) {
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime);

    sprintf(info->date, "%s", asctime(timeinfo));
    info->date[strlen(info->date)-1] = '\0';

    info->method = Invalid;
    char buffer[128];
    int i = 0;
    while (i < len && request[i] != '\r' &&request[i]!='\n' && request[i] != EOF) {
        buffer[i] = request[i];
        ++i;
    }
    buffer[i] = '\0';
    strcpy(info->first_line,buffer);

    for (int i = 0; i < HttpMethodNum; ++i) {
        if (starts_with(HttpMethodStr[i], buffer, strlen(buffer))) {
            info->method = i;
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
    strcpy(info->url_pattern, buffer2);

    // parse post body
    if (info->method == POST) {
        char *body = strstr(request, "\r\n\r\n");
        body += sizeof("\r\n\r\n");
        strcpy(info->body, body);
    }
}
