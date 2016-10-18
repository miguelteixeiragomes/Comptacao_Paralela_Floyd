#include "functions.h"


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


	//////////////////////////
	//Floyd warshall testing//
	//////////////////////////

	int *m00 = malloc(N*N / 4 * sizeof(int));
	int *m01 = malloc(N*N / 4 * sizeof(int));
	int *m10 = malloc(N*N / 4 * sizeof(int));
	int *m11 = malloc(N*N / 4 * sizeof(int));

	for (int i = 0; i < N / 2; i++) {
		for (int j = 0; j < N / 2; j++) {
			m00[i*N / 2 + j] = M[i*N + j];
			m01[i*N / 2 + j] = M[i*N + j + N / 2];
			m10[i*N / 2 + j] = M[(i + N / 2)*N + j];
			m11[i*N / 2 + j] = M[(i + N / 2)*N + j + N / 2];
		}
	}

	for (int step = 0; step < 10; step++) {
		floyd_algorithm(m00, m00, m00, N / 2);
		floyd_algorithm(m01, m10, m00, N / 2);

		floyd_algorithm(m00, m01, m01, N / 2);
		floyd_algorithm(m01, m11, m01, N / 2);

		floyd_algorithm(m10, m00, m10, N / 2);
		floyd_algorithm(m11, m10, m10, N / 2);

		floyd_algorithm(m10, m01, m11, N / 2);
		floyd_algorithm(m11, m11, m11, N / 2);

		floyd_algorithm(M, M, M, N);
	}

	print_matrix(M, N);


	for (int i = 0; i < N / 2; i++) {
		for (int j = 0; j < N / 2; j++) {
			M[i*N + j] = m00[i*N / 2 + j];
			M[i*N + j + N / 2] = m01[i*N / 2 + j];
			M[(i + N / 2)*N + j] = m10[i*N / 2 + j];
			M[(i + N / 2)*N + j + N / 2] = m11[i*N / 2 + j];
		}
	}

	print_matrix(M, N);

	free(m00);
	free(m01);
	free(m10);
	free(m11);
	free(M);
	return 0;
}


printf("Can't matrix of size %d among %d processes", world_size, N);
