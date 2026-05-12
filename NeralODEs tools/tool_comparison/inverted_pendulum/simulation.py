from pendulum_dynamics import inverted_pendulum_dynamics
from controller import get_static_control
import torch
from matplotlib import pyplot as plt
import tqdm


def simulate_inverted_pendulum(initial_state, dt, total_time, params):
    n_steps = int(total_time / dt)
    state = initial_state.clone()
    state_history = torch.zeros((n_steps + 1, 4))
    state_history[0] = state
    
    time_array = torch.linspace(0, total_time, n_steps + 1)
    
    for i in tqdm.tqdm(range(n_steps)):
        u = get_static_control(state)
        dstate = inverted_pendulum_dynamics(state, u, params)
        state = state + dstate * dt
        state_history[i + 1] = state
    
    return time_array, state_history

def plot_states(time_array, state_history):
    time_np = time_array.numpy()
    state_np = state_history.numpy()
    
    fig, axs = plt.subplots(2, 2, figsize=(12, 8))
    
    axs[0, 0].plot(time_np, state_np[:, 0])
    axs[0, 0].set_title('Cart Position (x)')
    axs[0, 0].set_xlabel('Time (s)')
    axs[0, 0].set_ylabel('Position (m)')
    
    axs[0, 1].plot(time_np, state_np[:, 1])
    axs[0, 1].set_title('Cart Velocity (x_dot)')
    axs[0, 1].set_xlabel('Time (s)')
    axs[0, 1].set_ylabel('Velocity (m/s)')
    
    axs[1, 0].plot(time_np, state_np[:, 2])
    axs[1, 0].set_title('Pendulum Angle (theta)')
    axs[1, 0].set_xlabel('Time (s)')
    axs[1, 0].set_ylabel('Angle (rad)')
    
    axs[1, 1].plot(time_np, state_np[:, 3])
    axs[1, 1].set_title('Pendulum Angular Velocity (theta_dot)')
    axs[1, 1].set_xlabel('Time (s)')
    axs[1, 1].set_ylabel('Angular Velocity (rad/s)')
    
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    params = {
        'M': 1.0, # Mass of the cart
        'm': 0.1, # Mass of the pendulum
        'l': 0.5, # Length of the pendulum
        'g': 9.81, # Gravitational acceleration
        'b': 0.5, # Damping coefficient
        'I': 0.01 # Moment of inertia of the pendulum
    }
    
    initial_state = torch.tensor([0.0, 0.0, 0.1, 0.0])
    
    dt = 0.0001
    total_time = 10.0
    u_constant = 0.0
    
    time_array, state_history = simulate_inverted_pendulum(initial_state, dt, total_time, params)
    
    plot_states(time_array, state_history)