#include "network.h"

void start_listener(int port, FILE *logger) {
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
    address.sin_addr.s_addr = INADDR_ANY;
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
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, MAX_REQUEST_BUFFER);
    printf("%s\n", buffer);

    struct RequestInfo req_info = request_parse(buffer, strlen(buffer));
    // TODO: adjust relative path
    char response_buffer[MAX_RESPONSE_BUFFER];
    struct ResponseHeader resp_header = get_response_header(req_info.url_pattern);
    switch (req_info.method) {
        case GET: {
            read_file(req_info.url_pattern, response_buffer, MAX_RESPONSE_BUFFER);

            break;
        }
        case POST:
            break;
        default:
            break;
    }


    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
}
