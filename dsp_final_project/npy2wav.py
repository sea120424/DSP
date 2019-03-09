import numpy as np
import os
import argparse
import operator
from scipy.io.wavfile import write
parser = argparse.ArgumentParser(description='numpy to wav Converter')
parser.add_argument('--labelname',required=True,help='input label name. E.g. Frog1')

args = parser.parse_args()

labelname = args.labelname
# collect data path
tmp = {}
for i in ['val']:
    for j in os.listdir(os.path.join(i,labelname)):
        num = int(j.split('.')[0])
        tmp[num] = i
sorted_x = sorted(tmp.items(), key=operator.itemgetter(0))
tmp = [os.path.join(i[1],labelname,str(i[0])+'.npy') for i in sorted_x]
# generate wav file
data = []
for i in tmp:
    d = np.load(i)
    data = np.append(data,d)
scaled = np.int16(data/np.max(np.abs(data))*32767)
write(labelname+'.wav',44100,scaled)

