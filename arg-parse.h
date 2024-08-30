#pragma once

#include <stdbool.h>
#include <argp.h>

typedef struct {
    bool rads;
    int precision;
    char *filename;
} Arguments;

Arguments get_default_arguments();
struct argp argp_conf();

extern const char args_doc[];
extern const char doc[];
