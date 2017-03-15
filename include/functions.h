#ifndef FUNCTION_H
#define FUNCTION_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int read_N(FILE*);
int* read_matrix(FILE*, int, int);
void print_matrix(int*, int, int);
void floyd_algorithm(int*, int*, int*, int);
int check_sizes(int, int);
int mod(int, int);
void set_inf(int*, int);
MPI_Comm* generate_row_comms(int, MPI_Comm, MPI_Group);
MPI_Comm* generate_col_comms(int, MPI_Comm, MPI_Group);


#endif
