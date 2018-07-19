#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "response.h"
#include "fileio.h"
#include "parser.h"

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int is_valid_dir(const char *path) {
    DIR *dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    }
    return 0;
}

void exec_command(const char *command, char *output) {
    FILE *pipe = popen(command, "r");
    if (pipe != NULL) {
        char buffer[2048] = {0};
        char c;
        int i = 0;
        while ((c = getc(pipe)) != EOF) {
            buffer[i++] = c;
        }
        sprintf(output, "<html><head><title>sws</title></head><body><pre>%s</pre></body></html>", buffer);
        buffer[i] = '\0';
        pclose(pipe);
    }

}

void get_date_info(struct Response *header) {
    struct tm *gtime;
    time_t now;
    time(&now);
    gtime = gmtime(&now);
    sprintf(header->date, "%s", asctime(gtime));
    if (header->date[strlen(header->date) - 1] == '\n') {
        header->date[strlen(header->date) - 1] = '\0';
    }

}

void get_url_path(char *url, char *index_path, char *dest) {
    char dir[512];
    int cursor = 0, cur = 0;
    int url_len = strlen(url);

    while (cursor < url_len && url[cursor] != '?') {
        dir[cur++] = url[cursor++];
    }
    dir[cur] = '\0';

    strcpy(dest, index_path);
//    if (dest[strlen(dest) - 1] != '/') {
//        strcat(dest, "/");
//    }
    strcat(dest, dir);
//    if (!is_regular_file(dest)) {
//        if (dest[strlen(dest) - 1] != '/')
//            strcat(dest, "/");
//        strcat(dest, IndexFile);
//    }
//    printf("%s\n", dest);
}

int get_file_info(char *filepath, struct Response *header) {
    if (access(filepath, F_OK) != -1) {
        struct stat attr;
        stat(filepath, &attr);
        sprintf(header->last_modified, "%s", ctime(&attr.st_mtime));
        if (header->last_modified[strlen(header->last_modified) - 1] == '\n') {
            header->last_modified[strlen(header->last_modified) - 1] = '\0';
        }
        return OK;
    }
    return Not_Found;
}


void get_response(struct RequestInfo request, struct ResponseInfo *response_info, char *response, char *index_path,
                  char *cgi_path) {
    struct Response response_header;

    strcpy(response_header.http_version, HTTP_VERSION);
    strcpy(response_header.connection, CONNECTION_TYPE);
    strcpy(response_header.server, SERVER_INFO);
    get_date_info(&response_header);

    if (cgi_path != NULL && starts_with("/cgi-bin/", request.url_pattern, strlen(request.url_pattern))) {
        response_header.status_code = OK;
        char process_name[256] = {0};
        int cur = 0, cursor = strlen("/cgi-bin/");
        while (request.url_pattern[cursor] != '\0' && request.url_pattern[cursor] != ' ') {
            process_name[cur++] = request.url_pattern[cursor++];
        }
        process_name[cur] = '\0';
        char command[512];
        sprintf(command, "%s/%s 2>&1", cgi_path, process_name);
        exec_command(command, response_header.body);


    } else {
        char urlpath[1024];
        get_url_path(request.url_pattern, index_path, urlpath);
        printf("%s\n", urlpath);

        switch (request.method) {
            case GET: {
                if (is_regular_file(urlpath)) {
                    response_header.status_code = get_file_info(urlpath, &response_header);
                    if (response_header.status_code != Not_Found)
                        read_file(urlpath, response_header.body, sizeof(response_header.body));
                    response_header.content_source = File;

                } else if (is_valid_dir(urlpath)) {
                    char cmd[256];
                    sprintf(cmd, "ls -lhF %s", urlpath);
                    exec_command(cmd, response_header.body);

                    response_header.content_source = Directory;

                } else {
                    response_header.status_code = Not_Found;
                    response_header.content_source = EmptyContent;
                }
                break;
            }
            case POST: {
                sprintf(response_header.body,
                        "<html><head><title>sws post</title></head><body>"
                        "<pre>Location:%s\r\nPost data: \r\n%s\r\n</pre></body></html>",
                        urlpath,
                        request.body
                );
                response_header.status_code = OK;
                response_header.content_source = Other;
                break;
            }
            case HEAD: {
                if (is_regular_file(urlpath)) {
                    response_header.status_code = get_file_info(urlpath, &response_header);
                } else if (is_valid_dir(urlpath)) {
                    response_header.status_code = OK;
                } else {
                    response_header.status_code = Not_Found;
                }
                response_header.content_source = EmptyContent;
                break;
            }
            case PUT: {
                if (is_regular_file(urlpath)) {
                    response_header.status_code = get_file_info(urlpath, &response_header);
                    if (response_header.status_code != Not_Found) {
                        char put_data[MAX_REQUEST_BODY_SIZE];
                        parameter_parse(request.body, PutDataKey, put_data);
                        if (!write_file(urlpath, put_data)) {
                            response_header.status_code = Internal_Server_Error;
                        } else {
                            response_header.status_code = OK;
                        }
                        response_header.content_source = File;
                    } else {
                        response_header.content_source = EmptyContent;
                    }
                } else {
                    response_header.status_code = Not_Found;
                    response_header.content_source = EmptyContent;
                }
                break;
            }
            case DELETE:{
                if (is_regular_file(urlpath)) {
                    response_header.status_code = get_file_info(urlpath, &response_header);
                    if (response_header.status_code != Not_Found) {
                        if (!delete_file(urlpath)) {
                            response_header.status_code = Internal_Server_Error;
                        } else {
                            response_header.status_code = OK;
                        }
                        response_header.content_source = File;
                    } else {
                        response_header.content_source = EmptyContent;
                    }
                } else {
                    response_header.status_code = Not_Found;
                    response_header.content_source = EmptyContent;
                }
                break;
            }
            default:
                response_header.status_code = Bad_Request;
                break;
        }
    }
    strcpy(response_header.content_type, "text/html; charset=UTF-8");
    response_header.content_length = strlen(response_header.body);
    response_info->content_length = response_header.content_length;

    get_status_msg(response_header.status_code, response_header.status_msg);
    strcpy(response_info->status_msg, response_header.status_msg);

    build_response(&response_header, response);
}

void build_response(struct Response *response, char *dest) {
    if (response->status_code == OK) {
        if (response->content_source == File) {
            sprintf(dest,
                    "%s %d %s\n"
                    "Connection: %s\n"
                    "Date: %s\n"
                    "Server: %s\n"
                    "Last-Modified: %s\n"
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
                    "Date: %s\n"
                    "Server: %s\n"
                    "Content-Type: %s\n"
                    "Content-Lenght: %llu\n\n"
                    "%s",
                    response->http_version,
                    response->status_code,
                    response->status_msg,
                    response->connection,
                    response->date,
                    response->server,
                    response->content_type,
                    response->content_length,
                    response->body);
        }
    } else {
        sprintf(dest,
                "%s %d %s\n"
                "Connection: %s\n"
                "Date: %s\n"
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
        case Not_Found:
            strcpy(dest, "Not Found");
            break;
        case Bad_Request:
            strcpy(dest, "Bad Request");
            break;
        case Internal_Server_Error:
            strcpy(dest, "Internal Server Error");
            break;
    }
}


