from Qlearning import Q_Learning
import gymnasium as gym
import numpy as np
from matplotlib import pyplot as plt

env = gym.make('FrozenLake-v1', is_slippery=False, map_name='4x4')

num_episodes = 10000
alpha = 0.1
gamma = 0.99
epsilon = 0.1
max_steps = 1000

Q, rewards = Q_Learning(env, num_episodes, alpha, gamma, epsilon, max_steps)

print(rewards)

plt.plot(rewards)
plt.xlabel('Episodes')
plt.ylabel('Total Reward')
plt.title('Q-Learning on FrozenLake-v1')
plt.show()
