import matplotlib.pyplot as plt


def plot_error(X, Error, label):
    plt.xlabel(label)
    plt.ylabel('Błąd wyniku')
    plt.plot(X, Error)
    plt.show()
