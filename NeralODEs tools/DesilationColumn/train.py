from torchdiffeq import odeint
import torch
from data import u_func_template

def train_model(model, trajectories_train, steering_table_train, trajectories_val, steering_table_val, dt, num_epochs_adam, num_steps_lbfgs, num_chunks, chunk_steps, ts):
    VAL_FREQ = 10

    criterion = torch.nn.MSELoss()

    u_func_train = lambda t: u_func_template(t, steering_table_train, len(ts), dt)
    nn_dynamics = lambda t, state: model(t, state, u_func_train)
    
    u_func_val = lambda t: u_func_template(t, steering_table_val, len(ts), dt)
    nn_dynamics_val = lambda t, state: model(t, state, u_func_val)

    # --- FAZA 1: ADAM (Chunking) ---
    print("Rozpoczęcie fazy 1: Adam")
    optimizer_adam = torch.optim.Adam(model.parameters(), lr=1e-3)
    
    for epoch in range(num_epochs_adam):
        model.train()
        epoch_loss = 0.0

        for i in range(num_chunks):
            optimizer_adam.zero_grad()
            
            start_idx = i * chunk_steps
            end_idx = start_idx + chunk_steps
            
            if end_idx > len(ts):
                break
                
            t_chunk = ts[start_idx:end_idx]
            y0_chunk = trajectories_train[start_idx]
            target_chunk = trajectories_train[start_idx:end_idx]

            pred_chunk = odeint(
                nn_dynamics, 
                y0_chunk, 
                t_chunk, 
                method='rk4', 
                options={'step_size': dt}
            )
            
            loss = criterion(pred_chunk, target_chunk)
            loss.backward()
            
            optimizer_adam.step()
            epoch_loss += loss.item()
        
        if epoch % VAL_FREQ == 0 or epoch == num_epochs_adam - 1:
            model.eval()
            with torch.no_grad():
                val_loss = 0.0
                for i in range(num_chunks):
                    start_idx = i * chunk_steps
                    end_idx = start_idx + chunk_steps
                    if end_idx > len(ts):
                        break
                    
                    t_chunk = ts[start_idx:end_idx]
                    y0_chunk = trajectories_val[start_idx]
                    target_chunk = trajectories_val[start_idx:end_idx]

                    pred_chunk = odeint(nn_dynamics_val, y0_chunk, t_chunk, method='rk4', options={'step_size': dt})
                    val_loss += criterion(pred_chunk, target_chunk).item()
                    
            print(f"Adam Epoch {epoch:03d} | Train Loss: {epoch_loss / num_chunks:.6f} | Val Loss: {val_loss / num_chunks:.6f}")

    # --- FAZA 2: L-BFGS (Pełne trajektorie) ---
    print("Rozpoczęcie fazy 2: L-BFGS (Pełne trajektorie)")
    optimizer_lbfgs = torch.optim.LBFGS(
        model.parameters(), 
        max_iter=100, 
        tolerance_grad=1e-5, 
        tolerance_change=1e-9, 
        history_size=50,
        line_search_fn="strong_wolfe"
    )
    VAL_FREQ = 1


    for step in range(num_steps_lbfgs):
        model.train()

        def closure():
            optimizer_lbfgs.zero_grad()
            pred = odeint(
                nn_dynamics, 
                trajectories_train[0], 
                ts, 
                method='rk4', 
                options={'step_size': dt}
            )
            loss = criterion(pred, trajectories_train)
            loss.backward()
            return loss

        loss_val = optimizer_lbfgs.step(closure)
        step_loss = loss_val.item()

        if step % VAL_FREQ == 0 or step == num_steps_lbfgs - 1:
            model.eval()
            with torch.no_grad():
                pred_val = odeint(
                    nn_dynamics_val, 
                    trajectories_val[0], 
                    ts, 
                    method='rk4', 
                    options={'step_size': dt}
                )
                val_loss = criterion(pred_val, trajectories_val).item()
                    
            print(f"L-BFGS Step {step:03d} | Train Loss: {step_loss:.6f} | Val Loss: {val_loss:.6f}")

def eval_model(model, y_0s, y_0s_val, trajectories, trajectories_val, steering_table, steering_table_val, ts, dt):
    import matplotlib.pyplot as plt
    import torch.nn.functional as F
    
    u_func_train = lambda t: u_func_template(t, steering_table, len(ts), dt)
    nn_dynamics = lambda t, state: model(t, state, u_func_train)

    u_func_val = lambda t: u_func_template(t, steering_table_val, len(ts), dt)
    nn_dynamics_val = lambda t, state: model(t, state, u_func_val)

    model.eval()
    with torch.no_grad():
        pred_train = odeint(nn_dynamics, y_0s, ts, method='rk4', options={'step_size': dt})
        pred_val = odeint(nn_dynamics_val, y_0s_val, ts, method='rk4', options={'step_size': dt})
        
        train_mse = F.mse_loss(pred_train, trajectories).item()
        val_mse = F.mse_loss(pred_val, trajectories_val).item()

    print("\n--- Evaluation Metrics ---")
    print(f"Total Train MSE: {train_mse:.6f}")
    print(f"Total Val MSE:   {val_mse:.6f}")
    print("--------------------------\n")

    dims_to_plot = range(32)
    num_to_plot = 1

    # DODANE squeeze=False, by axes zawsze było macierzą 2D
    fig, axes = plt.subplots(num_to_plot, 2, figsize=(15, 5 * num_to_plot), squeeze=False)

    for i in range(num_to_plot):
        # Training Trajectories (Lewa Kolumna)
        for d in dims_to_plot:
            axes[i, 0].plot(ts.cpu(), trajectories[:, i, d].cpu())
            axes[i, 0].plot(ts.cpu(), pred_train[:, i, d].cpu(), '--')
        axes[i, 0].set_title(f'Training Trajectory {i}')

        # Validation Trajectories (Prawa Kolumna)
        for d in dims_to_plot:
            axes[i, 1].plot(ts.cpu(), trajectories_val[:, i, d].cpu())
            axes[i, 1].plot(ts.cpu(), pred_val[:, i, d].cpu(), '--')
        axes[i, 1].set_title(f'Validation Trajectory {i}')

    plt.tight_layout()
    plt.show()