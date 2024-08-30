#include <stdlib.h>
#include <argp.h>

#include "arg-parse.h"

const char args_doc[] = "";

static struct argp_option options[] = {
    {"filename",    'f', "FILENAME",            0, "vector samp filename", 1},
    {"degrees",     'd', NULL,                  0, "Use degrees instead of radians", 2},
    {"precision",   'p', "DECIMAL_PRECISION",   0, "Decimal precision (from 0 to 10)", 2},
    {0}
};

Arguments get_default_arguments(){
    Arguments args;
    args.rads = true;
    args.precision = 1;
    args.filename = NULL;
    return args;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    Arguments *args = state->input;

    switch (key) {
        case 'f': //--filename
            args->filename = arg;
            break;
        case 'd': //--degrees
            args->rads = false;
            break;
        case 'p': //--precision
            errno = 0;
            char *endptr;
            long precision = strtol(arg, &endptr, 10);

            if (errno != 0 || *endptr != '\0')
                argp_error(state, "Invalid precision conversion to int");
            if (precision < 0 || precision > 10)
                argp_error(state, "Option precision must be between 0 and 10 inclusive");

            args->precision = (int)precision;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

struct argp argp_conf(){
    struct argp argp;
    argp.options = options;
    argp.parser = parse_opt;
    argp.args_doc = args_doc;
    argp.doc = doc;
    argp.children = NULL;
    argp.help_filter = NULL;
    argp.argp_domain = NULL;

    return argp;
}
