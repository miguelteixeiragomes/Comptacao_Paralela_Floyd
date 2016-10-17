#ifndef READ_MATRIX_H
#define READ_MATRIX_H




#include <stdio.h>
#include <stdlib.h>
#include "matrix_floyd.h"

int read_N(FILE*);
int* read_matrix(FILE*, int);
void print_matrix(int*, int);




#endif