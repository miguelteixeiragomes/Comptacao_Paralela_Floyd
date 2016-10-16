import numpy as np

def add(a, b):
    if a == 0 or b == 0:
        return 0
    return a + b

def min(a, b):
    if a == 0:
        return b
    if b == 0:
        return a
    if b < a:
        return b
    return a

def floyd_mul(a, b, c):
    for i in range(a.shape[0]):
        for j in range(a.shape[0]):
            if i != j:
                for k in range(a.shape[0]):
                    c[i, j] = min(c[i, j], add(a[i, k], b[k, j]))

M = np.array([[0, 2, 0, 5, 0, 0],
              [0, 0, 0, 0, 0, 0],
              [0, 2, 0, 0, 0, 5],
              [0, 0, 0, 0, 1, 0],
              [3, 9, 3, 0, 0, 0],
              [0, 0, 0, 0, 1, 0]])

M00 = M[:3, :3]
M01 = M[:3, 3:]
M10 = M[3:, :3]
M11 = M[3:, 3:]
Aux = np.zeros(M11.shape, M11.dtype)

'''print M
print M00
print M01
print M10
print M11'''

floyd_mul(M, M, M)
#floyd_mul(M, M, M)
#floyd_mul(M, M, M)
print M