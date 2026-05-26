import matplotlib.pyplot as plt

def plot_data(trajectories, steering_table, ts, label=None):
    prefix = label if label else "Data"
    print(f"{prefix} shape:", trajectories.shape)

    num_dim = trajectories.shape[2]

    fig, axs = plt.subplots(2, 1, figsize=(10, 8))

    # Zabezpieczenie przed próbą rysowania tensorów z GPU
    ts_plot = ts.cpu() if torch.is_tensor(ts) else ts
    u_vals = steering_table[:, 0, 0].cpu() if torch.is_tensor(steering_table) else steering_table[:, 0, 0]
    traj_plot = trajectories[:, 0, :].cpu() if torch.is_tensor(trajectories) else trajectories[:, 0, :]

    for i in range(num_dim):
        axs[0].plot(ts_plot, traj_plot[:, i])

    axs[1].plot(ts_plot, u_vals)
    
    if not label:
        axs[0].set_ylabel("xs")
        axs[1].set_ylabel("u")
    else:
        axs[0].set_ylabel(f"{label} xs")
        axs[1].set_ylabel(f"{label} u")

    plt.tight_layout()
    plt.show()