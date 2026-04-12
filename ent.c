#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -- Bigint struct --
typedef struct {
 char *digits; // decimal digits 0..9, stored in reverse order
 int len; // number of digits
 int sign; // 1 for positive, -1 for negative
} Bigint;

// Declarations
int is_decimal_string(const char *text);
int bigint_max_len(const Bigint *a, const Bigint *b);
char bigint_digit_at(const Bigint *n, int pos);
int char_to_number(char c);
char number_to_char(int n);

// -- Functions --

/*
   Creates a new Bigint from a decimal string
   Returns the Bigint, or NULL if the input is invalid
*/
Bigint *bigint_new(const char *text){
    if (!is_decimal_string(text)){
        return NULL;
    }

    Bigint *bigint = malloc(sizeof(Bigint));
    if (!bigint) {
        return NULL;
    }

    int start = 0;
    if (text[0] == '-'){
        bigint->sign = -1;
        start = 1;
    }
    else {
        bigint->sign = 1;
    }

    while (text[start] == '0' && text[start+1] != '\0'){
        start++;
    }

    int len = strlen(text) - start;

    // len + 1 with the \0 (char)
    char *digits = malloc((len + 1) * sizeof(char));
    if (!digits) {
        free(bigint);
        return NULL;
    }

    for (int i = 0; i < len; i++){
        digits[i] = text[start + len - 1 - i];
    }
    digits[len] = '\0';

    bigint->digits = digits;
    bigint->len = len;
    return bigint;
}

/*
    Prints the Bigint
*/
void bigint_print(const Bigint *n){
    if (n == NULL) return;
    
    if (n->sign == -1) {
        printf("-");
    }
    for (int i = n->len - 1; i >= 0; i--) {
        putchar(n->digits[i]);
    }
    putchar('\n');
}

/* 
    Frees the memory of a Bigint
*/
void bigint_free(Bigint *n){
    if (n == NULL)return;
    free(n->digits);
    free(n);
}

/* 
    Calculates the addition between two Bigints
*/
Bigint *bigint_add(const Bigint *a, const Bigint *b){
    int max_len = bigint_max_len(a, b);
    Bigint *result = malloc(sizeof(Bigint));
    if (result == NULL) return NULL; // OutOfMemory

    // +2: one extra digit for final carry, +1 for the '\0' just in case
    result->digits = malloc(max_len + 2);
    if (!result->digits) {
        free(result);
        return NULL; // OutOfMemory
    }

    result ->sign = 1;

    int carry = 0;
    int i;
    for (i = 0; i < max_len || carry; i++) {
        int suma = char_to_number(bigint_digit_at(a, i))+ char_to_number(bigint_digit_at(b, i)) + carry;

        result->digits[i] = number_to_char(suma % 10);
        carry = suma / 10;
    }

    result->digits[i] = '\0';
    result->len = i;
    return result;
}

/* 
    Verifies if a string is a valid decimal integer representation
    Returns 1 if valid, 0 otherwise
*/
int is_decimal_string(const char *text){
    if (text == NULL || *text == '\0') {
        return 0;
    }

    for (int i = 0; i<strlen(text); i++){
        char val = text[i];
        if (i == 0 && val == '-'){continue;} // If its negative

        if (!(val >= '0' && val <= '9')){
            return 0;
        }
    }
    return 1;
}

/*
    Returns the max lenght between two Bigints
*/
int bigint_max_len(const Bigint *a, const Bigint *b){
    int len_a = strlen(a->digits);
    int len_b = strlen(b->digits);
    if (len_a > len_b) return len_a;
    return len_b;
}

/*
    Returns a digit given its position. If the bigint lenght is exceded, returns 0
*/
char bigint_digit_at(const Bigint *n, int pos){
    if (pos < 0 || pos >= n->len) {
        return '0';
    }
    return n->digits[pos];  
}


int char_to_number(char c){
    return c - '0';
}

char number_to_char(int n){
    return n + '0';
}

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