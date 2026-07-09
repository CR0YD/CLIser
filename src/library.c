//
// Created by CR0YD on 10/12/25.
//
#include <library.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cliser_list_element {
    void *data;
    struct cliser_list_element *next;
} *cliser_list_element;

typedef struct cliser_list {
    cliser_list_element first;
    cliser_list_element last;
} *cliser_list;

cliser_list cliser_init_list(void) {
    return calloc(1, sizeof(struct cliser_list));
}

cliser_list cliser_add_element(cliser_list list, void *data) {
    if (!list || !data) {
        return list;
    }

    cliser_list_element element = calloc(1, sizeof(struct cliser_list_element));
    element->data = data;

    if (!list->first) {
        list->first = element;
        list->last = element;
        return list;
    }

    list->last->next = element;
    list->last = element;

    return list;
}

void cliser_free_list(cliser_list *list, void free_data(void**)) {
    if (!list || !*list || !free_data) {
        return;
    }

    cliser_list_element current = (*list)->first;
    cliser_list_element next;

    while (current) {
        next = current->next;
        free_data(&current->data);
        free(current);
        current = next;
    }

    free(*list);
    *list = NULL;
}

void cliser_free_argument(void **argument) {
    if (!argument || !*argument) {
        return;
    }

    free(*argument);
    *argument = NULL;
}

void cliser_free_option(void **option) {
    if (!option || !*option) {
        return;
    }

    free(*option);
    *option = NULL;
}

void cliser_free_subcommand(void **subcommand) {
    if (!subcommand || !*subcommand) {
        return;
    }

    cliser_free_list((cliser_list*) &((cliser_subcommand) (*subcommand))->arguments, cliser_free_argument);
    cliser_free_list((cliser_list*) &((cliser_subcommand) (*subcommand))->options, cliser_free_option);
    cliser_free_list((cliser_list*) &((cliser_subcommand) (*subcommand))->subcommands, cliser_free_subcommand);

    free(*subcommand);
    *subcommand = NULL;
}

cliser_schema cliser_create_schema(void) {
    return calloc(1, sizeof(struct cliser_schema));
}

void cliser_free_schema(cliser_schema *schema) {
    if (!schema || !*schema) {
        return;
    }

    cliser_free_list((cliser_list*) &(*schema)->subcommands, cliser_free_subcommand);

    free(*schema);
    *schema = NULL;
}

cliser_subcommand cliser_create_subcommand(char *name, char *value) {
    cliser_subcommand subcommand = calloc(1, sizeof(struct cliser_subcommand));
    subcommand->name = name;
    subcommand->value = value;
    return subcommand;
}

cliser_schema cliser_add_base_subcommand(cliser_schema schema, cliser_subcommand subcommand) {
    if (!schema || !subcommand) {
        return schema;
    }

    if (!schema->subcommands) {
        schema->subcommands = cliser_init_list();
    }

    schema->subcommands = cliser_add_element(schema->subcommands, subcommand);

    return schema;
}

cliser_subcommand cliser_add_subcommand(cliser_subcommand subcommand, cliser_subcommand new_subcommand) {
    if (!subcommand || !new_subcommand) {
        return subcommand;
    }

    if (!subcommand->subcommands) {
        subcommand->subcommands = cliser_init_list();
    }

    subcommand->subcommands = cliser_add_element(subcommand->subcommands, new_subcommand);

    return subcommand;
}

cliser_subcommand cliser_get_subcommand_by_value(cliser_list list, char *value) {
    if (!list || !value) {
        return NULL;
    }

    cliser_list_element current = list->first;
    cliser_subcommand subcommand = NULL;

    while (current) {
        subcommand = (cliser_subcommand) (current->data);
        if (strcasecmp(subcommand->value, value) == 0) {
            return subcommand;
        }

        current = current->next;
    }

    return NULL;
}

cliser_option cliser_create_option(char *name, char *value) {
    cliser_option option = calloc(1, sizeof(struct cliser_option));
    option->name = name;
    option->value = value;
    return option;
}

cliser_subcommand cliser_add_option(cliser_subcommand subcommand, cliser_option option) {
    if (!subcommand || !option) {
        return subcommand;
    }

    if (!subcommand->options) {
        subcommand->options = cliser_init_list();
    }

    subcommand->options = cliser_add_element(subcommand->options, option);

    return subcommand;
}

cliser_option cliser_get_option_by_value(cliser_list list, char *value) {
    if (!list || !value) {
        return NULL;
    }

    cliser_list_element current = list->first;
    cliser_option option = NULL;

    while (current) {
        option = (cliser_option) (current->data);
        if (strcasecmp(option->value, value) == 0) {
            return option;
        }

        current = current->next;
    }

    return NULL;
}

cliser_argument cliser_create_argument(char *name) {
    cliser_argument argument = calloc(1, sizeof(struct cliser_argument));
    argument->name = name;
    return argument;
}

cliser_subcommand cliser_add_argument(cliser_subcommand subcommand, cliser_argument argument) {
    if (!subcommand || !argument) {
        return subcommand;
    }

    if (!subcommand->arguments) {
        subcommand->arguments = cliser_init_list();
    }

    subcommand->arguments = cliser_add_element(subcommand->arguments, argument);

    return subcommand;
}

cliser_argument cliser_get_argument_by_number(cliser_list list, size_t number) {
    if (!list || !list->first) {
        return NULL;
    }

    cliser_list_element current = list->first;

    size_t i = 0;
    while (current) {
        if (i == number) {
            return current->data;
        }

        i++;
        current = current->next;
    }

    return NULL;
}

void cliser_free_result(cliser_result *result) {
    if (!result || !*result) {
        return;
    }

    cliser_result_subcommand subcommand = (*result)->subcommand, next;

    while (subcommand) {
        next = subcommand->subcommand;

        for (short i = 0; i < subcommand->options_count; i++) {
            free(subcommand->options[i]);
        }
        free(subcommand->options);

        for (short i = 0; i < subcommand->arguments_count; i++) {
            free(subcommand->arguments[i]);
        }
        free(subcommand->arguments);

        free(subcommand);
        subcommand = next;
    }

    free(*result);
    *result = NULL;
}

cliser_result cliser_parse(cliser_schema schema, int argc, char **argv) {
    if (!schema || argc == 0 || !argv) {
        return NULL;
    }

    cliser_result result = calloc(1, sizeof(struct cliser_result));
    cliser_result_subcommand result_subcommand = NULL;

    cliser_list options = NULL, arguments = NULL, subcommands = schema->subcommands;
    cliser_option option;
    cliser_argument argument;
    size_t argument_count = 0;
    cliser_subcommand subcommand;

    for (int i = 1; i < argc; i++) {
        subcommand = cliser_get_subcommand_by_value(subcommands, argv[i]);
        if (subcommand) {
            printf("### SUBCOMMAND ###\n%s: %s\n", subcommand->name, subcommand->value);

            if (!result_subcommand) {
                result_subcommand = calloc(1, sizeof(struct cliser_result_subcommand));
                result->subcommand = result_subcommand;
            } else {
                result_subcommand->subcommand = calloc(1, sizeof(struct cliser_result_subcommand));
                result_subcommand->subcommand = result_subcommand;
            }

            result_subcommand->name = subcommand->name;

            options = subcommand->options;
            arguments = subcommand->arguments;
            argument_count = 0;
            subcommands = subcommand->subcommands;
            continue;
        }

        if (!result_subcommand) {
            printf("### ERROR ###\n");
            cliser_free_result(&result);
            return NULL;
        }

        option = cliser_get_option_by_value(options, argv[i]);
        if (option) {
            if (i == argc - 1) {
                printf("### ERROR ###\n");
                cliser_free_result(&result);
                return NULL;
            }

            printf("### OPTION ###\n%s: %s\n", option->name, argv[++i]);

            cliser_result_option *dest = calloc(result_subcommand->options_count + 1, sizeof(cliser_result_option));
            memcpy(dest, result_subcommand->options, result_subcommand->options_count * sizeof(struct cliser_result_option));
            free(result_subcommand->options);
            result_subcommand->options = dest;

            cliser_result_option result_option = calloc(1, sizeof(struct cliser_result_option));
            result_option->name = option->name;
            result_option->value = argv[i];
            result_subcommand->options[result_subcommand->options_count] = result_option;
            result_subcommand->options_count++;

            continue;
        }

        argument = cliser_get_argument_by_number(arguments, argument_count);
        if (argument) {
            printf("### ARGUMENT ###\n%s: %s\n", argument->name, argv[i]);


            cliser_result_argument *dest = calloc(result_subcommand->arguments_count + 1, sizeof(cliser_result_argument));
            memcpy(dest, result_subcommand->arguments, result_subcommand->arguments_count * sizeof(struct cliser_result_argument));
            free(result_subcommand->arguments);
            result_subcommand->arguments = dest;

            cliser_result_argument result_argument = calloc(1, sizeof(struct cliser_result_argument));
            result_argument->name = argument->name;
            result_argument->value = argv[i];
            result_subcommand->arguments[result_subcommand->arguments_count] = result_argument;
            result_subcommand->arguments_count++;

            argument_count++;
            continue;
        }

        printf("### ERROR ###\n");
        cliser_free_result(&result);
        return NULL;
    }

    return result;
}
