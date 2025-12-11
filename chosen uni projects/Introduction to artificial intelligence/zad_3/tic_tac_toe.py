class TicTacToe:
    def __init__(self):
        self.state = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        self.current_player = 1

    def make_move(self, x, y):
        if x < 0 or x > 2 or y < 0 or y > 2:
            return False
        if self.state[x][y] == 0:
            self.state[x][y] = self.current_player
            self.current_player = -self.current_player
            return True
        return False

    def check_win(self):
        rows = [self.state[i] for i in range(3)]
        collums = [[self.state[j][i] for j in range(3)] for i in range(3)]
        diagonals = [[self.state[i][i] for i in range(3)], [self.state[i][2 - i] for i in range(3)]]
        lines = rows + collums + diagonals
        if [1, 1, 1] in lines:
            return 1
        if [-1, -1, -1] in lines:
            return -1
        return 0

    def check_draw(self):
        return all([all([x != 0 for x in row]) for row in self.state])
