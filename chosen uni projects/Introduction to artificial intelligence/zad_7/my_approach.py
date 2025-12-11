from ucimlrepo import fetch_ucirepo 
import numpy as np
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.utils import shuffle


wine = fetch_ucirepo(id=109)
x = wine.data.features.values
y = wine.data.targets.values
random_states = [0, 10, 20]
x, y = shuffle(x, y, random_state=0)

accuracy = []
precision = []
recall = []
f1 = []

accuracy_data = []
precision_data = []
recall_data = []
f1_data = []

for random_state in random_states:
    X1, X2, X3, X4 = np.array_split(x, 4)
    Y1, Y2, Y3, Y4 = np.array_split(y, 4)
    training_pairs = [(X1, Y1), (X2, Y2), (X3, Y3), (X4, Y4)]
    for i in range(4):
        gnb = GaussianNB()
        X_train, Y_train = [], []
        for j in range(4):
            if j != i:
                if len(X_train) == 0:
                    X_train = training_pairs[j][0]
                    Y_train = training_pairs[j][1]
                else:
                    X_train = np.concatenate((X_train, training_pairs[j][0]))
                    Y_train = np.concatenate((Y_train, training_pairs[j][1]))
        gnb.fit(X_train, Y_train.ravel())
        X_test, Y_test = training_pairs[i]
        gnb_predictions = gnb.predict(X_test)
        accuracy_data.append(accuracy_score(Y_test, gnb_predictions))
        f1_data.append(f1_score(Y_test, gnb_predictions, average='weighted', labels=np.unique(gnb_predictions)))
        precision_data.append(precision_score(Y_test, gnb_predictions, average='weighted', labels=np.unique(gnb_predictions)))
        recall_data.append(recall_score(Y_test, gnb_predictions, average='weighted', labels=np.unique(gnb_predictions)))
    accuracy.append(sum(accuracy_data) / len(accuracy_data))
    precision.append(sum(precision_data) / len(precision_data))
    recall.append(sum(recall_data) / len(recall_data))
    f1.append(sum(f1_data) / len(f1_data))


for i in range(len(random_states)):
    print(f'Random State: {random_states[i]}')
    print('accuracy: ', accuracy[i])
    print('precision: ', precision[i])
    print('f1_score: ', f1[i])
    print('recall: ', recall[i])

    print('\n')
