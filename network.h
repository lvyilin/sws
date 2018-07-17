#ifndef SWS_NET_H
#define SWS_NET_H
#define MAX_REQUEST_BUFFER 2097152
#define MAX_RESPONSE_BUFFER 2097152

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "response.h"
#include "parser.h"
#include "fileio.h"
#include "utility.h"

void start_listener(int port, FILE *logger);

#endif //SWS_NET_H
