import torch
from torchdiffeq import odeint
import matplotlib.pyplot as plt
from dynamics import inverted_pendulum_dynamics
from controller import NeuralController, ClosedLoopSystem

# PARAMETRY KONFIGURACYJNE
NUM_TRAIN = 50
NUM_TEST = 5
T_HORIZON = 10.0
DT = 0.1
EPOCHS = 150
LR = 0.01

device = "cuda" if torch.cuda.is_available() else "cpu"

def calculate_loss(system, y0, ts):
    """
    Funkcja pomocnicza do obliczania całkowitego kosztu (loss) 
    dla podanych warunków początkowych.
    """
    trajs = odeint(system, y0, ts, method='dopri5')
    
    # Kara za odchylenie od pionu (theta=0, omega=0) w trakcie całej trajektorii
    state_loss = torch.mean(trajs**2) 
    
    # Zwiększona kara terminalna (stan na końcu czasu T)
    terminal_loss = torch.mean(trajs[-1]**2) * 5.0
    
    return state_loss + terminal_loss, trajs

def run_simulation():
    # 1. Generowanie warunków początkowych
    y0_train = (torch.rand((NUM_TRAIN, 2), device=device) * 2 - 1)
    y0_train[:, 0] *= (torch.pi / 3)
    
    y0_test = (torch.rand((NUM_TEST, 2), device=device) * 2 - 1)
    y0_test[:, 0] *= (torch.pi / 3)

    ts = torch.linspace(0, T_HORIZON, int(T_HORIZON / DT)).to(device)

    # Inicjalizacja modelu
    controller = NeuralController(u_max=10.0).to(device)
    system = ClosedLoopSystem(controller, inverted_pendulum_dynamics).to(device)
    optimizer = torch.optim.Adam(controller.parameters(), lr=LR)

    print(f"Rozpoczynanie treningu na {NUM_TRAIN} trajektoriach...")
    
    train_losses = []
    test_losses = []

    for epoch in range(EPOCHS):
        # --- FAZA TRENINGU ---
        system.train()
        optimizer.zero_grad()
        
        loss_train, _ = calculate_loss(system, y0_train, ts)
        
        loss_train.backward()
        optimizer.step()

        # --- FAZA EWALUACJI ---
        if epoch % 5 == 0 or epoch == EPOCHS - 1:
            system.eval()
            with torch.no_grad():
                loss_test, _ = calculate_loss(system, y0_test, ts)
            
            print(f"Epoka {epoch:03d} | Train Loss: {loss_train.item():.6f} | Test Loss: {loss_test.item():.6f}")
            
            train_losses.append(loss_train.item())
            test_losses.append(loss_test.item())

    # TESTOWANIE I WYKRESY KOŃCOWE
    system.eval()
    with torch.no_grad():
        _, test_trajs = calculate_loss(system, y0_test, ts)
        test_trajs = test_trajs.cpu()
        test_times = ts.cpu().numpy()

    fig, ax = plt.subplots(4, 1, figsize=(10, 15), sharex=False)
    
    # Wykres krzywych uczenia
    epochs_recorded = range(0, EPOCHS, 5)
    # Uzupełnienie jeśli ostatnia epoka nie wypadła na krok podzielny przez 5
    if len(epochs_recorded) < len(train_losses):
        epochs_recorded = list(epochs_recorded) + [EPOCHS - 1]

    ax[0].plot(epochs_recorded, train_losses, label='Train Loss', marker='o')
    ax[0].plot(epochs_recorded, test_losses, label='Test Loss', marker='x')
    ax[0].set_title("Krzywe uczenia")
    ax[0].set_ylabel("Loss")
    ax[0].set_xlabel("Epoka")
    ax[0].legend()
    ax[0].set_yscale('log') # Skala logarytmiczna często lepiej pokazuje zbieżność
    ax[0].grid(True)

    # Wykresy trajektorii
    for i in range(NUM_TEST):
        ax[1].plot(test_times, test_trajs[:, i, 0], label=f'Traj {i+1}' if i==0 else "")
        ax[2].plot(test_times, test_trajs[:, i, 1])
        
        u_vals = [controller(t, test_trajs[idx, i:i+1].to(device)).item() 
                  for idx, t in enumerate(ts)]
        ax[3].plot(test_times, u_vals)

    ax[1].set_ylabel("Kąt Theta [rad]")
    ax[1].set_title(f"Stabilizacja (Test na {NUM_TEST} trajektoriach)")
    ax[2].set_ylabel("Prędkość Omega [rad/s]")
    ax[3].set_ylabel("Sterowanie U")
    ax[3].set_xlabel("Czas [s]")
    
    for a in ax[1:]: a.grid(True); a.axhline(0, color='black', lw=1)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    run_simulation()