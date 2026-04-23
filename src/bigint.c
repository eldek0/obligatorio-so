#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"
#include "utils.h"


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


