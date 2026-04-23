#pragma once

// -- Bigint struct --
typedef struct Bigint {
 char *digits; // decimal digits 0..9, stored in reverse order
 int len; // number of digits
 int sign; // 1 for positive, -1 for negative
} Bigint;

Bigint *bigint_new(const char *text);
void bigint_print(const Bigint *n);
void bigint_free(Bigint *n);
Bigint *bigint_add(const Bigint *a, const Bigint *b);
int bigint_max_len(const Bigint *a, const Bigint *b);
char bigint_digit_at(const Bigint *n, int pos);