#include <stdio.h>
#include <string.h>
#include "utils.h"

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

int char_to_number(char c){
    return c - '0';
}

char number_to_char(int n){
    return n + '0';
}