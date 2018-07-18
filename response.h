#ifndef SWS_RESPONSE_H
#define SWS_RESPONSE_H

#include "version.h"
#include "utility.h"
#include <time.h>

void get_basic_info(struct Response *header);

void get_response(struct RequestInfo request,struct ResponseInfo *response_info, char *response, char *index_path, char *cgi_path);

void get_url_path(char *url, char *index_path, char *dest);

int get_file_info(char *filepath, struct Response *header);

void build_response(struct Response *response, char *dest);

void get_status_msg(enum HttpStatus status, char *dest);

#endif //SWS_RESPONSE_H
