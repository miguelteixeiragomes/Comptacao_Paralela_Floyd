#define INF 1000000000


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
				C[n*i + j] = min(C[n*i + j], A[n*i + k] + B[n*k + j]);
}
