#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"
#include "utils.h"

int is_valid_command(const char *cmd, char *commands[], int num_commands){
    for (int i = 0; i < num_commands; i++){
        if (strcmp(cmd, commands[i]) == 0) return 1;
    }
    return 0;
}

char* available_commands_msg(){
    return "Comandos disponibles: sumar";
}

// -- Main --
int main(int argc, char* argv[]) {
    char *commands[] = {"sumar"};
    int num_commands = sizeof(commands) / sizeof(commands[0]);
    char *command;
    int len_arguments = argc - 2;
    char **num_arguments = malloc(len_arguments * sizeof(char *));

    // Verify command
    if (argc < 2){
        fprintf(stderr, "Error: Debe de introducir un comando\n%s\n", available_commands_msg());
        free(num_arguments);
        return 1;
    }
    command = argv[1];
    if (!is_valid_command(command, commands, num_commands)){
        fprintf(stderr, "Error: Comando desconocido '%s'\n%s\n", command, available_commands_msg());
        free(num_arguments);
        return 1;
    }

    // Verify decimal parameters
    for (int i = 2; i < argc; i++) {
        char* parameter = argv[i];
        if (!is_decimal_string(parameter)) {
            fprintf(stderr, "Error: El parametro '%s' no es un numero valido\n", parameter);
            free(num_arguments);
            return 1;
        }
        num_arguments[i - 2] = parameter;
    }

    // Error handler
    if (len_arguments <= 1){
        fprintf(stderr, "Error: La operacion '%s' requiere por lo menos dos parametros\n", command);
        free(num_arguments);
        return 1;
    }

    // Make operations
    Bigint *output = bigint_new(num_arguments[0]);
    if (output == NULL){
        fprintf(stderr, "Error: No hay memoria\n");
        bigint_free(output);
        free(num_arguments);
        return 1;
    }
    for (int j = 1; j < argc - 2; j++) {
        Bigint *new = bigint_new(num_arguments[j]);
        if (new == NULL){
            fprintf(stderr, "Error: No hay memoria\n");
            bigint_free(new);
            free(num_arguments);
            return 1;
        }
        if (strcmp(command, "sumar") == 0) {
            if (output->sign == -1 || new->sign == -1){
                fprintf(stderr, "Error: La operacion 'sumar' no acepta parametros con signos negativos\n");
                bigint_free(new);
                bigint_free(output);
                free(num_arguments);
                return 1;
            }
            Bigint *temp = bigint_add(output, new);
            bigint_free(output);
            if (temp == NULL){
                fprintf(stderr, "Error: No hay memoria\n");
                free(num_arguments);
                return 1;
            }
            output = temp;
        }
        bigint_free(new);
    }

    bigint_print(output);
    bigint_free(output);
    free(num_arguments);

    return 0;
}