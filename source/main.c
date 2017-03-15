#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/functions.h"

int main(int argc, char** argv) {
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	double start_time, finish_time;

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	MPI_Comm cart_comm; 												// Create cartesian grid communicator
	int **sub_matrices; 												// Pointer of pointers. Contains the submatrices
	
	int N; 																// The complete matrix (NxN) dimension
	int *M; 															// The pointer to the complete matrix
	int size_m;															// The size of the matix sent to each worker
	
	int Q = (int)sqrtf((double)world_size);								// Q parameter to dividide the workers on a square grid	
	int dims[2] = {Q, Q};												// Cartesian grid dimensions 
	int periods[2] = {0, 0};											// Cartesian grid periods

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);	// Cartesian grid initiation

	// Creation of the cartesian group
	MPI_Group cart_group;
	MPI_Comm_group(cart_comm, &cart_group);

	// Generating Rows and Columns Communicators
	MPI_Comm *row_comms = generate_row_comms(Q, cart_comm, cart_group);
	MPI_Comm *col_comms = generate_col_comms(Q, cart_comm, cart_group);

	// Get the rank of each worker
	int world_rank;
	MPI_Comm_rank(cart_comm, &world_rank);

	// The worker with rank == 0 reads the inout file and prepares
	// the division of the complete matrix
	if (world_rank == 0){
		// Check file input
		if (argc < 2) {
			printf("No input file supplied\n");
			return 1;
		}

		// Open input file
		FILE *file;
		file = fopen(argv[1], "r");

		N = read_N(file);				// Read the matrix dimension
		M = read_matrix(file, N, Q); 	// Read the submatrices in line
		fclose(file);

		// Test the Fox condition for the algorithm (N % Q = 0)
		Q = check_sizes(N, world_size); 		
		if (Q == 0) {
			printf("Fox condition failed\n");
			return 1;
		}
		// The dimension of the matrix in each worker
		size_m = N/Q;
	}

	// Start time count
	start_time = MPI_Wtime();

	// Broadcast matrix parameters to the group of workers
	MPI_Bcast(&N,      1, MPI_INT, 0, cart_comm);
	MPI_Bcast(&Q,      1, MPI_INT, 0, cart_comm);
	MPI_Bcast(&size_m, 1, MPI_INT, 0, cart_comm);

	// The matrices needed in each worker for the calculation (allocated in line)
	// the workers use their submatrice and the adjacent matrices in their 
	// row and column
	int *row_m, *col_m, *m; 
	row_m = (int*)malloc(size_m*size_m*sizeof(int));
	col_m = (int*)malloc(size_m*size_m*sizeof(int));
	m     = (int*)malloc(size_m*size_m*sizeof(int));

	// Scatter the complete matrix among the group of workers
	MPI_Scatter(M, size_m2, MPI_INT, m, size_m2, MPI_INT, 0, cart_comm);

	// Implementation of the Fox algorithm

	int coord[2];		// Variable that will contain the coordinates of the worker on the cartesian grid
	int aux_coord[2];	// Variable that will contain the coordinates of the adjacent worker on the cartesian grid

	// Auxliary varuiable for the communications
	int my_rank, rank_row_source, rank_col_source, rank_row_dest, rank_col_dest; 
	int cart_root, row_root, col_root;
	MPI_Group row_group, col_group;

	int max_iter = (((int)log((double)N)) / log(2.0) + 1);  // The number of iterations is determined by the algorithm
	for (int iter = 0; iter < max_iter; iter++) {
		for (int k = 0; k < Q; k++){ // Iterate over the number of workers

			MPI_Cart_coords(cart_comm, world_rank, 2, coord);  // Each worker obtains its coords
			if (coord[1] == k){
				memcpy(row_m, m, size_m2*sizeof(int));
			}
			if (coord[0] == k){
				memcpy(col_m, m, size_m2*sizeof(int));
			}

			// Coords of the rank to communicate with
			aux_coord[0] = coord[0];
			aux_coord[1] = k;

			// Obtain the communication ranks using the auxiliary coords and make
			// every worker broadcast it's matrix along it's row
			MPI_Cart_rank(cart_comm, aux_coord, &cart_root);
			MPI_Comm_group(row_comms[coord[0]], &row_group); 
			MPI_Group_translate_ranks(cart_group, 1, &cart_root, row_group, &row_root);
			MPI_Bcast(row_m, size_m2, MPI_INT, row_root, row_comms[coord[0]]);

			// Same as previous chunk but allon the worker's column
			aux_coord[0] = k;
			aux_coord[1] = coord[1];
			MPI_Cart_rank(cart_comm, aux_coord, &cart_root);
			MPI_Comm_group(col_comms[coord[1]], &col_group);
			MPI_Group_translate_ranks(cart_group, 1, &cart_root, col_group, &col_root);
			MPI_Bcast(col_m, size_m2, MPI_INT, col_root, col_comms[coord[1]]);

			// After each worker has recieved the necessary matrices
			// apply the Floyd-Warshall algorithm
			floyd_algorithm(row_m, col_m, m, size_m);
		}
	}

	// Gathering of the output matrices in a complete matrice of
	// dimension N at the worker with rank 0
	MPI_Gather(m, size_m2, MPI_INT, M, size_m2, MPI_INT, 0, cart_comm);

	// Accquire the time for calculation of the elapsed time
	finish_time = MPI_Wtime();

	// Rank 0 prints the output matrix
	if (world_rank == 0) {
		print_matrix(M, N, Q);
	}

	// Cleanup and print execution time
	free(row_m);
	free(col_m);
	free(m);
	if (world_rank == 0){
		printf("Execution time: %fs\n", finish_time - start_time);
		free(M);
	}

	// Finalize the MPI environment.
	MPI_Finalize();
	return 0;
}

