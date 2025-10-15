//
// Created by CR0YD on 10/12/25.
//
#ifndef CLISER_LIBRARY_H
#define CLISER_LIBRARY_H

typedef struct cliser_argument {
    char *name;
} *cliser_argument;

typedef struct cliser_option {
    char *name;
    char *value;
} *cliser_option;

typedef struct cliser_subcommand {
    char *name;
    char *value;
    void *arguments;
    void *options;
    void *subcommands;
} *cliser_subcommand;

typedef struct cliser_schema {
    void *subcommands;
} *cliser_schema;

typedef struct cliser_result_option {
    char *name;
    char *value;
} *cliser_result_option;

typedef struct cliser_result_argument {
    char *name;
    char *value;
} *cliser_result_argument;

typedef struct cliser_result_subcommand {
    char *name;
    cliser_result_option *options;
    short options_count;
    cliser_result_argument *arguments;
    short arguments_count;
    struct cliser_result_subcommand *subcommand;
} *cliser_result_subcommand;

typedef struct cliser_result {
    cliser_result_subcommand subcommand;
} *cliser_result ;

cliser_schema cliser_create_schema(void);

void cliser_free_schema(cliser_schema *schema);

cliser_subcommand cliser_create_subcommand(char *name, char *value);

cliser_schema cliser_add_base_subcommand(cliser_schema schema, cliser_subcommand subcommand);

cliser_subcommand cliser_add_subcommand(cliser_subcommand subcommand, cliser_subcommand new_subcommand);

cliser_option cliser_create_option(char *name, char *value);

cliser_subcommand cliser_add_option(cliser_subcommand subcommand, cliser_option option);

cliser_argument cliser_create_argument(char *name);

cliser_subcommand cliser_add_argument(cliser_subcommand subcommand, cliser_argument argument);

void cliser_free_result(cliser_result *result);

cliser_result cliser_parse(cliser_schema, int argc, char **argv);

#endif // CLISER_LIBRARY_H