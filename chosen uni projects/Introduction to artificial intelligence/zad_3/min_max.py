from tic_tac_toe import TicTacToe
from copy import deepcopy


def min_max(game: TicTacToe):
    checked_possibilities = 1
    if game.check_win() != 0:
        return (game.check_win(), (-1, -1)), checked_possibilities
    scores = []
    possible_moves = [(x, y) for x in range(3) for y in range(3) if game.state[x][y] == 0]
    if not possible_moves:
        return (0, (-1, -1)), checked_possibilities
    for x, y in possible_moves:
        new_game = deepcopy(game)
        new_game.make_move(x, y)
        (score, _), checked = min_max(new_game)
        checked_possibilities += checked
        scores.append((score, (x, y)))
    bestmax = (-2, (0, 0))
    bestmin = (2, (0, 0))
    for score, args in scores:
        if score > bestmax[0]:
            bestmax = (score, args)
        if score < bestmin[0]:
            bestmin = (score, args)
    if game.current_player == 1:
        return bestmax, checked_possibilities
    return bestmin, checked_possibilities
