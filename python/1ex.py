import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation

m = int(input("ширина: "))
n = int(input("высота: "))

fig, ax = plt.subplots(n, m)

lines = [ax[i][j].plot([], [])[0] for i in range(n) for j in range(m)]

for i in range(n):
    for j in range(m):
        ax[i][j].set_ylim(-1.1, 1.1)
        ax[i][j].set_xlim(-1.1, 1.1)


def init():
    for line in lines:
        line.set_data([], [])
    return lines


def animate(i):
    t = np.linspace(0, 2 * np.pi, num=1000)
    x = np.sin(t)

    for num, line in enumerate(lines):
        y = np.cos((num + 1) * t + i / 40)
        line.set_data(x, y)
    return lines


anim = matplotlib.animation.FuncAnimation(fig, animate, frames=20000, interval=1, blit=True, init_func=init)


plt.show()


