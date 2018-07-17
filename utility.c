#include <memory.h>

const int HttpMethodNum = 5;
const char *HttpMethodStr[] = {"GET", "POST", "HEAD", "PUT", "DELETE"};

int starts_with(const char *pre, const char *str, int str_len) {
    unsigned long lenpre = strlen(pre);

    return str_len < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}