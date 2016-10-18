#ifndef FUNCTION_H
#define FUNCTION_H




#include <stdio.h>
#include <stdlib.h>

int read_N(FILE*);
int* read_matrix(FILE*, int);
void print_matrix(int*, int);
void floyd_algorithm(int*, int*, int*, int);




#endif
