import random
import os
import time

N_s = [int(i) for i in raw_input("Matrix size = ").replace(' ', '').split(',')]
N_MAQ = input("Number of machines = ")
CPU = input("Number of cpu = ")
MIN = input("minimum tests = ")
MAX_TIME = input("maximum time per size (sec) = ")

f = open("clusterfile", "r")
cluster = [i.split('=')[0] + '=' + str(CPU) for i in f.readlines() if 'cpu' in i]
f.close()
if len(cluster) < N_MAQ:raise ValueError("Original clusterfile doesn't have enough machines.")
f = open("clusterfile_special.txt", "w")
for i in cluster:
    f.write(i + '\n')
f.close()


for N in N_s:
    f = open("test_matrix.txt", "w")
    f.write(str(N) + "\n")
    for i in range(N):
        for j in range(N):
            f.write(str(int(random.random() + .2) * random.randrange(1, 20)) + ' ')
        f.write("\n")
    f.close()


    f = open("time_data.txt", "w")
    f.close()


    os.system("make")

    init_time = time.clock()
    for i in range(MIN):
        os.system("mpirun -np %d -hostfile clusterfile_special.txt floyd test_matrix.txt >> time_data.txt" % CPU*N_MAQ)
    while (time.clock() - init_time) < MAX_TIME:
        os.system("mpirun -np %d -hostfile clusterfile_special.txt floyd test_matrix.txt >> time_data.txt" % CPU*N_MAQ)

    f = open("time_data.txt", "r")
    print str(N) + '\t' + str(sum([float(i[16:]) for i in f.readlines()]) / float(len(N_s)))
    f.close()

os.system("rm -f clusterfile_special.txt")
