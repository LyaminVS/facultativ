import numpy as np

n = int(input("размер матрицы"))
arr = np.identity(n) * 2 + (np.eye(n, k=1) + np.eye(n, k=-1)) * (-1)
vec = np.zeros((1, n))[0]
vec[0] = 100
vec[-1] = 1

answer = np.linalg.solve(arr, vec)

print(answer)



