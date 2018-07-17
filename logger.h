#ifndef SWS_LOGGER_H
#define SWS_LOGGER_H

#include <stdio.h>

FILE *make_logger(char *filepath);

int close_logger(FILE *logger);

int log_to(FILE *logger);

#endif //SWS_LOGGER_H
