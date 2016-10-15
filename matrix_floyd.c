#include "matrix_floyd.h"
#define INF 1073741823
#define N 6


int min(int a, int b)
{
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	if (b < a)
		return b;
	return a;
}


int add(int a, int b)
{
	return (1 - ((a == 0) || (b == 0))) * (a + b);
}


void floyd_algorithm(int* M, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				for (int k = 0; k < n; k++)
					M[n*i + j] = min(M[n*i + j], M[n*i + k] + M[n*k + j]);
}


void floyd_algorithm_2(int* A, int* B, int* C, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				for (int k = 0; k < n; k++)
					C[n*i + j] = min(C[n*i + j], add(A[n*i + k], B[n*k + j]));
}


void add_matrix_floyd(int* A, int* B, int* C, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[n*i + j] = min(A[n*i + j], B[n*i + j]);
}


int main(void)
{
	int m[N][N] = {{0, 2, 0, 5, 0, 0},
				   {0, 0, 0, 0, 0, 0},
				   {0, 2, 0, 0, 0, 5},
				   {0, 0, 0, 0, 1, 0},
				   {3, 9, 3, 0, 0, 0},
				   {0, 0, 0, 0, 1, 0}};
	int m00[N/2][N/2], m01[N/2][N/2], m10[N/2][N/2], m11[N/2][N/2], aux[N/2][N/2];

	/*for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (m[i][j] == 0)
				m[i][j] = INF;*/

	for (int i = 0; i < N/2; i++){
		for (int j = 0; j < N/2; j++){
			m00[i][j] = m[i][j];
			m01[i][j] = m[i + N/2][j];
			m10[i][j] = m[i][j + N/2];
			m11[i][j] = m[i + N/2][j + N/2];
		}
	}

	//floyd_algorithm_2(*m00, *m00, *m00, N/2);
	//floyd_algorithm_2(*m01, *m10, *aux, N/2);
	//add_matrix_floyd(*m00, *aux, *m00, N/2);

	/*floyd_algorithm(*m, N);
	floyd_algorithm(*m, N);
	floyd_algorithm(*m, N);*/
	//floyd_algorithm_2(*m, *m, *m, N);
	//floyd_algorithm_2(*m, *m, *m, N);
	//floyd_algorithm_2(*m, *m, *m, N);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (m[i][j] == INF)
				m[i][j] = 0;

	for (int i = 0; i < N/2; i++)
		for (int j = 0; j < N/2; j++)
			if (m00[i][j] == INF)
				m00[i][j] = 0;

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++)
			printf("%d ", m[i][j]);
		printf("\n");}
	printf("\n");

	for (int i = 0; i < N/2; i++){
		for (int j = 0; j < N/2; j++)
			printf("%d ", m00[i][j]);
		printf("\n");}
	printf("\n\n");

	return 0;
}
