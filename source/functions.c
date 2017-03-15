#define INF 1000000000
#include "../include/functions.h"
#include <stdio.h>



/*
Function that reads the dimension of a sqaure 
matrix from a file containing the matrix info

@param FILE* 		file 	Pointer to the file that contains the matrix information
*/
int read_N(FILE* file)
{
	int N;
	fscanf(file, "%i", &N);
	return N;
}

/*
Function that reads a square matrix of int type and returns 
a pointer to said matrix. The matrix is represented in line
and prepared for scaterring with MPI scatter

@param FILE* 		file 	Pointer to the file that contains the matrix
@param int 			N 		Matrix dimension
@param int 			Q 		Number of workers
*/
int* read_matrix(FILE *file, int N, int Q)
{
	// Allocate matrix pointers to values
	int* M = malloc(N*N*sizeof(int));
	// Read from file to matrix
	for (int I = 0; I < N; I++) {
		for (int J = 0; J < N; J++){
			int S = N/Q;
			int i = I*N + J;
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

/*
Function that prints an inline matrix. The matrix indexing respects
is determined by the mpi gather method

@param int* 		a 		Pointer to matrix
@param int 			N 		Matrix dimension
@param int 			Q 		Number of workers
*/
void print_matrix(int* a, int N, int Q)
{
	FILE *f = fopen("output.txt", "w+");
	int i, j, I, S, index;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			I = i*N + j;
			S = N/Q;
			index = I%S  +  S*(S*((I%N)/S)  +  ((I%(N*S))/N)  +  N*(I/(N*S)));
			if (a[index] == INF) {
				//printf("%d ", 0);
				fprintf(f, "%d ", 0);
			}
			else {
				//printf("%d ", a[index]);
				fprintf(f, "%d ", a[index]);
			}
		}
		//printf("\n");
		I = i*N + N - 1;
		S = N/Q;
		index = I%S  +  S*(S*((I%N)/S)  +  ((I%(N*S))/N)  +  N*(I/(N*S)));
		if (a[index] == INF)
			fprintf(f, "%d\n", 0);
		else
			fprintf(f, "%d\n", a[index]);
	}
	fclose(f);
}

/*
Function that returns the minimum of two numbers

@param int 		a 	First int
@param int 		b 	Second int
*/
int min(int a, int b)
{
	if (b < a)
		return b;
	return a;
}

/*
Function that applies the Floyd-Warshall 
algorithm to a set of adjecent matrices

@param int* 		A 		First matrix
@param int* 		B 		Second matrix
@param int* 		C 		Third matrix
*/
void floyd_algorithm(int* A, int* B, int* C, int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				C[n*i + j] = min(C[n*i + j], A[n*i + k] + B[n*k + j]);
}


/*
Function that sets the values of a mtrix to INF

@param int* 	a 	Matrix
@param int 		n 	Matrix dimension
*/
void set_inf(int* a, int n)
{
	for (int i = 0; i < (n*n); i++){
		a[i] = INF;
	}
}

/*
Function that check if it is possible to solve a given matrix
in a given cluster, by analyzing the Fox condition

@param int 		N 				Matrix dimension
@param int 		world_size		Number of workers in the cluster
*/
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

/*
Function that creates an array of communicators for each row 
on the cartesian grid

@param int 			Q				Number of workers
@param MPI_Comm 	cart_comm		Cartesian grid communicator
@param MPI_Group 	cart_group		Cartesian grid group
*/
MPI_Comm* generate_row_comms(int Q, MPI_Comm cart_comm, MPI_Group cart_group)
{
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

/*
Function that creates an array of communicators for each column 
on the cartesian grid

@param int 			Q				Number of workers
@param MPI_Comm 	cart_comm		Cartesian grid communicator
@param MPI_Group 	cart_group		Cartesian grid group
*/
MPI_Comm* generate_col_comms(int Q, MPI_Comm cart_comm, MPI_Group cart_group)
{
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
	return col_comms;
}