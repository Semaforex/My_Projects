import torch

def inverted_pendulum_dynamics(state, u, params):
    M = params['M']
    m = params['m']
    l = params['l']
    g = params['g']
    b = params['b']
    I = params['I']
    
    x_dot = state[1]
    theta = state[2] + torch.pi
    theta_dot = state[3]
    
    A_11 = torch.as_tensor(M + m, dtype=state.dtype, device=state.device)
    A_22 = torch.as_tensor(I + m * l**2, dtype=state.dtype, device=state.device)
    cos_term = m * l * torch.cos(theta)
    
    A = torch.stack(
        [
            torch.stack([A_11, cos_term]),
            torch.stack([cos_term, A_22]),
        ]
    )
    
    B = torch.stack(
        [
            u - b * x_dot + m * l * (theta_dot**2) * torch.sin(theta),
            -m * g * l * torch.sin(theta),
        ]
    )
    
    accels = torch.linalg.inv(A) @ B
    
    return torch.stack([x_dot, accels[0], theta_dot, accels[1]])