from matplotlib import pyplot as plt

def plot_results(x_data, acuracy_data, precision_data, recall_data, f1_data):
    fig, axs = plt.subplots(2, 2)
    fig.suptitle('Model Evaluation Metrics')
    axs[0, 0].plot(x_data, acuracy_data)
    axs[0, 0].set_title('Accuracy')
    axs[0, 1].plot(x_data, precision_data, 'tab:orange')
    axs[0, 1].set_title('Precision')
    axs[1, 0].plot(x_data, recall_data, 'tab:green')
    axs[1, 0].set_title('Recall')
    axs[1, 1].plot(x_data, f1_data, 'tab:red')
    axs[1, 1].set_title('F1 Score')
    for ax in axs.flat:
        ax.set(xlabel='Random State', ylabel='Score')
    for ax in axs.flat:
        ax.label_outer()
    plt.show()