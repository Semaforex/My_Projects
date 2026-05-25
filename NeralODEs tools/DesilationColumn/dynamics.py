import torch

global num_dim
num_dim = 32

def distill_dynamics(t, state, u_func, device="cpu"):
    u_val = u_func(t)
    
    Feed = 24.0 / 60.0
    x_Feed = 0.5
    D = 0.5 * Feed
    L = u_val * D
    V = L + D
    FL = Feed + L
    vol = 1.6
    atray = 0.25
    acond = 0.5
    areb = 1.0

    y = state * vol / (1 + (vol - 1) * state)
    xdot = torch.empty_like(state) # empty_like jest minimalnie szybsze niż zeros_like

    # Wymuszamy kształt (BATCH, 1) do prawidłowego rozgłaszania (broadcasting) wzdłuż 15 wymiarów
    L_val = L[..., 0:1] if L.ndim > 1 else L
    V_val = V[..., 0:1] if V.ndim > 1 else V
    FL_val = FL[..., 0:1] if FL.ndim > 1 else FL

    # Równanie 0
    xdot[..., 0] = (1 / acond) * V[..., 0] * (y[..., 1] - state[..., 0])
    
    # Równania 1 do 15 (zastępuje pierwszą pętlę)
    xdot[..., 1:16] = (1 / atray) * (
        L_val * (state[..., 0:15] - state[..., 1:16]) - 
        V_val * (y[..., 1:16] - y[..., 2:17])
    )
        
    # Równanie 16
    xdot[..., 16] = (1 / atray) * (
        Feed * x_Feed + 
        L[..., 0] * state[..., 15] - 
        FL[..., 0] * state[..., 16] - 
        V[..., 0] * (y[..., 16] - y[..., 17])
    )
    
    # Równania 17 do 30 (zastępuje drugą pętlę)
    xdot[..., 17:31] = (1 / atray) * (
        FL_val * (state[..., 16:30] - state[..., 17:31]) - 
        V_val * (y[..., 17:31] - y[..., 18:32])
    )
        
    # Równanie 31
    xdot[..., 31] = (1 / areb) * (
        FL[..., 0] * state[..., 30] - 
        (Feed - D) * state[..., 31] - 
        V[..., 0] * y[..., 31]
    )
    
    return xdot