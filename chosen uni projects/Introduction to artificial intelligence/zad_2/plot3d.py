import matplotlib.pyplot as plt
import numpy as np


def plot3D(function, P, bounds):
    X = [p[0] for p in P]
    Y = [p[1] for p in P]
    Z = [function(p) for p in P]
    x = np.linspace(bounds[0][0], bounds[1][0], 50)
    y = np.linspace(bounds[0][1], bounds[1][1], 50)
    x, y = np.meshgrid(x, y)
    z = function((x, y))
    ax = plt.axes(projection='3d')
    ax.scatter3D(X, Y, Z, color="red")
    ax.plot_surface(x, y, z, color='blue', alpha=0.5)
    plt.show()
