#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <unistd.h>

static struct argp_option options[] = {
        {0, 'c',          "dir",     OPTION_ARG_OPTIONAL, "Allow execution of CGIs from the given directory"},
        {0, 'd',          0,         OPTION_ARG_OPTIONAL, "Enter debugging mode"},
        {0, 'i',          "address", OPTION_ARG_OPTIONAL, "Don't produce any output"},
        {0, 'l',          "file",    OPTION_ARG_OPTIONAL, "Log all requests to the given file"},
        {0, 'p',          "port",    OPTION_ARG_OPTIONAL, "Listen on the given por"},
        {0, ARGP_KEY_ARG, "dir", 0,                       0},
        {0}
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    char *cgi_dir;
    bool debug_mode;
    char *bind_addr;
    char *log_file;
    int port;
    char *current_path;
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
            arguments->debug_mode = true;
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

            arguments->current_path = arg;

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

int main(int argc, char **argv) {
    struct arguments arguments;

    /* Default values. */
    arguments.cgi_dir = NULL;
    arguments.debug_mode = false;
    arguments.bind_addr = NULL;
    arguments.log_file = NULL;
    arguments.port = 8080;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    arguments.current_path = cwd;

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);


    exit(0);
}