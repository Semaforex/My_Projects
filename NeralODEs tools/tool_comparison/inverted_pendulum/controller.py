import torch

def get_static_control(state):
    return state.new_zeros(())