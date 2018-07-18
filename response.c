#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "response.h"
#include "fileio.h"

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void get_basic_info(struct Response *header) {
    struct tm *gtime;
    time_t now;
    time(&now);
    gtime = gmtime(&now);
    sprintf(header->date, "%s", asctime(gtime));

}

void get_file_path(char *url, char *index_path, char *dest) {
    char dir[512];
    int cursor = 0, cur = 0;
    int url_len = strlen(url);

    while (cursor < url_len && url[cursor] != '?') {
        dir[cur++] = url[cursor++];
    }
    dir[cur] = '\0';

    strcpy(dest, index_path);
    strcat(dest, "/");
    strcat(dest, dir);
    if (!is_regular_file(dest)) {
        if (dest[strlen(dest) - 1] != '/')
            strcat(dest, "/");
        strcat(dest, IndexFile);
    }

    printf("%s\n", dest);
}

int get_file_info(char *filepath, struct Response *header) {
    if (access(filepath, F_OK) != -1) {
        struct stat attr;
        stat(filepath, &attr);
        sprintf(header->last_modified, "%s", ctime(&attr.st_mtime));
        return OK;
    }
    return NOT_FOUND;
}


void get_response(struct RequestInfo request, char *response, char *index_path, char *cgi_path) {
    struct Response response_header;

    strcpy(response_header.http_version, HTTP_VERSION);
    strcpy(response_header.connection, CONNECTION_TYPE);
    strcpy(response_header.server, SERVER_INFO);
    get_basic_info(&response_header);

    if (cgi_path != NULL && starts_with("/cgi-bin", request.url_pattern, strlen(request.url_pattern))) {
        response_header.status_code = OK;
    } else {
        char filepath[1024];
        get_file_path(request.url_pattern, index_path, filepath);

        enum HttpStatus status = get_file_info(filepath, &response_header);

        response_header.status_code = status;
        strcpy(response_header.content_type, "text/html");//TODO: check mime here

        switch (request.method) {
            case GET: {
                read_file(filepath, response_header.body, sizeof(response_header.body));
                response_header.content_length = strlen(response_header.body);
                break;
            }
            case POST:
                break;
            default:
                break;
        }
    }

    get_status_msg(response_header.status_code, response_header.status_msg);
    build_response(&response_header, response);
}

void build_response(struct Response *response, char *dest) {
    if (response->status_code == OK) {
        sprintf(dest,
                "%s %d %s\n"
                "Connection: %s\n"
                "Date: %s"
                "Server: %s\n"
                "Last-Modified: %s"
                "Content-Type: %s\n"
                "Content-Lenght: %llu\n\n"
                "%s",
                response->http_version,
                response->status_code,
                response->status_msg,
                response->connection,
                response->date,
                response->server,
                response->last_modified,
                response->content_type,
                response->content_length,
                response->body);
    } else {
        sprintf(dest,
                "%s %d %s\n"
                "Connection: %s\n"
                "Date: %s"
                "Server: %s\n\n",
                response->http_version,
                response->status_code,
                response->status_msg,
                response->connection,
                response->date,
                response->server
        );
    }

}

void get_status_msg(enum HttpStatus status, char *dest) {
    switch (status) {
        case OK:
            strcpy(dest, "OK");
            break;
        case NOT_FOUND:
            strcpy(dest, "Not Found");
    }
}