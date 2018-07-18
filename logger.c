#include "logger.h"

FILE *make_logger(char *filepath) {
    FILE *fp = fopen(filepath, "a+");
    return fp;
}

int log_to(struct RequestInfo *req,struct ResponseInfo *resp, FILE *logger) {
        fprintf(logger,
            "%s %s \"%s\" >%s %llu\n",
            req->ip_address,
            req->date,
            req->first_line,
            resp->status_msg,
            resp->content_length
    );
}

int close_logger(FILE *logger) {
    if (logger)
        return fclose(logger);
}
