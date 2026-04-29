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
    return "Comandos disponibles: sumar, restar, multiplicar";
}

int main(int argc, char* argv[]) {
    char *commands[] = {"sumar", "restar", "multiplicar"};
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    if (argc < 2){
        fprintf(stderr, "Error: Debe introducir un comando\n%s\n", available_commands_msg());
        return 1;
    }

    char *command = argv[1];

    int len_arguments = argc - 2;

    if (!is_valid_command(command, commands, num_commands)){
        fprintf(stderr, "Error: Comando desconocido '%s'\n%s\n", command, available_commands_msg());
        return 1;
    }

    if (strcmp(command, "restar") == 0 && len_arguments != 2){
        fprintf(stderr, "Error: La operacion 'restar' requiere exactamente dos parametros\n");
        return 1;
    }

    if ((strcmp(command, "sumar") == 0 || strcmp(command, "multiplicar") == 0) && len_arguments < 2){
        fprintf(stderr, "Error: La operacion '%s' requiere por lo menos dos parametros\n", command);
        return 1;
    }

    char **num_arguments = malloc(len_arguments * sizeof(char *));
    if (num_arguments == NULL){
        fprintf(stderr, "Error: No hay memoria\n");
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        char* parameter = argv[i];

        if (!is_decimal_string(parameter)) {
            fprintf(stderr, "Error: El parametro '%s' no es un numero valido\n", parameter);
            free(num_arguments);
            return 1;
        }

        num_arguments[i - 2] = parameter;
    }

    Bigint *output = bigint_new(num_arguments[0]);
    if (output == NULL){
        fprintf(stderr, "Error: No hay memoria\n");
        free(num_arguments);
        return 1;
    }

    if (strcmp(command, "restar") == 0) {
        Bigint *b = bigint_new(num_arguments[1]);

        if (b == NULL){
            fprintf(stderr, "Error: No hay memoria\n");
            bigint_free(output);
            free(num_arguments);
            return 1;
        }

        Bigint *temp = bigint_sub(output, b);

        bigint_free(output);
        bigint_free(b);

        if (temp == NULL){
            fprintf(stderr, "Error: No hay memoria\n");
            free(num_arguments);
            return 1;
        }

        output = temp;
    } else {
        for (int j = 1; j < len_arguments; j++) {
            Bigint *new = bigint_new(num_arguments[j]);

            if (new == NULL){
                fprintf(stderr, "Error: No hay memoria\n");
                bigint_free(output);
                free(num_arguments);
                return 1;
            }

            Bigint *temp = NULL;

            if (strcmp(command, "sumar") == 0) {
                temp = bigint_add(output, new);
            } else if (strcmp(command, "multiplicar") == 0) {
                temp = bigint_mul(output, new);
            }

            bigint_free(output);
            bigint_free(new);

            if (temp == NULL){
                fprintf(stderr, "Error: No hay memoria\n");
                free(num_arguments);
                return 1;
            }

            output = temp;
        }
    }

    bigint_print(output);

    bigint_free(output);
    free(num_arguments);

    return 0;
}