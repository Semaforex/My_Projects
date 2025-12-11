from tester import test_Q_Learning, test_Q_Learning_noplot
from Qlearning import Q_Learning
import numpy as np
import gymnasium as gym
from matplotlib import pyplot as plt

parameters = {
    'alpha': 0.1,
    'gamma': 0.8,
    'epsilon': 0.2,
    'max_steps': 1000,
    'episodes': 10000
}


# def plotter(ax, rewards, epsilon):
#     ax.plot(rewards[0], label='Seed 0')
#     ax.plot(rewards[1], label='Seed 1')
#     ax.plot(rewards[2], label='Seed 2')
#     ax.set_xlabel('Episodes')
#     ax.set_ylabel('Total Reward' + f' (epsilon = {epsilon})')
#     ax.legend()

# figure, ax = plt.subplots(3, 3)
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[0, 0], rewards, 0.1)
# parameters['epsilon'] = 0.2
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[0, 1], rewards, 0.2)
# parameters['epsilon'] = 0.3
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[0, 2], rewards, 0.3)
# parameters['epsilon'] = 0.4
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[1, 0], rewards, 0.4)
# parameters['epsilon'] = 0.5
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[1, 1], rewards, 0.5)
# parameters['epsilon'] = 0.6
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[1, 2], rewards, 0.6)
# parameters['epsilon'] = 0.7
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[2, 0], rewards, 0.7)
# parameters['epsilon'] = 0.8
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[2, 1], rewards, 0.8)
# parameters['epsilon'] = 0.9
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter(ax[2, 2], rewards, 0.9)
# plt.show()

# def plotter2(ax, steps, stop_point, avg, epsilon):
#     ax.plot(steps, label='Average steps in episode')
#     ax.plot(stop_point, avg, marker="o", markeredgecolor="red", markerfacecolor="red")
#     ax.axhline(y=avg, color='g', linestyle='--', label='Average after reaching stability')
#     ax.set_xlabel('Episodes')
#     ax.set_ylabel('steps in episode' + f' (epsilon = {epsilon})')
#     ax.legend()

# figure, ax = plt.subplots(3, 3)
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[0, 0], steps, stop_point, avg, 0.1)
# parameters['epsilon'] = 0.2
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[0, 1], steps, stop_point, avg, 0.2)
# parameters['epsilon'] = 0.3
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[0, 2], steps, stop_point, avg, 0.3)
# parameters['epsilon'] = 0.4
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[1, 0], steps, stop_point, avg, 0.4)
# parameters['epsilon'] = 0.5
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[1, 1], steps, stop_point, avg, 0.5)
# parameters['epsilon'] = 0.6
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[1, 2], steps, stop_point, avg, 0.6)
# parameters['epsilon'] = 0.7
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[2, 0], steps, stop_point, avg, 0.7)
# parameters['epsilon'] = 0.8
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[2, 1], steps, stop_point, avg, 0.8)
# parameters['epsilon'] = 0.9
# stop_point, rewards, steps, avg = test_Q_Learning(parameters, '8x8')
# plotter2(ax[2, 2], steps, stop_point, avg, 0.9)
# plt.show()


env = gym.make('FrozenLake-v1', is_slippery=False, map_name='4x4')
env.reset()
Q, _, _ = Q_Learning(env, parameters, random_seed=0)
print(Q)
env = gym.make('FrozenLake-v1', is_slippery=False, map_name='4x4', render_mode='human')
env.reset()
state = 0
for i in range(100):
    env.render()
    action = np.argmax(Q[state])
    state, reward, done, _, _ = env.step(action)
    if done:
        break
