#include "logger.h"

FILE *make_logger(char *filepath) {
    FILE *fp = fopen(filepath, "a+");
    return fp;
}

int log_to(FILE *logger) {
//    fprintf(logger,"%a %t \"%r\" %>s %b");
}

int close_logger(FILE *logger) {
    return fclose(logger);
}
