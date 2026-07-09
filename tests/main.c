//
// Created by CR0YD on 10/12/25.
//
#include <library.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    cliser_schema schema = cliser_create_schema();
    schema = cliser_add_base_subcommand(
            schema,
            cliser_add_option(
                cliser_create_subcommand("test", "t"),
                cliser_create_option("arg", "--arg")
            )
        );
    cliser_result result = cliser_parse(schema, argc, argv);
    cliser_free_schema(&schema);
    cliser_free_result(&result);

    return EXIT_SUCCESS;
}
