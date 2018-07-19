#ifndef SWS_NET_H
#define SWS_NET_H
#define MAX_REQUEST_BUFFER 2097152

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "response.h"
#include "parser.h"
#include "fileio.h"
#include "utility.h"

in_addr_t get_binary_addr(const char *src);
void start_listener(int port, char *bind_addr, FILE *logger, char *index_path, char *cgi_path, int debug);

#endif //SWS_NET_H
