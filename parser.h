#ifndef SWS_PARSER_H
#define SWS_PARSER_H

#include <stdio.h>
#include "utility.h"



void request_parse(char *request, int len, struct RequestInfo *info);

void parameter_parse(char *body, char *key, char *value);
#endif //SWS_PARSER_H
