#define INF 1000000000
#include "functions.h"


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


int mod(int a, int b)
{
	return ((a % b) + b) % b;
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
