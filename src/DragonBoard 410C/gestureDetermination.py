from __future__ import absolute_import, division, print_function, unicode_literals
import functools
import tensorflow as tf
import numpy as np
from tensorflow import keras
import csv

train_data = []
train_label = []
test_data = []
test_label = []

with open('C:/Users/felix/MakeUofT/venv/src/test_data.csv','rt') as f:
  data = csv.reader(f)
  for row in data:
        test_label.append(int(row[0]))
        temp = []
        for i in range (1, 9001):
            if (len(row) - 1 < i):
                temp.append(float(0))
            else:
                temp.append(float(row[i]))
        test_data.append(np.array(temp))

test_data = np.array(test_data)
test_label = np.array(test_label)

with open('C:/Users/felix/MakeUofT/venv/src/train_data.csv','rt') as f:
    data = csv.reader(f)
    for row in data:
        train_label.append(int(row[0]))
        temp = []
        for i in range(1, 9001):
            if (len(row) - 1 < i):
                temp.append(float(0))
            else:
                temp.append(float(row[i]))
        train_data.append(np.array(temp))

train_data = np.array(train_data)
train_label = np.array(train_label)

motions = ["swipe_up", "swipe_down", "swipe_left", "swipe_right", "cw_circ",
           "ccw_circ", "push_out", "pull_in", "cw_semi", "ccw_semi"]

model = keras.Sequential([
        keras.layers.Dense(9000, activation="relu"),
        keras.layers.Dense(128, activation="relu"),
        keras.layers.Dense(64, activation="relu"),
        keras.layers.Dense(10, activation="softmax") #Probability
    ])

#model = keras.models.load_model("Motion.h5")
model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
model.fit(train_data, train_label, epochs = 20)
model.save("Motion.h5")

test_loss, test_acc = model.evaluate(test_data, test_label)
print("Tested acc", test_acc)

# File name needs the file extension to function
# Return the string with the motion that it detects
def predict(file_name):
    model = keras.models.load_model("Motion.h5")
    with open (file_name, encoding="utf-8") as f:
        for line in f.readlines():
            data = parse
            if (len(data) > 90):
                temp = []
                for str in line:
                    temp.append(float(str))
                while (len(temp) < 9000):
                    temp.append(float(0))
        predict = model.predict(data)

    return motions[predict]

'''
if __name__ == '__main__':
    model = keras.models.load_model("Motion.h5") 
'''