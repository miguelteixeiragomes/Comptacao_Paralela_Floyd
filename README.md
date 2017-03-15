# Floyd-Warshall Algorithm

The Floyd–Warshall algorithm is can be used for finding shortest paths 
in a weighted graph.

(source: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm)

# Implementation

This code was developed with the intent of solving the Floyd-Warshall alrogithm
in a multi-computer cluster. The algorithm itself is implemented in C and 
paralelized with OpenMPI. This code also includes some python scripts
for launching the code on a given cluster, analysing data,
and random matrix creation.

The matrix multiplication inherent to the algorithm is solved using the Fox
algorithm with a small tweak to improve effiency since the calculations 
involved in the F-W algorithm can be mathematically simplified beforehand.

# Compiling and executing

A makefile is included in the repo.

After compiling, the output program takes as external arguments
a file where the first line corresponds to the dimension N of the
square matrix and the subsequent lines contain the lines of the matrix.
The line elements are separated by spaces (' ')

The output will be similar to the input with the exception of the first line containing N.
