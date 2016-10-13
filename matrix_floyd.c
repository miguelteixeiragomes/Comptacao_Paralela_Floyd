#include <stdio.h>
#define INF 1073741823
#define N 6

int min(int a, int b)
{
	if (b < a)
		return b;
	return a;
}


void floyd_algorithm(int* M, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				for (int k = 0; k < n; k++)
					M[n*i + j] = min(M[n*i + j], M[n*i + k] + M[n*k + j]);
}


int main(void)
{
	int m[N][N] = {{0, 2, 0, 5, 0, 0},
								 {0, 0, 0, 0, 0, 0},
								 {0, 2, 0, 0, 0, 5},
								 {0, 0, 0, 0, 1, 0},
								 {3, 9, 3, 0, 0, 0},
								 {0, 0, 0, 0, 1, 0}};

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (m[i][j] == 0)
				m[i][j] = INF;

	floyd_algorithm(*m, N);
	floyd_algorithm(*m, N);
	floyd_algorithm(*m, N);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (m[i][j] == INF)
				m[i][j] = 0;

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++)
			printf("%d ", m[i][j]);
		printf("\n");}
}
