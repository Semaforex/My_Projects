import numpy as np
from random import choices


class EvolutionaryAlgorithm:
    def __init__(
            self,
            fitness_function,
            pop_size: int,
            bounds: list[tuple[int]],
            mutation_rate: float,
            cross_rate: float,
            sigma: float
            ) -> None:
        self.fitness_function = fitness_function
        self.pop_size = pop_size
        self.bounds = bounds
        self.mutation_rate = mutation_rate
        self.cross_rate = cross_rate
        self.population = np.random.uniform(
            self.bounds[0],
            self.bounds[1],
            (self.pop_size, 2)
            )
        self.scores = np.zeros(pop_size)
        self.best = self.population[np.argmin(self.scores)]
        self.sigma = sigma
        self.history = list(self.population)

    def calc_score(self) -> None: 
        self.scores = np.array([self.fitness_function(i) for i in self.population])

    def selection(self):
        selected = []
        for _ in range(self.pop_size):
            participants = np.random.choice(self.pop_size, 2)
            Dict = {i: participants[i] for i in range(2)}
            selected.append(Dict[np.argmin(self.scores[participants])])
        return self.population[selected]

    def crossover(self, parent1, parent2):
        if np.random.random() >= self.cross_rate:
            return parent1, parent2
        weight1 = np.random.random()
        weight2 = np.random.random()
        child1, child2 = [0, 0], [0, 0]
        child1[0] = weight1*parent1[0]+(1-weight1)*parent2[0]
        child1[1] = weight2*parent1[1]+(1-weight2)*parent2[1]
        child2[0] = parent1[0]+parent2[0]-child1[0]
        child2[1] = parent1[1]+parent2[1]-child1[1]
        return child1, child2

    def mutate(self, individual):
        for i in range(2):
            if np.random.random() < self.mutation_rate:
                individual[i] += self.sigma*np.random.normal()
        return np.array(individual)

    def create_new_population(self, selected):
        new_population = []
        for _ in range(self.pop_size):
            p1, p2 = choices(selected, k=2)
            c1, c2 = self.crossover(p1, p2)
            new_population.append(self.mutate(c1))
            new_population.append(self.mutate(c2))
        self.population = np.array(new_population[:self.pop_size])

    def run(self, iterations):

        for _ in range(iterations):
            self.calc_score()
            self.create_new_population(self.selection())
            newbest = self.population[np.argmin(self.scores)]
            self.history += list(self.population)
            if self.fitness_function(newbest) < self.fitness_function(self.best):
                self.best = newbest
        return self.best, self.history
