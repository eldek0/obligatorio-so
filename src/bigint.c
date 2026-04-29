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
    else if (text[0] == '+'){
        bigint->sign = 1;
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
Bigint *bigint_add(const Bigint *a, const Bigint *b) {
    int max_len = bigint_max_len(a, b);
    Bigint *result = malloc(sizeof(Bigint));
    if (result == NULL) return NULL;

    result->digits = malloc(max_len + 2);
    if (!result->digits) {
        free(result);
        return NULL; // OutOfMemory
    }

    if (a->sign != b->sign) {
        int cmp = bigint_compare_abs(a, b);
        const Bigint *bigger  = (cmp >= 0) ? a : b;
        const Bigint *smaller = (cmp >= 0) ? b : a;

        result->sign = bigger->sign;

        int borrow = 0, i;
        for (i = 0; i < max_len || borrow; i++) {
            int diff = char_to_number(bigint_digit_at(bigger,  i))
                     - char_to_number(bigint_digit_at(smaller, i))
                     - borrow;
            if (diff < 0) { diff += 10; borrow = 1; }
            else            borrow = 0;
            result->digits[i] = number_to_char(diff);
        }

        while (i > 1 && result->digits[i - 1] == '0') i--;
        result->digits[i] = '\0';
        result->len = i;
        return result;
    }

    result->sign = a->sign;

    int carry = 0;
    int i;
    for (i = 0; i < max_len || carry; i++) {
        int suma = char_to_number(bigint_digit_at(a, i))
                 + char_to_number(bigint_digit_at(b, i))
                 + carry;

        result->digits[i] = number_to_char(suma % 10);
        carry = suma / 10;
    }

    result->digits[i] = '\0';
    result->len = i;
    return result;
}

/* 
    Calculates the substractions between two Bigints
*/
Bigint *bigint_sub(const Bigint *a, const Bigint *b) {
    Bigint temp_b = *b;
    temp_b.sign = -(b->sign);
    return bigint_add(a, &temp_b);
}

/*
    Calculates the multiplication between two Bigints
*/
Bigint *bigint_mul(const Bigint *a, const Bigint *b) {
    int result_len = a->len + b->len;

    Bigint *result = malloc(sizeof(Bigint));
    if (result == NULL) return NULL;

    result->digits = calloc(result_len + 1, 1); 
    if (!result->digits) {
        free(result);
        return NULL;
    }

    // Initialize digits to '0'
    for (int i = 0; i < result_len; i++)
        result->digits[i] = '0';

    result->sign = (a->sign == b->sign) ? 1 : -1;

    for (int i = 0; i < a->len; i++) {
        int carry = 0;
        int da = char_to_number(a->digits[i]);

        for (int j = 0; j < b->len || carry; j++) {
            int db = (j < b->len) ? char_to_number(b->digits[j]) : 0;
            int current = char_to_number(result->digits[i + j]);

            int prod = da * db + current + carry;

            result->digits[i + j] = number_to_char(prod % 10);
            carry = prod / 10;
        }
    }

    // Remove zeros
    int i = result_len;
    while (i > 1 && result->digits[i - 1] == '0')
        i--;

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

/*
    Compares the absolute values of two Bigints
    Returns:  1 if |a| > |b|
              0 if |a| == |b|
             -1 if |a| < |b|
*/
int bigint_compare_abs(const Bigint *a, const Bigint *b) {
    if (a->len != b->len)
        return (a->len > b->len) ? 1 : -1;

    for (int i = a->len - 1; i >= 0; i--) {
        int da = char_to_number(a->digits[i]);
        int db = char_to_number(b->digits[i]);
        if (da != db)
            return (da > db) ? 1 : -1;
    }

    return 0;
}


