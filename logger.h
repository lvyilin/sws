#ifndef SWS_LOGGER_H
#define SWS_LOGGER_H

#include <stdio.h>
#include "utility.h"

FILE *make_logger(char *filepath);
int close_logger(FILE *logger);
int log_to(struct RequestInfo *req,struct ResponseInfo *resp, FILE *logger) ;

#endif //SWS_LOGGER_H
