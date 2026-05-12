import torch
import torch.nn as nn

class NeuralController(nn.Module):
    def __init__(self, state_dim=2, hidden_dim=32, u_max=5.0):
        super(NeuralController, self).__init__()
        # Wejście: Stan (2) + Czas (1)
        self.net = nn.Sequential(
            nn.Linear(state_dim + 1, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, 1),
            nn.Tanh() # Ograniczenie sterowania zgodnie z artykułem
        )
        self.u_max = u_max

    def forward(self, t, state):
        # Dopasowanie czasu t do wymiaru batcha stanów
        t_vec = torch.full((state.shape[0], 1), t.item() if isinstance(t, torch.Tensor) else t, 
                           device=state.device, dtype=state.dtype)
        
        combined_input = torch.cat([state, t_vec], dim=-1)
        u = self.net(combined_input) * self.u_max
        return u[..., 0]

class ClosedLoopSystem(nn.Module):
    def __init__(self, controller, dynamics_func):
        super(ClosedLoopSystem, self).__init__()
        self.controller = controller
        self.dynamics_func = dynamics_func

    def forward(self, t, state):
        return self.dynamics_func(t, state, self.controller)