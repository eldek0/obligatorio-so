#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -- Bigint struct --
typedef struct {
 char *digits; // decimal digits 0..9, stored in reverse order
 int len; // number of digits
 int sign; // 1 for positive, -1 for negative
} Bigint;

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

    int len = strlen(text) - start + 1;

    char *digits = malloc(len * sizeof(char));
    if (!digits) {
        free(bigint);
        return NULL;
    }

    for (int i = 0; i < len; i++){
        if (i == len - 1){
            digits[i] = '\0';
            break;
        }
        digits[i] = text[start + i];
    }

    bigint->digits = digits;
    bigint->len = len;
    return bigint;
}

/*
    Prints the Bigint
*/
void bigint_print(const Bigint *n); // IAN 
    {
    if (n == NULL) return;
    
    if (n->sign == -1) {
        printf("-");
    }
    //opcion de claudio
    for (int i = n->len - 1; i >= 0; i--) {
        putchar('0' + n->digits[i]);
    }
    putchar('\n');

    //opcion de copilot
    // for (int i = n->len - 1; i >= 0; i--) {
        //printf("%c", n->digits[i]);
    //}
    }
/* 

    Frees the memory of a Bigint
*/
void bigint_free(Bigint *n); // EDU

/* 
    Calculates the addition between two Bigints
*/
Bigint *bigint_add(const Bigint *a, const Bigint *b); // IAN
    {
    int max_len = bigint_max_len(a, b);
    Bigint *result = malloc(sizeof(Bigint));
    if (result == NULL) return NULL; // OutOfMemory

    // +2: un digito extra por acarreo final, +1 por e '\0' por si acaso
    result->digits = malloc(max_len + 2);
    if (!result->digits) {
        free(result);
        return NULL; // OutOfMemory
    }

    result ->sign = 1;

    int carry = 0;
    int i;
    for (i = 0; i < max_len || carry; i++) {
        int suma = (int)bigint_digit_at(a, i) 
                    + (int)bigint_digit_at(b, i)   
                    + carry;
        result->digits[i] = (suma % 10); //copilot propone sumar un '0' para convertir a char, pero creo que es mejor dejarlo como int y convertir a char al imprimir
        carry = suma / 10;
    }

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
int bigint_max_len(const Bigint *a, const Bigint *b); // EDU

/*
    Returns a digit given its position. If the bigint lenght is exceded, returns 0
*/
char bigint_digit_at(const Bigint *n, int pos); // IAN
    {
    if (pos < 0 || pos >= n->len) {
        return 0;
    }
    return n->digits[pos];  
    }

// -- Main --
int main(int argc, char* argv[]){
    printf("%d", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    char* text = "-00001232323";
    Bigint* bigint = bigint_new(text);
    if (bigint){
        for (int i = 0; i<strlen(bigint->digits); i++){
            printf("%c", bigint->digits[i]);
        }
    }

    return 0;
}