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
        // NotAString
        return NULL;
    }

    Bigint *bigint = malloc(sizeof(Bigint));
    if (!bigint) {
        // OutOfMemory
        return NULL;
    }

    bigint->digits = text;
    bigint->len = strlen(text);
    if (text[0] == '-'){
        bigint->sign = -1; // Negative
    }
    else {
        bigint->sign = 1; // Positive
    }
    return bigint;
}

/*
    Prints the Bigint
*/
void bigint_print(const Bigint *n); // IAN

/* 
    Frees the memory of a Bigint
*/
void bigint_free(Bigint *n); // EDU

/* 
    Calculates the addition between two Bigints
*/
Bigint *bigint_add(const Bigint *a, const Bigint *b); // IAN

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


// -- Main --
int main(int argc, char* argv[]){
    printf("%d", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    char* text = "-1232323";
    Bigint* bigint = bigint_new(text);
    if (bigint){
        for (int i = 0; i<strlen(bigint->digits); i++){
            printf("%c", bigint->digits[i]);
        }
    }

    return 0;
}