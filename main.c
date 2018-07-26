#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <unistd.h>
#include "network.h"
#include "logger.h"

const static struct argp_option options[] = {
        {0, 'c',          "dir",     OPTION_ARG_OPTIONAL, "Allow execution of CGIs from the given directory"},
        {0, 'd',          0,         OPTION_ARG_OPTIONAL, "Enter debugging mode"},
        {0, 'i',          "address", OPTION_ARG_OPTIONAL, "Bind to the given IPv4 or IPv6 address"},
        {0, 'l',          "file",    OPTION_ARG_OPTIONAL, "Log all requests to the given file"},
        {0, 'p',          "port",    OPTION_ARG_OPTIONAL, "Listen on the given por"},
        {0, ARGP_KEY_ARG, "dir", 0,                       0},
        {0}
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    char *cgi_dir;
    int debug_mode;
    char *bind_addr;
    char *log_file;
    int port;
    char *index_path;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'c':
            arguments->cgi_dir = arg;
            break;
        case 'd':
            arguments->debug_mode = 1;
            break;
        case 'i':
            arguments->bind_addr = arg;
            break;
        case 'l':
            arguments->log_file = arg;
            break;
        case 'p':
            arguments->port = atoi(arg);
            if (arguments->port == 0) {
                return ARGP_KEY_ERROR;
            }
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
                /* Too many arguments. */
                argp_usage(state);

            arguments->index_path = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1)
                /* Not enough arguments. */
                argp_usage(state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, 0, 0};

void to_absolute_path(char *path, char *buffer, char **arg) {
    if (!path) return;
    if (!is_absolute_path(path)) {
        get_absolute_path(path, buffer);
        (*arg) = buffer;
    }
}

int main(int argc, char *argv[]) {
    struct arguments arguments;

    /* Default values. */
    arguments.cgi_dir = NULL;
    arguments.debug_mode = 0;
    arguments.bind_addr = NULL;
    arguments.log_file = NULL;
    arguments.port = 8080;
    arguments.index_path = NULL;

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    char dir_buffer[1024], dir_buffer2[1024], dir_buffer3[1024];
    if (arguments.cgi_dir) {
        strcpy(dir_buffer, arguments.cgi_dir);
        arguments.cgi_dir = dir_buffer;
    }
    if (arguments.log_file) {
        strcpy(dir_buffer2, arguments.log_file);
        arguments.log_file = dir_buffer2;
    }
    if (arguments.index_path) {
        strcpy(dir_buffer3, arguments.index_path);
        arguments.index_path = dir_buffer3;
    }
    /* Make arguments path to absolute path */
    char buffer[1024], buffer2[1024], buffer3[1024];
    to_absolute_path(arguments.index_path, buffer3, &arguments.index_path);
    to_absolute_path(arguments.log_file, buffer, &arguments.log_file);
    to_absolute_path(arguments.cgi_dir, buffer2, &arguments.cgi_dir);
    printf("%s\n%s\n%s\n", arguments.cgi_dir, arguments.log_file, arguments.index_path);
    FILE *logger = NULL;
    if (arguments.log_file != NULL) {
        logger = make_logger(arguments.log_file);
    } else {
        logger = stdout;
    }

    start_listener(arguments.port, arguments.bind_addr, logger, arguments.index_path, arguments.cgi_dir,
                   arguments.debug_mode);

    close_logger(logger);
//    exit(0);
}