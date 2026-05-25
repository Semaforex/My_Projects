import torch.nn as nn
import torch

class DynamicsModel(nn.Module):
    def __init__(self, state_dim=32, control_dim=1, hidden_dim=128):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(state_dim + control_dim, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, state_dim)
        )

    def forward(self, t, state, u_func):
        u_val = u_func(t)
        if u_val.dim() < state.dim():
            u_val = u_val.expand(*state.shape[:-1], -1)
        
        x = torch.cat([state, u_val], dim=-1)
        return self.net(x)