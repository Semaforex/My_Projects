from tic_tac_toe import TicTacToe
from copy import deepcopy


def compare(game: TicTacToe):
    if game.current_player == 1:
        return max
    return min


def find_best_move(game: TicTacToe):
    all_checked = 0
    possible_moves = [(x, y) for x in range(3) for y in range(3) if game.state[x][y] == 0]
    if not possible_moves:
        return 0
    best = -10*game.current_player
    best_move = (0, 0)
    for x, y in possible_moves:
        new_game = deepcopy(game)
        new_game.make_move(x, y)
        score, checked_possibilities = alpha_beta(new_game)
        all_checked += checked_possibilities
        if compare(game)(score, best) == score and score != best:
            best = score
            best_move = (x, y)
    return best_move, checked_possibilities


def alpha_beta(game: TicTacToe, best = -10, worst = 10):
    all_checked = 1
    if game.check_win() != 0:
        return game.check_win(), all_checked
    possible_moves = [(x, y) for x in range(3) for y in range(3) if game.state[x][y] == 0]
    if not possible_moves:
        return 0, all_checked
    if game.current_player == 1:
        for x, y in possible_moves:
            new_game = deepcopy(game)
            new_game.make_move(x, y)
            temp, checked_possibilities = alpha_beta(new_game, best, worst)
            best = max(temp, best)
            all_checked += checked_possibilities
            if best >= worst:
                return best, all_checked
        return best, all_checked
    for x, y in possible_moves:
        new_game = deepcopy(game)
        new_game.make_move(x, y)
        temp, checked_possibilities = alpha_beta(new_game, best, worst)
        worst = min(temp, worst)
        all_checked += checked_possibilities
        if worst <= best:
            return worst, all_checked
    return worst, all_checked
