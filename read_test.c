#include <stdio.h>
#include <stdlib.h>
#define INF 100


int mod(int a, int b)
{
	return ((a % b) + b) % b;
}


int read_N(FILE* file)
{
	int N;
	fscanf(file, "%i", &N);
	return N;
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


int main(int argc, char** argv) {
    // Open input file
    FILE *file;
    file = fopen(argv[1], "r");

    int Q = 2;
    int N = read_N(file);
	int size_m = N/Q;
    int *M = read_matrix2(file, N, Q); // the matrix
    fclose(file);

    for (int i = 0; i < Q*Q; i++){
		for (int j = 0; j < size_m*size_m; j++){
        	printf("%d ", M[i*size_m*size_m + j]);
		}printf("-\n");
    }
    printf("\n");
}
