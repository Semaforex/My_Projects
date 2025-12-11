from tic_tac_toe import TicTacToe
from alpha_beta import find_best_move
from min_max import min_max


def main():
    bot = input("choose bot [basic/alpha_beta]\n")
    if bot == "basic":
        if input("Do you want to start? [y/n]\n") in ["yes", "Yes", "y", "Y"]:
            playerstart = True
        else:
            playerstart = False
        game = TicTacToe()
        if not playerstart:
            (_, (x, y)), checked = min_max(game)
            game.make_move(x, y)
            print("checked endings: ", checked)
        while game.check_win() == 0:
            state = game.state
            for row in state:
                print(row)
            x = int(input("Choose row\n"))
            y = int(input("Choose column\n"))
            while not game.make_move(x, y):
                x = int(input("Illegal move, choose row\n"))
                y = int(input("Choose column\n"))
            if not game.check_win() == 0:
                break
            (_, (x, y)), checked = min_max(game)
            game.make_move(x, y)
            print("checked endings: ", checked)
            if game.check_draw():
                print("Draw")
                break
        if game.check_win() == 1:
            print("Player 1 wins")
        else:
            print("Player -1 wins")
        for row in state:
            print(row)

    elif bot == "alpha_beta":
        if input("Do you want to start? [y/n]\n") in ["yes", "Yes", "y", "Y"]:
            playerstart = True
        else:
            playerstart = False
        game = TicTacToe()
        if not playerstart:
            (x, y), checked_possibilities = find_best_move(game)
            game.make_move(x, y)
            print("checked endings: ", checked_possibilities)
        while game.check_win() == 0:
            state = game.state
            for row in state:
                print(row)
            x = int(input("Choose row\n"))
            y = int(input("Choose column\n"))
            while not game.make_move(x, y):
                x = int(input("Illegal move, choose row\n"))
                y = int(input("Choose column\n"))
            if not game.check_win() == 0:
                break
            (x, y), checked_possibilities = find_best_move(game)
            game.make_move(x, y)
            print("checked endings: ", checked_possibilities)
            if game.check_draw():
                print("Draw")
                break
        if game.check_win() == 1:
            print("Player 1 wins")
        else:
            print("Player -1 wins")
        for row in state:
            print(row)
    else:
        print("Wrong input")


if __name__ == "__main__":
    main()
