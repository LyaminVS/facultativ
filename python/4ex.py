import matplotlib.pyplot as plt
import numpy

A = int(input("A = "))
B = int(input("B = "))

fig, ax = plt.subplots(1, 3)

x = numpy.arange(-100, 100, 1/1)
y = A * x + B

new_x = numpy.random.rand(1, len(x))[0] + x
new_y = numpy.random.rand(1, len(y))[0] + y

A = numpy.vstack([new_x, numpy.ones(len(new_x))]).T
m, c = numpy.linalg.lstsq(A, new_y, rcond=None)[0]

ax[0].plot(new_x, m * new_x + c)
ax[0].scatter(new_x, new_y, s=2)

gauss_standard_x = numpy.random.normal(size=len(x)) + x
gauss_standard_y = numpy.random.normal(size=len(y)) + y

A = numpy.vstack([gauss_standard_x, numpy.ones(len(gauss_standard_x))]).T
m, c = numpy.linalg.lstsq(A, gauss_standard_y, rcond=None)[0]

ax[1].plot(gauss_standard_x, m * gauss_standard_x + c)
ax[1].scatter(gauss_standard_x, gauss_standard_y, s=2)

loc = float(input("loc "))
scale = float(input("scale "))

gauss_x = numpy.random.normal(loc=loc, scale=scale, size=len(x)) + x
gauss_y = numpy.random.normal(loc=loc, scale=scale, size=len(y)) + y

A = numpy.vstack([gauss_x, numpy.ones(len(gauss_x))]).T
m, c = numpy.linalg.lstsq(A, gauss_y, rcond=None)[0]

ax[2].plot(gauss_x, m * gauss_x + c)
ax[2].scatter(gauss_x, gauss_y, s=2)
plt.show()
