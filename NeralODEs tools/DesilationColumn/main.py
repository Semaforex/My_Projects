from model import DynamicsModel
from dynamics import num_dim
from data import generate_initial_conditions, generate_trajectories
from plot import plot_data
from train import train_model, eval_model
import datetime
import torch
import numpy as np
import os

def main():
    torch.manual_seed(42)
    
    NUM_TRAJECTORIES = 64
    NUM_TRAJECTORIES_VAL = 4
    T = 1000
    CHUNK_LEN = 40
    dt = 1.0
    NUM_STEERING_JUMP = 10

    TOTAL_STEPS = int(T / dt)
    steps_per_jump = TOTAL_STEPS // NUM_STEERING_JUMP
    
    DATA_FILE = "dataset.pt"

    if os.path.exists(DATA_FILE):
        print(f"Loading generated data from {DATA_FILE}...")
        dataset = torch.load(DATA_FILE)
        
        y_0s_train = dataset['y_0s_train']
        steering_table_train = dataset['steering_table_train']
        trajectories_train = dataset['trajectories_train']
        
        y_0s_val = dataset['y_0s_val']
        steering_table_val = dataset['steering_table_val']
        trajectories_val = dataset['trajectories_val']
    else:
        print("Generating new data...")
        y_0s_train, steering_table_train = generate_initial_conditions(NUM_TRAJECTORIES, num_dim, NUM_STEERING_JUMP, steps_per_jump)
        y_0s_val, steering_table_val = generate_initial_conditions(NUM_TRAJECTORIES_VAL, num_dim, NUM_STEERING_JUMP, steps_per_jump)

        trajectories_val = generate_trajectories(y_0s_val, steering_table_val, dt, T)
        trajectories_train = generate_trajectories(y_0s_train, steering_table_train, dt, T)
        
        print(f"Saving generated data to {DATA_FILE}...")
        torch.save({
            'y_0s_train': y_0s_train,
            'steering_table_train': steering_table_train,
            'trajectories_train': trajectories_train,
            'y_0s_val': y_0s_val,
            'steering_table_val': steering_table_val,
            'trajectories_val': trajectories_val
        }, DATA_FILE)

    # plot_data(trajectories_train, steering_table_train, torch.arange(TOTAL_STEPS)*dt, label="Train")
    # plot_data(trajectories_val, steering_table_val, torch.arange(TOTAL_STEPS)*dt, label="Val")

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    model = DynamicsModel(state_dim=num_dim, control_dim=1, hidden_dim=128).to(device)

    trajectories_train = trajectories_train.to(device)
    trajectories_val = trajectories_val.to(device)
    steering_table_train = steering_table_train.to(device)
    steering_table_val = steering_table_val.to(device)
    ts = (torch.arange(TOTAL_STEPS, dtype=torch.float32) * dt).to(device)
    
    y_0s_train = y_0s_train.to(device)
    y_0s_val = y_0s_val.to(device)

    train_model(
        model, 
        trajectories_train, 
        steering_table_train, 
        trajectories_val, 
        steering_table_val, 
        dt, 
        num_epochs_adam=150, 
        num_steps_lbfgs=30,
        num_chunks=TOTAL_STEPS // CHUNK_LEN,
        chunk_steps=CHUNK_LEN, 
        ts=ts
    )

    eval_model(
        model, 
        y_0s_train, 
        y_0s_val, 
        trajectories_train, 
        trajectories_val, 
        steering_table_train, 
        steering_table_val, 
        ts, 
        dt
    )

    os.makedirs("models", exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    save_path = os.path.join("models", f"model_{timestamp}.pt")
    
    torch.save(model.state_dict(), save_path)
    print(f"Model saved successfully to {save_path}")

if __name__ == "__main__":
    main()