#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


MPI_Comm* generate_row_comms(int Q, MPI_Comm cart_comm)
{
	MPI_Group cart_group;
	MPI_Comm_group(cart_comm, &cart_group);
	int **row_ranks = (int**)malloc(Q*sizeof(int*));
	MPI_Comm *row_comms = (MPI_Comm*)malloc(Q*sizeof(MPI_Comm));
	for (int i = 0;  i < Q; i++){
		row_ranks[i] = (int*)malloc(Q*sizeof(int));
		for (int j = 0;  j < Q; j++){
				int coord[2] = {i, j};
				int rank;
				MPI_Cart_rank(cart_comm, coord, &rank);
				row_ranks[i][j] = rank;
		}
		MPI_Group row_group;
		MPI_Group_incl(cart_group, Q, row_ranks[i], &row_group);
		MPI_Comm_create(cart_comm, row_group, &row_comms[i]);
	}
	return row_comms;
}

MPI_Comm* generate_col_comms(int Q, MPI_Comm cart_comm)
{
	MPI_Group cart_group;
	MPI_Comm_group(cart_comm, &cart_group);
	int **col_ranks = (int **)malloc(Q*sizeof(int*));
	MPI_Comm *col_comms = (MPI_Comm *)malloc(Q*sizeof(MPI_Comm));
	for(int i = 0; i < Q; i++){
		col_ranks[i] = (int *)malloc(Q*sizeof(int));
		for(int j = 0; j < Q; j++){
			int coord[2] = {j, i};
			int rank;
			MPI_Cart_rank(cart_comm, coord, &rank);
			col_ranks[i][j] = rank;
		}
		MPI_Group col_group;
		MPI_Group_incl(cart_group, Q, col_ranks[i], &col_group);
		MPI_Comm_create(cart_comm, col_group, &col_comms[i]);
	}
	for(int i = 0; i < Q; i++){
		printf("Collumm Comm: \n", );
		for(int j = 0; j < Q; j++){
			printf("%d ", col_ranks[i][j]);
		}
		printf("\n");
	}
	return col_comms;
}

int main(int argc, char** argv) {
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	//make cartesian grid communictor
	MPI_Comm cart_comm;
	int **sub_matrices;
	int N;
	int size_m;
	int Q = (int)sqrtf((double)world_size);
	int dims[2] = {Q, Q};
	int periods[2] = {1, 1};
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);


	MPI_Comm *row_comms = generate_row_comms(Q, cart_comm);
	MPI_Comm *col_comms = generate_col_comms(Q, cart_comm);


	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(cart_comm, &world_rank);


	  ////////////////////////////////////////////
	 // Initiation of the matrix and broadcast //
	////////////////////////////////////////////
	if (world_rank == 0){
			  /////////////////////
			//  Root process:  //
	 	//////////////////////

		// Check file input
		if (argc < 2) {
			printf("No input file supplied\n");
			return 1;
		}

		// Open input file
		FILE *file;
		file = fopen(argv[1], "r");

		N = read_N(file);
		int* M = read_matrix(file, N); // the matrix
		fclose(file);

		Q = check_sizes(N, world_size); // check if the num of processes is good.
		if (Q == 0) {
			return 1;
		}
		print_matrix(M, N); // see original matrix - depois sai

		// divide the matrix in blocks
		size_m = N/Q;
		sub_matrices = (int**)malloc(Q*Q*sizeof(int*));

		for (int m_i = 0; m_i < Q; m_i++){
			for (int m_j = 0; m_j < Q; m_j++){
				sub_matrices[Q*m_i + m_j] = (int*)malloc(size_m*size_m*sizeof(int));
				for (int i = 0; i < size_m; i++)
					for (int j = 0; j < size_m; j++)
						sub_matrices[Q*m_i + m_j][size_m*i + j] = M[N*(m_i*size_m + i) + m_j*size_m + j];
				//print_matrix(sub_matrices[Q*m_i + m_j], size_m); // to check validity of the division of the matrix
			}
		}
		free(M);
	}

	MPI_Bcast(&N,      1, MPI_INT, 0, cart_comm);
	MPI_Bcast(&Q,      1, MPI_INT, 0, cart_comm);
	MPI_Bcast(&size_m, 1, MPI_INT, 0, cart_comm);


	size_m = N/Q;
	int *row_m, *col_m, *m; // the matrices in each process for the calculation
	row_m = (int*)malloc(size_m*size_m*sizeof(int));
	col_m = (int*)malloc(size_m*size_m*sizeof(int));
	m     = (int*)malloc(size_m*size_m*sizeof(int));
	set_inf(m, size_m);

	if (world_rank == 0){
			////////////////////
		 //  Root process:  //
		/////////////////////
		m = sub_matrices[0];
		//printf("my rank: %d\n", world_rank);
		//print_matrix(m, size_m);
		for (int m_i = 0; m_i < Q; m_i++){
			for (int m_j = 0; m_j < Q; m_j++){
				int rank;
				int coord[2] = {m_i, m_j};
				MPI_Cart_rank(cart_comm, coord, &rank);
				MPI_Send(sub_matrices[Q*m_i + m_j], size_m*size_m, MPI_INT, rank, 0, cart_comm);
			}
		}
	}
	/*else{
			/////////////////////
		 // All other ranks //
		/////////////////////
		MPI_Recv(m, size_m*size_m, MPI_INT, 0, 0, cart_comm, MPI_STATUS_IGNORE);

	}
	MPI_Barrier(cart_comm);


	  ////////////////////
	 // Fox algorithm  //
	////////////////////
	int coord[2];
	int aux_coord[2];
	int my_rank, rank_row_source, rank_col_source, rank_row_dest, rank_col_dest;

	int max_iter = (((int)log((double)N)) / log(2.0) + 1);
	for (int iter = 0; iter < max_iter; iter++) {
		for (int k = 0; k < Q; k++){

			MPI_Cart_coords(cart_comm, world_rank, 2, coord);
			if (coord[1] == k)
				memcpy(row_m, m, size_m*size_m*sizeof(int));

			aux_coord[0] = coord[0];
			aux_coord[1] = k;
			int cart_root;
			MPI_Cart_rank(cart_comm, aux_coord, &cart_root);
			MPI_Group cart_group;
			MPI_Group row_group;
			MPI_Comm_group(cart_comm, &cart_group);
			MPI_Comm_group(row_comms[coord[0]], &row_group);
			int row_root;
			MPI_Group_translate_ranks(cart_group, 1, &cart_root, row_group, &row_root);
			MPI_Bcast(row_m, size_m*size_m, MPI_INT, row_root, row_comms[coord[0]]);


			MPI_Cart_coords(cart_comm, world_rank, 2, coord);
			MPI_Cart_rank(cart_comm, coord, &my_rank);
			aux_coord[0] = coord[0];
			aux_coord[1] = mod(coord[1] + 1, Q);
			MPI_Cart_rank(cart_comm, aux_coord, &rank_row_source);
			aux_coord[0] = mod(coord[0] + 1, Q);
			aux_coord[1] = coord[1];
			MPI_Cart_rank(cart_comm, aux_coord, &rank_col_source);
			aux_coord[0] = coord[0];
			aux_coord[1] = mod(coord[1] - 1, Q);
			MPI_Cart_rank(cart_comm, aux_coord, &rank_row_dest);
			aux_coord[0] = mod(coord[0] - 1, Q);
			aux_coord[1] = coord[1];
			MPI_Cart_rank(cart_comm, aux_coord, &rank_col_dest);

			//MPI_Sendrecv_replace(row_m, size_m*size_m, MPI_INT, rank_row_dest, 0, rank_row_source, 0, cart_comm, MPI_STATUS_IGNORE);
			//MPI_Sendrecv_replace(col_m, size_m*size_m, MPI_INT, rank_col_dest, 0, rank_col_source, 0, cart_comm, MPI_STATUS_IGNORE);
			//floyd_algorithm(row_m, col_m, m, size_m);
		}
		//printf("done step %d of %d\n", iter, max_iter);
	}*/





	printf("my rank: %d\n", world_rank);
	print_matrix(m, size_m);

	// Finalize the MPI environment.
	MPI_Finalize();

	free(row_m);
	free(col_m);
	free(m);
	free(sub_matrices);
	return 0;
}

// to run:
// clear; mpicc functions.c main.c -o floyd.exe -lm; mpirun -np 4 floyd.exe ex_mat.txt; rm -f floyd.exe

// run a bunch of times
// clear; mpicc functions.c main.c -o floyd.exe -lm; for i in {1..20}; do mpirun -np 4 floyd.exe ex_mat.txt; done; rm -f floyd.exe