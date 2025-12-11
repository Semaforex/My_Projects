from Qlearning import Q_Learning
import gymnasium as gym
import numpy as np
from matplotlib import pyplot as plt

def test_Q_Learning(parameters, size):
    env = gym.make('FrozenLake-v1', is_slippery=False, map_name=size)
    max_steps = parameters['max_steps']

    Q = [None]*3
    rewards = [None]*3
    steps = [None]*3

    Q[0], rewards[0], steps[0] = Q_Learning(env, parameters, random_seed=0)
    Q[1], rewards[1], steps[1] = Q_Learning(env, parameters, random_seed=1)
    Q[2], rewards[2], steps[2] = Q_Learning(env, parameters, random_seed=2)

    # figure, ax = plt.subplots(2)

    # for i in range(3):
    #     ax[0].plot(rewards[i], label=f'Seed {i}')

    # ax[0].set_xlabel('Episodes')
    # ax[0].set_ylabel('Total Reward')
    # ax[0].legend()

    steps = [(steps[0][i]+steps[1][i]+steps[2][i])/3 for i in range(len(steps[0]))]

    stop_point = len(steps)-1

    for i in range(15, len(steps)-15):
        if abs(sum(steps[i-15: i]) - sum(steps[i: i+15])) <= 1 and i > 20:
            stop_point = i-15
            break

    # ax[1].plot(steps, label='Average Steps')
    # ax[1].plot(stop_point, steps[stop_point], marker="o", markeredgecolor="red", markerfacecolor="red")

    # ax[1].set_ylabel('number of steps')
    # ax[1].set_xlabel('Episodes')
    # ax[1].legend()

    # plt.title('Q-Learning on FrozenLake-v1')
    # plt.show()


    steps2 = [0]*3

    for i in range(3):
        env.reset()
        state = env.reset()[0]
        done = False
        while not done and steps2[i] < max_steps:
            action = np.argmax(Q[i][state, :])
            next_state, reward, done, _, _ = env.step(action)
            state = next_state
            steps2[i] += 1

    print(steps2)

    print(parameters['alpha'])

    avg = sum(steps[stop_point:])/len(steps[stop_point:])

    return stop_point, rewards, steps, avg


def test_Q_Learning_noplot(parameters, size):
    env = gym.make('FrozenLake-v1', is_slippery=False, map_name=size)

    Q = [None]*3
    rewards = [None]*3
    steps = [None]*3

    Q[0], rewards[0], steps[0] = Q_Learning(env, parameters, random_seed=0)
    Q[1], rewards[1], steps[1] = Q_Learning(env, parameters, random_seed=1)
    Q[2], rewards[2], steps[2] = Q_Learning(env, parameters, random_seed=2)

    steps = [(steps[0][i]+steps[1][i]+steps[2][i])/3 for i in range(len(steps[0]))]

    stop_point = len(steps)-1

    for i in range(15, len(steps)-15):
        if abs(sum(steps[i-15: i]) - sum(steps[i: i+15])) <= 1:
            stop_point = i-15
            break
    
    avg = sum(steps[stop_point:])/len(steps[stop_point:])

    print(parameters['alpha'])

    return stop_point, (rewards[0][stop_point]+rewards[1][stop_point]+rewards[2][stop_point])/3, avg


