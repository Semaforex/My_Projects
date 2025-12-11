from matplotlib import pyplot as plt


def plot_results(minmax1, alphabeta1, minmax2, alphabeta2, minmax3, alphabeta3, minmax4, alphabeta4):
    plt.plot(minmax1[0], minmax1[1], label='basic_1')
    plt.plot(alphabeta1[0], alphabeta1[1], label='Alpha-Beta_1')
    plt.plot(minmax2[0], minmax2[1], label='basic_2')
    plt.plot(alphabeta2[0], alphabeta2[1], label='Alpha-Beta_2')
    plt.plot(minmax3[0], minmax3[1], label='basic_3')
    plt.plot(alphabeta3[0], alphabeta3[1], label='Alpha-Beta_3')
    plt.plot(minmax4[0], minmax4[1], label='basic_4')
    plt.plot(alphabeta4[0], alphabeta4[1], label='Alpha-Beta_4')
    plt.yscale('log')
    plt.legend()
    plt.show()


minmax1 = ([1, 3, 5, 7, 9], [549946, 7332, 198, 14, 2])
alphabeta1 = ([1, 3, 5, 7, 9], [3957, 318, 21, 4, 1])
minmax2 = ([1, 3, 5, 7], [549946, 8232, 246, 7])
alphabeta2 = ([1, 3, 5, 7], [3957, 241, 27, 1])
minmax3 = ([2, 4, 6, 8], [55505, 933, 51, 5])
alphabeta3 = ([2, 4, 6, 8], [1560, 57, 9, 2])
minmax4 = ([2, 4, 6], [59705, 1053, 24])
alphabeta4 = ([2, 4, 6], [573, 30, 1])
plot_results(minmax2, alphabeta2, minmax2, alphabeta2, minmax3, alphabeta3, minmax4, alphabeta4)
