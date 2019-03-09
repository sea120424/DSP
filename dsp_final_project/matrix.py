from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
from keras.models import load_model
import numpy as np
import pandas as pd
def scale(M):
    A = np.sum(M,axis=0)
    M = M/A
    M = np.round(M*1000) / 1000
    return M

def normalize(X):
    X = (X - X.mean()) / X.std()
    return X

def span(num, X):
    Y = np.zeros((X.shape[0], num))
    index = 0
    for i in X:
        Y[index][i] = 1
        index += 1

    return Y

new_model = load_model('model/128_CNN_5.h5')
val_data = pd.read_csv('data/data_128_val.csv')
train_val_y = val_data['label']
train_val_y = np.array(train_val_y, int)
train_val_y = span(20,train_val_y)

print(train_val_y)

train_val_x = np.array([row.split(' ') for row in val_data['feature'].tolist()], dtype = float)
train_val_x = normalize(train_val_x)
train_val_x = train_val_x.reshape(-1, 32, 32, 1)

ans = new_model.predict(train_val_x)

y_true = []
y_guess = []
for i in train_val_y:
    y_true.append(np.argmax(i))
for i in ans:    
    y_guess.append(np.argmax(i))

confmat = confusion_matrix(y_true=y_true, y_pred=y_guess) 
confmat = scale(confmat)

fig, ax = plt.subplots(figsize=(15,15))
ax.matshow(confmat, cmap=plt.cm.Blues, alpha=0.3)
for i in range(confmat.shape[0]):
    for j in range(confmat.shape[1]):
        ax.text(x=j, y=i, s=confmat[i,j], va='center', ha='center')
plt.xlabel('predicted label')        
plt.ylabel('true label')
plt.show()
