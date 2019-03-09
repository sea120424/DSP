import pandas as pd 
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Input
from keras.layers import Activation
from keras.layers import Dropout
from keras.layers.convolutional import Conv2D, AveragePooling2D, MaxPooling2D
from keras.layers.normalization import BatchNormalization
from keras.layers import Flatten
from keras.layers.advanced_activations import PReLU
from keras.preprocessing.image import ImageDataGenerator
from keras import regularizers, optimizers


def span(num, X):
    Y = np.zeros((X.shape[0], num))
    index = 0
    for i in X:
        Y[index][i] = 1
        index += 1

    return Y

def normalize(X):
    X = (X - X.mean()) / X.std()
    return X

train_data = pd.read_csv('data/data_128.csv')
print(train_data)
train_y = train_data['label']
train_y = np.array(train_y, int)
train_y = span(20,train_y)
print(train_y)

train_x = np.array([row.split(' ') for row in train_data['feature'].tolist()],dtype = float)

train_x = normalize(train_x)
train_x = train_x.reshape(-1,32,32,1)

vaild_data = pd.read_csv('data/data_128_val.csv')
train_vaild_y = vaild_data['label']
train_vaild_y = np.array(train_vaild_y, int)
train_vaild_y = span(20, train_vaild_y)

train_vaild_x = np.array([row.split(' ') for row in vaild_data['feature'].tolist()], dtype = float)
train_vaild_x = normalize(train_vaild_x)
train_vaild_x = train_vaild_x.reshape(-1, 32, 32, 1)

weight_decay = 0.000025
model = Sequential()
model.add(Conv2D(6, kernel_size = (5,5), padding = 'valid', kernel_regularizer = regularizers.l2(weight_decay), input_shape = (32, 32, 1) ))
model.add(PReLU())
#model.add(BatchNormalization())
model.add(MaxPooling2D(pool_size = (2,2), strides = (2,2), padding='valid'))
#model.add(Dropout(0.3))
model.add(Conv2D(16, kernel_size = (5,5), padding = 'valid', kernel_regularizer = regularizers.l2(weight_decay)))
model.add(PReLU())
#model.add(BatchNormalization())
model.add(MaxPooling2D(pool_size = (2,2), strides = (2,2), padding = 'valid'))
#model.add(Dropout(0.4))

model.add(Flatten())
model.add(Dense(units = 120))
#model.add(PReLU())
#model.add(Dropout(0.5))
model.add(Dense(units = 84))
#model.add(PReLU())
#model.add(Dropout(0.4))
model.add(Dense(units = 84))
model.add(Dense(units = 20))
model.add(Activation('softmax'))

model.summary()

datagen = ImageDataGenerator(
        featurewise_center = False,
        samplewise_center = False,
        horizontal_flip = False,
        vertical_flip = False,
)

datagen.fit(train_x)

opt = keras.optimizers.Adam(lr = 0.00005, decay = 1e-6)

model.compile(loss = 'categorical_crossentropy', optimizer = opt, metrics = ['accuracy'])

model.fit_generator(datagen.flow(train_x, train_y, batch_size = 32), epochs = 75, verbose = 1, validation_data = (train_vaild_x, train_vaild_y))

model.save('model/128_CNN_5.h5')

scores = model.evaluate(train_vaild_x, train_vaild_y, batch_size = 32, verbose = 1)
print('result %.3f , loss %.3f' %(scores[1]*100, scores[0]))


