import random
import os
import numpy

'''
This script runs the C code with a given set of parameters 
such as the cluster specifications, composes a cluster file for
the mpicc and records the computation time of each run
'''

N_s = [int(i) for i in raw_input("Matrix size = ").replace(' ', '').split(',')]
N_MAQ = input("Number of machines = ")
CPU = input("Number of cpu = ")
MIN = input("number of tests = ")

f = open("clusterfile", "r")
cluster = [i.split('=')[0] + '=' + str(CPU) for i in f.readlines() if 'cpu' in i]
f.close()
if len(cluster) < N_MAQ:
    raise ValueError("Original clusterfile doesn't have enough machines.")
os.system("make")
print ' '


for N in N_s:
    f = open("clusterfile_special.txt", "w")
    for i in random.sample(set(cluster), N_MAQ):
        f.write(i + '\n')
    f.close()
    
    f = open("test_matrix.txt", "w")
    f.write(str(N) + "\n")
    for i in range(N):
        for j in range(N):
            f.write(str(int(random.random() + .2) * random.randrange(1, 20)) + ' ')
        f.write("\n")
    f.close()


    f = open("time_data.txt", "w")
    f.close()


    for i in range(MIN):
        os.system("mpirun -np %d -hostfile clusterfile_special.txt floyd test_matrix.txt >> time_data.txt" % CPU*N_MAQ)

    f = open("time_data.txt", "r")
    lst = numpy.array([float(i[16:-2]) for i in f.readlines()])
    print str(N) + ' ' + str(numpy.average(lst)) + ' ' + str(numpy.std(lst))
    f.close()

os.system("rm -f clusterfile_special.txt")
