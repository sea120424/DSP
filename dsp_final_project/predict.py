import numpy as np
from keras.models import load_model
import scipy
from skimage import transform, data
import csv
import pandas as pd
np.set_printoptions(threshold = 10000)

def write_into_csv(file_name, spectrogram):
    S = transform.resize(spectrogram, (32,32))
    S = S.reshape(-1)
    #print(S)
    S = str(S)
    S = ''.join( c for c in S if c not in '[]')
    with open(file_name, 'a', newline = '') as csvfile:
        writer = csv.writer(csvfile, delimiter = ',')
        writer.writerow([S])

def normalize(X):
    X = (X - X.mean()) / X.std()
    return X

test_data = np.load('test.npy')
#test_data = normalize(test_data)
new_model = load_model('model/128_CNN_5.h5')

feq = 44100
window_size = 128
mode = 'magnitude'

out_file = 'data/test.csv'
with open(out_file, 'w', newline = '') as csvfile:
    writer = csv.writer(csvfile, delimiter = ',')
    writer.writerow(['feature'])

result = []

for i in test_data:
    #print(i)
    F, T, S = scipy.signal.spectrogram(i, fs = feq, nperseg = window_size, window = 'hamming', mode = mode) 
    #S = transform.resize(S, (32,32))
    #S = S.reshape(-1)
    write_into_csv(out_file,S)

test = pd.read_csv('data/test.csv')
test_x = np.array([row.split(' ') for row in test['feature'].tolist()],dtype = float)
test_x = normalize(test_x)
test_x = test_x.reshape(-1,32,32,1)

ans = new_model.predict(test_x)
result = []
for i in ans:
    result.append(np.argmax(i))

results = np.array(result)
print(results)
print(results.shape)
np.save('results.npy',results)
