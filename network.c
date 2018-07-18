#include "network.h"


in_addr_t get_binary_addr(const char *src) {
    if (src == NULL) {
        return INADDR_ANY;
    }
    //TODO: only support ipv4
    struct sockaddr_in sa;
    inet_pton(AF_INET, src, &(sa.sin_addr));
    return sa.sin_addr.s_addr;
}

void start_listener(int port, char *bind_addr, FILE *logger, char *index_path, char *cgi_path, int debug) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_REQUEST_BUFFER] = {0};
    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = get_binary_addr(bind_addr);
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    do {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, MAX_REQUEST_BUFFER);
        printf("%s\n", buffer);

        struct RequestInfo req_info = request_parse(buffer, strlen(buffer));
        char response_buffer[MAX_RESPONSE_BUFFER];
        get_response(req_info, response_buffer, index_path, cgi_path);

        send(new_socket, response_buffer, strlen(response_buffer), 0);
        printf("Response:\n%s", response_buffer);

        close(new_socket);
    } while (!debug);
}
