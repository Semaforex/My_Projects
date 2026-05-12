from dynamics import inverted_pendulum_dynamics
from controller import NeuralController, ClosedLoopSystem
import torch
from torchdiffeq import odeint
from matplotlib import pyplot as plt

# Zero steering simulation

if __name__ == "__main__":
    # Inicjalizacja modelu
    controller = NeuralController(u_max=0.0)  # Zero steering
    system = ClosedLoopSystem(controller, inverted_pendulum_dynamics)

    # Przykładowe warunki początkowe (theta, omega)
    y0 = torch.tensor([[torch.pi / 3, 0.0]])  # Duże odchylenie od pionu, brak prędkości kątowej

    # Czas symulacji
    T_HORIZON = 10.0
    DT = 0.1
    ts = torch.linspace(0, T_HORIZON, int(T_HORIZON / DT))

    # Symulacja trajektorii
    with torch.no_grad():
        trajs = odeint(system, y0, ts, method='dopri5')

    # Wizualizacja trajektorii
    plt.figure(figsize=(12, 6))
    for i in range(trajs.shape[1]):
        plt.plot(ts.cpu(), trajs[:, i, 0].cpu(), label=f'Trajektoria {i+1} (theta)')
        plt.plot(ts.cpu(), trajs[:, i, 1].cpu(), label=f'Trajektoria {i+1} (omega)', linestyle='--')
    plt.title('Symulacja odwróconego wahadła (Zero Steering)')
    plt.xlabel('Czas [s]')
    plt.ylabel('Stan (theta, omega)')
    plt.legend()
    plt.grid()
    plt.show()