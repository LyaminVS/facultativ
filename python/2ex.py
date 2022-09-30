import numpy as np
import matplotlib.pyplot as plt

n = int(input("отношение частот: "))
phi = int(input("фаза: "))

t = np.arange(0, 2 * np.pi, np.pi/1000)[:-1]
x = np.sin(t)
y = np.cos(n * t + phi)
# print(np.sin(np.pi), np.cos(n * np.pi), np.sin(0), np.cos(0))
fig, ax = plt.subplots(1, 2)

ax[0].plot(x, y)

arr_for_hist = np.array([])

for i in range(len(x)):
    for j in range(i + 1, len(x)):
        x_elem_1 = x[i]
        x_elem_2 = x[j]
        y_elem_1 = y[i]
        y_elem_2 = y[j]
        if (np.abs(x_elem_1 - x_elem_2) < 0.000000000001) and (np.abs(y_elem_1 - y_elem_2) < 0.000000000001):

            arr_for_hist = np.append(arr_for_hist, x_elem_1)
            print(arr_for_hist)

ax[1].hist(arr_for_hist)

plt.show()

