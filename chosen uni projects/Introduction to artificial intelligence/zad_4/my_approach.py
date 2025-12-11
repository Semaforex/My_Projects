from ucimlrepo import fetch_ucirepo 
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.utils import shuffle
import numpy as np
from plot_results import plot_results
import json

Kernels = ['linear', 'poly', 'rbf', 'sigmoid']

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Example for testing influence of C parameter. For other parameters fully analogical.
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!


# parameters:
Kernel = Kernels[0]
c_values = [i*0.1 for i in range(1, 100)]
max_iterations = -1
Tol = 1e-3


wine = fetch_ucirepo(id=109)
x = wine.data.features.values
y = wine.data.targets.values
random_states = [0, 10, 20]
x, y = shuffle(x, y, random_state=0)
dt_accuracy = []
dt_precision = []
dt_recall = []
dt_f1_score = []

accuracy_data = []
precision_data = []
recall_data = []
f1_data = []

for C in c_values:
    print(C)
    svm_accuracy = []
    svm_precision = []
    svm_recall = []
    svm_f1_score = []

    for random_state in random_states:
        X1, X2, X3, X4 = np.array_split(x, 4)
        Y1, Y2, Y3, Y4 = np.array_split(y, 4)
        training_pairs = [(X1, Y1), (X2, Y2), (X3, Y3), (X4, Y4)]
        for i in range(4):
            svm_model = SVC(kernel=Kernel, C=C, max_iter=max_iterations, tol=Tol)
            X_train, Y_train = [], []
            for j in range(4):
                if j != i:
                    if len(X_train) == 0:
                        X_train = training_pairs[j][0]
                        Y_train = training_pairs[j][1]
                    else:
                        X_train = np.concatenate((X_train, training_pairs[j][0]))
                        Y_train = np.concatenate((Y_train, training_pairs[j][1]))
            svm_model.fit(X_train, Y_train.ravel())
            X_test, Y_test = training_pairs[i]
            svm_predictions = svm_model.predict(X_test)
            svm_accuracy.append(accuracy_score(Y_test, svm_predictions))
            svm_f1_score.append(f1_score(Y_test, svm_predictions, average='weighted', labels=np.unique(svm_predictions)))
            svm_precision.append(precision_score(Y_test, svm_predictions, average='weighted', labels=np.unique(svm_predictions)))
            svm_recall.append(recall_score(Y_test, svm_predictions, average='weighted', labels=np.unique(svm_predictions)))

    accuracy_data.append(sum(svm_accuracy) / len(svm_accuracy))
    precision_data.append(sum(svm_precision) / len(svm_precision))
    recall_data.append(sum(svm_recall) / len(svm_recall))
    f1_data.append(sum(svm_f1_score) / len(svm_f1_score))

data = {
    'accuracy': accuracy_data,
    'precision': precision_data,
    'recall': recall_data,
    'f1': f1_data
}

with open('data.json', 'w') as f:
    json.dump(data, f)

plot_results(c_values, accuracy_data, precision_data, recall_data, f1_data)

for random_state in random_states:
    X1, X2, X3, X4 = np.array_split(x, 4)
    Y1, Y2, Y3, Y4 = np.array_split(y, 4)
    training_pairs = [(X1, Y1), (X2, Y2), (X3, Y3), (X4, Y4)]
    for i in range(4):
        dt_model = DecisionTreeClassifier()
        X_train, Y_train = [], []
        for j in range(4):
            if j != i:
                if len(X_train) == 0:
                    X_train = training_pairs[j][0]
                    Y_train = training_pairs[j][1]
                else:
                    X_train = np.concatenate((X_train, training_pairs[j][0]))
                    Y_train = np.concatenate((Y_train, training_pairs[j][1]))
        dt_model.fit(X_train, Y_train.ravel())
        X_test, Y_test = training_pairs[i]
        dt_predictions = dt_model.predict(X_test)
        dt_accuracy.append(accuracy_score(Y_test, dt_predictions))
        dt_f1_score.append(f1_score(Y_test, dt_predictions, average='weighted', labels=np.unique(dt_predictions)))
        dt_precision.append(precision_score(Y_test, dt_predictions, average='weighted', labels=np.unique(dt_predictions)))
        dt_recall.append(recall_score(Y_test, dt_predictions, average='weighted', labels=np.unique(dt_predictions)))


dt_accuracy = sum(dt_accuracy) / len(dt_accuracy)
dt_precision = sum(dt_precision) / len(dt_precision)
dt_recall = sum(dt_recall) / len(dt_recall)
dt_f1_score = sum(dt_f1_score) / len(dt_f1_score)

print("DT Model:")
print("Accuracy:", dt_accuracy)
print("Precision:", dt_precision)
print("Recall:", dt_recall)
print("F1 Score:", dt_f1_score)



