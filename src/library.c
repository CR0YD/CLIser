//
// Created by CR0YD on 10/12/25.
//
#include <library.h>
#include <stdlib.h>

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
        schema->subcommands = calloc(1, sizeof(cliser_list));
    }

    schema->subcommands = cliser_add_element(schema->subcommands, subcommand);

    return schema;
}

cliser_subcommand cliser_add_subcommand(cliser_subcommand subcommand, cliser_subcommand new_subcommand) {
    if (!subcommand || !new_subcommand) {
        return subcommand;
    }

    if (!subcommand->subcommands) {
        subcommand->subcommands = calloc(1, sizeof(cliser_list));
    }

    subcommand->subcommands = cliser_add_element(subcommand->subcommands, new_subcommand);

    return subcommand;
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
        subcommand->options = calloc(1, sizeof(cliser_list));
    }

    subcommand->options = cliser_add_element(subcommand->options, option);

    return subcommand;
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
        subcommand->arguments = calloc(1, sizeof(cliser_list));
    }

    subcommand->arguments = cliser_add_element(subcommand->arguments, argument);

    return subcommand;
}
