import gymnasium as gym
import numpy as np


def Q_Learning(env, parameters, random_seed) -> np.ndarray:
    num_episodes = parameters['episodes']
    alpha = parameters['alpha']
    gamma = parameters['gamma']
    epsilon = parameters['epsilon']
    max_steps = parameters['max_steps']
    np.random.seed(random_seed)
    Q = np.zeros((env.observation_space.n, env.action_space.n))
    rewards = []
    Steps = []
    for i in range(num_episodes):
        rewards.append(0)
        state = env.reset()[0]
        done = False
        steps = 0
        while not done and steps < max_steps:
            if np.random.uniform(0, 1) < epsilon:
                action = env.action_space.sample()
            else:
                max_value = np.max(Q[state, :])
                action = np.random.choice(np.where(Q[state, :] == max_value)[0])
            next_state, reward, done, _, _ = env.step(action)
            if done and reward == 0:
                reward = -1
            next_state = next_state
            Q[state, action] = Q[state, action]*(1-alpha) + alpha*(reward + gamma*np.max(Q[next_state, :]))
            state = next_state
            steps += 1
        if done and reward == 0:
            steps = max_steps
        Steps.append(steps)
        for j in range(env.observation_space.n):
            for k in range(env.action_space.n):
                rewards[i] += Q[j, k]
    return Q, rewards, Steps
