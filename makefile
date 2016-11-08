floyd: functions.c main.c
	mpicc functions.c main.c -o floyd -lm -fopenmp
