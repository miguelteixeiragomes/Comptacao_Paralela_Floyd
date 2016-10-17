//#include "matrix_floyd.h"
#include "read_matrix.h"


int read_N(FILE* file) 
{
	int N;
	fscanf(file, "%i", &N);
	return N;
}


int* read_matrix(FILE *file, int N)
{
	// Allocate matrix pointers to values
	int *M = malloc(N*N*sizeof(int));
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


int main(int argc, char **argv)
{
	// Check file input
	if (argc < 2) {
		printf("No input file supplied\n");
		return 1;
	}

	// Open input file
	FILE *file;
	file = fopen(argv[1], "r");

	int N = read_N(file);
	int* M = read_matrix(file, N);

	print_matrix(M, N);
	return 0;
}