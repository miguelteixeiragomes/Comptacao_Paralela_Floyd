#include <stdio.h>
#include <stdlib.h>
#include "matrix_floyd.h"
#define N 6


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
				//printf("i,j,k=(%d,%d,%d) -> C=%d, %d+%d=%d => %d\n", i, j, k, C[n*i + j], A[n*i + k], B[n*k + j], A[n*i + k] + B[n*k + j], min(C[n*i + j], A[n*i + k] + B[n*k + j]));
				C[n*i + j] = min(C[n*i + j], A[n*i + k] + B[n*k + j]);
}


void print_matrix(int* a, int n)
{
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (a[i*n + j] == INF)
				printf("%d ", 0);
			else
				printf("%d ", a[i*n + j]);
		}
		printf("\n");
	}
	printf("\n");
}


int main(void)
{
	int m[N*N] = {0, 2, 0, 5, 0, 0,
				  0, 0, 0, 0, 0, 0,
				  0, 2, 0, 0, 0, 5,
				  0, 0, 0, 0, 1, 0,
				  3, 9, 3, 0, 0, 0,
				  0, 0, 0, 0, 1, 0};

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if ((i != j) && (m[N*i + j] == 0))
				m[N*i + j] = INF;

	int *m00 = malloc(N*N/4*sizeof(int));
	int *m01 = malloc(N*N/4*sizeof(int));
	int *m10 = malloc(N*N/4*sizeof(int));
	int *m11 = malloc(N*N/4*sizeof(int));

	for (int i = 0; i < N/2; i++){
		for (int j = 0; j < N/2; j++){
			m00[i*N/2 + j] = m[i*N + j];
			m01[i*N/2 + j] = m[i*N + j + N/2];
			m10[i*N/2 + j] = m[(i + N/2)*N + j];
			m11[i*N/2 + j] = m[(i + N/2)*N + j + N/2];
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

		floyd_algorithm(m, m, m, N);
	}

	print_matrix(m, N);


	for (int i = 0; i < N/2; i++){
		for (int j = 0; j < N/2; j++){
			m[i*N + j]               = m00[i*N/2 + j];
			m[i*N + j + N/2]         = m01[i*N/2 + j];
			m[(i + N/2)*N + j]       = m10[i*N/2 + j];
			m[(i + N/2)*N + j + N/2] = m11[i*N/2 + j];
		}
	}

	print_matrix(m, N);

	free(m00);
	free(m01);
	free(m10);
	free(m11);
	return 0;
}
