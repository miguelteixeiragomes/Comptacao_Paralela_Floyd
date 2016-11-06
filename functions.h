#ifndef FUNCTION_H
#define FUNCTION_H




#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int read_N(FILE*);
int* read_matrix(FILE*, int);
void print_matrix(int*, int);
int* read_matrix2(FILE*, int, int);
void print_matrix2(int*, int, int);
void floyd_algorithm(int*, int*, int*, int);
int check_sizes(int, int);
int mod(int, int);
void set_inf(int*, int);




#endif
