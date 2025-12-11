import numpy as np
from evolution import EvolutionaryAlgorithm
import matplotlib.pyplot as plt
from plot3d import plot3D
from plot_error import plot_error


def himmelblau(input) -> float:
    x, y = input
    return (x**2+y-11)**2 + (x+y**2-7)**2


def ackley(input) -> float:
    x, y = input
    exp1 = np.exp(-0.2*np.sqrt((x**2+y**2)/2))
    exp2 = np.exp((np.cos(2*np.pi*x)+np.cos(2*np.pi*y))/2)
    return -20*exp1-exp2+20+np.e


def error(vect, function):
    return function(vect)**2


def check_param_m(function):
    bounds = [(-10, -10), (-5, -5)]
    population_size = 10
    sigma = 1
    iterations = 40
    mutation_rate = 0.7
    crossover_rate = 0.3

    errors = []
    values = [x/1000 for x in range(1001)]
    for mutation_rate in values:
        ea1 = EvolutionaryAlgorithm(
            function,
            population_size,
            bounds,
            mutation_rate,
            crossover_rate,
            sigma
        )
        min_f, history_f = ea1.run(iterations)
        errors.append(error(min_f, ackley))
    plot_error(values, errors, "prawdopodbieńśtwo mutacji")


def check_param_c(function):
    bounds = [(-10, -10), (-5, -5)]
    population_size = 10
    sigma = 1
    iterations = 40
    mutation_rate = 0.7
    crossover_rate = 0.3

    errors = []
    values = [x/1000 for x in range(1001)]
    for crossover_rate in values:
        ea1 = EvolutionaryAlgorithm(
            function,
            population_size,
            bounds,
            mutation_rate,
            crossover_rate,
            sigma
        )
        min_f, history_f = ea1.run(iterations)
        errors.append(error(min_f, ackley))
    plot_error(values, errors, "prawdopodbieńśtwo krzyżowania")


def check_population_size(function):
    bounds = [(-100, -100), (100, 100)]
    sigma = 1
    iterations = 20
    mutation_rate = 0.6
    crossover_rate = 0.4

    errors = []
    values = [i+1 for i in range(100)]
    for population_size in values:
        ea1 = EvolutionaryAlgorithm(
            function,
            population_size,
            bounds,
            mutation_rate,
            crossover_rate,
            sigma
        )
        min_f, history_f = ea1.run(iterations)
        errors.append(error(min_f, ackley))
    plot_error(values, errors, "rozmiar populacji")


def main():
    print(ackley((0.00537837, -0.07308644)))
    # check_param_c(himmelblau)
    # check_param_c(ackley)
    # check_population_size(himmelblau)
    bounds = [(-10, -10), (10, 10)]
    sigma = 1
    iterations = 50
    mutation_rate = 0.6
    crossover_rate = 0.4
    population_size = 10
    ea1 = EvolutionaryAlgorithm(
            ackley,
            population_size,
            bounds,
            mutation_rate,
            crossover_rate,
            sigma
    )
    min_f, history_f = ea1.run(iterations)
    plot3D(ackley, history_f, bounds)
    print(min_f)
    Hbounds = [(-10, -10), (10, 10)]
    population_size = 10
    mutation_rate = 0.01
    crossover_rate = 0.7
    sigma = 1
    iterations = 1000

    ea1 = EvolutionaryAlgorithm(
        himmelblau,
        population_size,
        Hbounds,
        mutation_rate,
        crossover_rate,
        sigma
    )
    min_himmelblau, history_himmelblau = ea1.run(iterations)
    plot3D(himmelblau, history_himmelblau, Hbounds)

if __name__ == "__main__":
    main()
