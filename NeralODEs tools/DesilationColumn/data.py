import torch
from torchdiffeq import odeint
from scipy.optimize import fsolve
from dynamics import distill_dynamics, num_dim
import numpy as np

def get_initial_states(steering_table, x_guess=np.array([0.5]*num_dim)):
    u_initials = steering_table[0, :, 0].detach().cpu().numpy()
    num_trajectories = len(u_initials)
    num_dim = len(x_guess)
    
    y_0s_np = np.zeros((num_trajectories, num_dim))
    
    for i in range(num_trajectories):
        u_val = u_initials[i]
        
        def fun(x):
            state_tensor = torch.tensor(x, dtype=torch.float32)
            u_func = lambda t: torch.tensor([u_val], dtype=torch.float32)
            xdot = distill_dynamics(0.0, state_tensor, u_func)
            return xdot.detach().numpy()
            
        x_ss, _, _, _ = fsolve(fun, x_guess, full_output=True, xtol=1e-8)
        y_0s_np[i, :] = x_ss
        
    y_0s = torch.tensor(y_0s_np, dtype=torch.float32)
    
    return y_0s

def generate_initial_conditions(num_trajectories, num_dim, num_steering_jump, steps_per_jump):

    steering_table = (torch.rand((num_steering_jump, num_trajectories, 1))*9+1).repeat_interleave(steps_per_jump, dim=0)
    y_0s = get_initial_states(steering_table)

    return y_0s, steering_table

def u_func_template(t, steering_table, num_steps, dt):
    if torch.isnan(t).any() or torch.isinf(t).any():
        idx = torch.tensor(0, device=t.device, dtype=torch.long)
    else:
        idx = (t / dt).long()
        idx = torch.clamp(idx, min=0, max=num_steps - 1)
        
    return steering_table[idx]

def generate_trajectories(y_0s, steering_table, dt, T):
    print("Generating trajectories...")
    device = "cuda" if torch.cuda.is_available() else "cpu"
    
    y_0s = y_0s.to(device)
    steering_table = steering_table.to(device)

    num_steps = int(T / dt)
    times = torch.arange(num_steps, dtype=torch.float32, device=device) * dt
    
    u_func = lambda t: u_func_template(t, steering_table, num_steps, dt)

    trajectories = odeint(lambda t, state: distill_dynamics(t, state, u_func, device), y_0s, times)

    return trajectories.cpu()