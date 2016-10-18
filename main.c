#include <mpi.h>
#include "functions.h"


int main(int argc, char** argv) {
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


	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	int size_m = N/Q;
	int *row_m, *col_m, *m; // the matrices in each process for the calculation
	row_m = (int*)malloc(size_m*size_m*sizeof(int));
	col_m = (int*)malloc(size_m*size_m*sizeof(int));
	m     = (int*)malloc(size_m*size_m*sizeof(int));


	//Root process
	if (world_rank == 0){
		int Q = check_sizes(N, world_size); // check if the num of processes is good.
		if (Q == 0) {
			return 1;
		}
		print_matrix(M, N);

		// divide the matrix in blocks
		int m_i, m_j, i, j, **matrices = (int**)malloc(Q*Q*sizeof(int*));
		for (m_i = 0; m_i < Q; m_i++){
			for (m_j = 0; m_j < Q; m_j++){
				matrices[Q*m_i + m_j] = (int*)malloc(size_m*size_m*sizeof(int));
				for (i = 0; i < size_m; i++)
					for (j = 0; j < size_m; j++)
						matrices[Q*m_i + m_j][size_m*i + j] = M[N*(m_i*size_m + i) + m_j*size_m + j];
				//print_matrix(matrices[Q*m_i + m_j], size_m); // to check validity of the division of the matrix
		}}
		free(M);




	}





	// Finalize the MPI environment.
	MPI_Finalize();
	return 0;
}
