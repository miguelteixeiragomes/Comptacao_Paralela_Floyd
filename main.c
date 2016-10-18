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

	//print_matrix(M, N);



	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);






	//






	// Finalize the MPI environment.
	MPI_Finalize();
	return 0;
}
