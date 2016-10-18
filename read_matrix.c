#include <stdio.h>
#include <stdlib.h>
#include "matrix_floyd.h"
#define INF 1000000000


int read_N(FILE* file) 
{
	int N;
	fscanf(file, "%i", &N);
	return N;
}


int* read_matrix(FILE *file, int N)
{
	// Allocate matrix pointers to values
	int* M = malloc(N*N*sizeof(int));
	// Read from file to matrix
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!fscanf(file, "%i", &M[N*i + j])) {
				fclose(file);
				return 0;
			}
			if ((i != j) && (M[N*i + j] == 0))
				M[N*i + j] = INF;
		}
	}
	return M;
}


void print_matrix(int* a, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i*n + j] == INF)
				printf("%d ", 0);
			else
				printf("%d ", a[i*n + j]);
		}
		printf("\n");
	}
	printf("\n");
}