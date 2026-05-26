import torch

global num_dim
num_dim = 2

def lv_dynamics(t, state, u_func, device="cpu"):
    u_val = u_func(t)
    
    alv = 0.5
    blv = 0.025
    clv = 0.5
    dlv = 0.005

    xdot = torch.empty_like(state) 

    xdot[..., 0] = alv * state[..., 0] - blv * state[..., 0] * state[..., 1]
    xdot[..., 1] = -clv * state[..., 1] + dlv * state[..., 0] * state[..., 1] + u_val[..., 0]
    
    return xdot
