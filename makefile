floyd.exe: functions.c main.c
	mpicc functions.c main.c -o floyd.exe -lm
