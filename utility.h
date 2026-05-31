#ifndef LARGE_NUMBER_UTILITY_H
#define LARGE_NUMBER_UTILITY_H
#include "LN.h"

#include <string>
#include <vector>
using namespace std;

int max(int, int);
int min(int, int);

bool unary(const string *);
bool binary(const string *);
bool comparator(const string *);
bool shorthand(const string *);

bool is_zero(const LN *);
int compare(const char *a, int len_a, const char *b, int len_b);

char *add(const char *a, const char *b, int len_a, int *res_length);
char *subtract(const char *a, const char *b, int len_a, int *res_length);
char *multiply(const char *a, const char *b, int len_a, int *res_length);
char *divide(const char *, int, const char *, int, int *);
char *equality_check(const char *, const char *, int);
char *clear(char *, int);

#endif
