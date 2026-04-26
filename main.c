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
    // CAMBIO PARTE 2: se agregan los nuevos comandos disponibles
    return "Comandos disponibles: sumar, restar, multiplicar";
}

int main(int argc, char* argv[]) {
    // CAMBIO PARTE 2: ahora el programa acepta sumar, restar y multiplicar
    char *commands[] = {"sumar", "restar", "multiplicar"};
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    if (argc < 2){
        fprintf(stderr, "Error: Debe introducir un comando\n%s\n", available_commands_msg());
        return 1;
    }

    char *command = argv[1];

    // CAMBIO PARTE 2: se calcula la cantidad de operandos después del comando
    int len_arguments = argc - 2;

    if (!is_valid_command(command, commands, num_commands)){
        fprintf(stderr, "Error: Comando desconocido '%s'\n%s\n", command, available_commands_msg());
        return 1;
    }

    // CAMBIO PARTE 2: restar debe recibir exactamente 2 operandos
    // Ejemplo válido: ent restar 10 3
    if (strcmp(command, "restar") == 0 && len_arguments != 2){
        fprintf(stderr, "Error: La operacion 'restar' requiere exactamente dos parametros\n");
        return 1;
    }

    // CAMBIO PARTE 2: sumar y multiplicar deben recibir al menos 2 operandos
    // Ejemplos válidos:
    // ent sumar 1 2 3
    // ent multiplicar 2 3 4
    if ((strcmp(command, "sumar") == 0 || strcmp(command, "multiplicar") == 0) && len_arguments < 2){
        fprintf(stderr, "Error: La operacion '%s' requiere por lo menos dos parametros\n", command);
        return 1;
    }

    char **num_arguments = malloc(len_arguments * sizeof(char *));
    if (num_arguments == NULL){
        fprintf(stderr, "Error: No hay memoria\n");
        return 1;
    }

    // Se validan todos los parámetros numéricos
    // CAMBIO PARTE 2: is_decimal_string debe aceptar opcionalmente '+' o '-' al inicio
    for (int i = 2; i < argc; i++) {
        char* parameter = argv[i];

        if (!is_decimal_string(parameter)) {
            fprintf(stderr, "Error: El parametro '%s' no es un numero valido\n", parameter);
            free(num_arguments);
            return 1;
        }

        num_arguments[i - 2] = parameter;
    }

    // Se crea el primer Bigint, que será el acumulador del resultado
    Bigint *output = bigint_new(num_arguments[0]);
    if (output == NULL){
        fprintf(stderr, "Error: No hay memoria\n");
        free(num_arguments);
        return 1;
    }

    // CAMBIO PARTE 2: caso especial para restar
    // Como restar acepta exactamente 2 operandos, no hace falta recorrer una lista
    if (strcmp(command, "restar") == 0) {
        Bigint *b = bigint_new(num_arguments[1]);

        if (b == NULL){
            fprintf(stderr, "Error: No hay memoria\n");
            bigint_free(output);
            free(num_arguments);
            return 1;
        }

        // CAMBIO PARTE 2: se llama a bigint_sub(a, b)
        // Representa: output - b
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
        // CAMBIO PARTE 2: sumar y multiplicar usan la misma lógica acumulativa
        // Ejemplo:
        // ent multiplicar 2 3 4
        // primero hace 2 * 3, despues el resultado * 4
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
                // CAMBIO PARTE 2: ya no se rechazan números negativos en sumar
                // bigint_add debe encargarse de manejar signos
                temp = bigint_add(output, new);
            } else if (strcmp(command, "multiplicar") == 0) {
                // CAMBIO PARTE 2: nueva operación multiplicar
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

    // La salida estándar debe contener únicamente el número resultante
    bigint_print(output);

    bigint_free(output);
    free(num_arguments);

    return 0;
}