import torch

def inverted_pendulum_dynamics(t, state, controller, params=None):
    """
    Oblicza pochodne stanu [theta, omega] dla odwróconego wahadła.
    Kontroler dostarcza akcję w pętli zamkniętej: u = pi_theta(x(t), t).
    """
    if params is None:
        # Standardowe parametry fizyczne
        params = {'m': 0.5, 'l': 0.5, 'g': 9.81, 'b': 0.1, 'I': 0.05}
        
    m, l, g, b, I = params['m'], params['l'], params['g'], params['b'], params['I']
    
    theta = state[..., 0]
    omega = state[..., 1]
    
    # Pobranie akcji sterującej z sieci neuronowej (Feedback Policy)
    u = controller(t, state)
    
    # Równanie ruchu: (I + ml^2) * d_omega = u + mgl*sin(theta) - b*omega
    dtheta_dt = omega
    denominator = I + m * (l ** 2)
    domega_dt = (u + m * g * l * torch.sin(theta) - b * omega) / denominator

    return torch.stack([dtheta_dt, domega_dt], dim=-1)