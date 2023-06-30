

### ML Concepts
### Author: Ta Anh Trung

import random
import math

size = random.randint(10,20)
print(size)
func_V = []

for i in range(size):
    func_V.append(random.randint(1,10))
print(func_V)
print('#############################')
weights = []
for i in range(size):
    weights.append(5)
print(weights)
print('#############################')

### generate looping test cases
def test_generator():
    test_array = []
    for i in range(100):
        temp_array = []
        temp_sum = 0
        for j in range(size):
            temp = random.randint(1,10)
            temp_array.append(temp)
            temp_sum += temp*func_V[j]
        temp_array.append(temp_sum)
        test_array.append(temp_array)
    return test_array

test_array = test_generator()
for i in test_array:
    print(i)
print('###############################')



def predict(weights, test_case):
    sum = 0
    for i in range(len(weights)):
        sum += weights[i]*test_case[i]
    return sum

def update(weight, actual_value, predict_value,variable):
    weight += (float) (0.001*(actual_value - predict_value)*variable)
    weight = (int) (weight*100)
    weight /= 100
    return weight

def LMS_predictor(weights, test_array):
    list_weights = weights
    for i in range(100):
        sum = predict(weights= list_weights, test_case= test_array[i])
        print('Test ', (i+1))
        print(sum, test_array[i][size])
        print(list_weights)
        print('--------')
        for j in range(len(list_weights)):
            list_weights[j] = update(list_weights[j], test_array[i][size], sum, test_array[i][j])
        print(list_weights)
        print('--------')
    return list_weights
    

weights = LMS_predictor(weights, test_array)
print(func_V)






