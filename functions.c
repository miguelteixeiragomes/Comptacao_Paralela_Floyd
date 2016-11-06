#define INF 1000000000
#include "functions.h"
#include <stdio.h>

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
				return 0;
			}
			if ((i != j) && (M[N*i + j] == 0))
				M[N*i + j] = INF;
		}
	}
	return M;
}


int* read_matrix2(FILE *file, int N, int Q)
{
	// Allocate matrix pointers to values
	int* M = malloc(N*N*sizeof(int));
	// Read from file to matrix
	for (int I = 0; I < N; I++) {
		for (int J = 0; J < N; J++){
			int S = N/Q;
			int i = I*N + J;
			//int index = i%S  +  S*S*((i%N)/S)  +  S*((i%(N*S))/N)  +  N*S*(i/(N*S)); // plain version
			int index = i%S  +  S*(S*((i%N)/S)  +  ((i%(N*S))/N)  +  N*(i/(N*S)));

			if (!fscanf(file, "%i", &M[index])) {
				return 0;
			}
			if ((I != J) && (M[index] == 0)) {
				M[index] = INF;
			}
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


void print_matrix2(int* a, int N, int Q)
{
	FILE *f = fopen("output.txt", "w+");
	int i, j, I, S, index;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			I = i*N + j;
			S = N/Q;
			index = I%S  +  S*(S*((I%N)/S)  +  ((I%(N*S))/N)  +  N*(I/(N*S)));
			if (a[index] == INF)
				printf("%d ", 0);
				fprintf(f, "%d ", 0);
			else {
				printf("%d ", a[index]);
				fprintf(f, "%d ", a[index]);
			}
		}printf("\n");
		/*j++;
		I = i*N + j;
		S = N/Q;
		index = I%S  +  S*(S*((I%N)/S)  +  ((I%(N*S))/N)  +  N*(I/(N*S)));
		if (a[index] == INF)
			fprintf(f, "%d\n", 0);
		else
			fprintf(f, "%d\n", a[index]);*/
	}
	fclose(f);
}


int min(int a, int b)
{
	if (b < a)
		return b;
	return a;
}


void floyd_algorithm(int* A, int* B, int* C, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				C[n*i + j] = min(C[n*i + j], A[n*i + k] + B[n*k + j]);
}


void set_inf(int* a, int n)
{
	for (int i = 0; i < (n*n); i++){
		a[i] = INF;
	}
}


int check_sizes(int N, int world_size) // returns 0 for invalid sizes, else returns the side of the processors 'matrix' Q.
{
		int Q = (int)sqrtf((double)world_size);

		if (Q*Q != world_size){
			if ((Q + 1)*(Q + 1) != world_size){
				printf("Invalid number of processes. %d is not a perfect square.\n", world_size);
				return 0;
			}
			else{
				Q++;
			}
		}


		if (N % Q != 0){
			printf("The size N of the matrix must be divisable by the square root of the number of processes.\n");
			return 0;
		}
		return Q;
}
