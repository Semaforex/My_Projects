import matplotlib.pyplot as plt

def plot_data(trajectories, steering_table, ts, label=None):
    print("Train shape:", trajectories.shape)

    num_dim = trajectories.shape[2]

    fig, axs = plt.subplots(2, figsize=(10, 8))


    u_vals = steering_table[:, 0, 0]

    for i in range(num_dim):
        axs[0].plot(ts, trajectories[:, 0, i].tolist())

    axs[1].plot(ts, u_vals)
    if not label:
        axs[0].set_ylabel("xs")
    else:
        axs[0].set_ylabel(f"{label} xs")
    axs[1].set_ylabel(f"{label} u")

    plt.tight_layout()
    plt.show()