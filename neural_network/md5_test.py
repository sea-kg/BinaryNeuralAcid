#!/usr/bin/env python3

import math
import random
import hashlib
# from pprint import pprint

class Generation:
    def __init__(self):
        self.__diff = 0.0
        self.__genom = [
            [0.1, 0.26, 0.47, 0.43, 0.37, 0.09, 0.51, 0.46, 0.59, 0.18, 0.29, 0.11, 0.19, 0.04, 0.3, 0.27, ],
            [0.29, 0.11, 0.15, 0.12, 0.03, 0.03, 0.71, 0.34, 0.88, 0.21, 0.88, 0.9, 0.15, 0.32, 0.72, 0.04, ],
            [0.29, 0.11, 0.15, 0.12, 0.03, 0.03, 0.71, 0.34, 0.88, 0.21, 0.88, 0.9, 0.15, 0.32, 0.72, 0.04, ],
            [0.29, 0.11, 0.15, 0.12, 0.03, 0.03, 0.71, 0.34, 0.88, 0.21, 0.88, 0.9, 0.15, 0.32, 0.72, 0.04, ],
        ]

    def random_genom(self):
        for x in self.__genom:
            _max = len(x)
            y = 0
            while y < _max:
                x[y] = random.randint(0,100) / 100
                y += 1

    def print_genom(self):
        print("[")
        for l in self.__genom:
            s = ""
            for i in l:
                s += str(i) + ", "
            print("    [" + s + "],")
        print("]")

    def set_genom(self, _genom):
        y = 0
        while y < len(self.__genom):
            x = 0
            while x < len(self.__genom[y]):
                self.__genom[y][x] = _genom[y][x]
                x += 1
            y += 1

    def get_genom(self):
        return self.__genom

    def set_diff(self, _diff):
        self.__diff = _diff

    def get_diff(self):
        return self.__diff

    def calculate_result(self, _input_vector):
        layer = [
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0
        ]
        i = 0
        while i < len(self.__genom):
            x = 0
            while x < 16:
                layer[x] += _input_vector[x] * self.__genom[i][x]
                x += 1
            i += 1
        return layer

    def update_diff(self, _learns_list):
        _diffs = []
        self.__diff = 0.0
        for _learn in _learns_list:
            _input_vector = _learn['in']
            _output_vector = self.calculate_result(_learn['out'])
            _diff = 0.0
            i = 0
            while i < len(_input_vector):
                val1 = _input_vector[i]
                val2 = _output_vector[i]
                if val1 > val2:
                    _diff += val1 - val2
                else:
                    _diff += val2 - val1
                i += 1
            _diffs.append(_diff)
            self.__diff += _diff

    def mutate(self):
        _max = random.randint(0, 16)
        i = 0
        while i < _max:
            layer = random.randint(0, len(self.__genom)-1)
            k = random.randint(0, 15)
            k1 = random.randint(-100, 100) / 1000
            self.__genom[layer][k] += k1
            self.__genom[layer][k] = math.fabs(self.__genom[layer][k])
            i += 1
    
    def shaffle(self, _gen):
        # _max = random.randint(0, 24)
        _max = 24
        i = 0
        while i < _max:
            layer = random.randint(0, len(self.__genom)-1)
            k = random.randint(0, 15)
            self.__genom[layer][k] = _gen.get_genom()[layer][k]
            i += 1

def input_vector(b):
    ret = []
    for i in b:
       ret.append(i / 255)
    i = len(ret)
    while i < 16:
        ret.append(0.0)
        i += 1
    return ret

def output_vector(b):
    ret = []
    for i in b:
        ret.append(int(i * 255))
    return ret

learning_list = [{
    "in": input_vector(b'GeeksforGeeks'),
    "out": input_vector(hashlib.md5(b'GeeksforGeeks').digest())
},{
    "in": input_vector(b'Some'),
    "out": input_vector(hashlib.md5(b'Some').digest())
},{
    "in": input_vector(b'123456'),
    "out": input_vector(hashlib.md5(b'123456').digest())
},{
    "in": input_vector(b'123'),
    "out": input_vector(hashlib.md5(b'123').digest())
}]

def random_k():
    return random.randint(0,100) / 100

generations = []

i = 0
while i < 40:
    gen = Generation()
    gen.random_genom()
    generations.append(gen)
    i += 1

gen_stable = Generation()

# gen_stable.random_genom()

# 3 layers
# genom_stable = [
#     [0.01, 0.15, 0.66, 0.97, 0.15, 0.3, 0.65, 0.13, 0.23, 0.69, 0.48, 0.16, 0.0, 0.31, 0.47, 0.01, ],
#     [0.1, 0.03, 0.53, 0.93, 0.23, 0.16, 0.39, 0.88, 0.43, 0.0, 0.05, 0.48, 0.56, 0.08, 0.28, 0.06, ],
#     [0.17, 0.58, 0.83, 0.17, 0.06, 0.27, 0.34, 0.37, 0.57, 0.26, 0.77, 0.08, 0.2, 0.43, 0.05, 0.48, ],
# ]

def sort_generations(_gens):
    changes = 1
    while changes:
        changes = 0
        i = 0
        while i < len(_gens) - 1:
            if _gens[i].get_diff() > _gens[i+1].get_diff():
                gen1 = _gens[i]
                _gens[i] = _gens[i+1]
                _gens[i+1] = gen1
                changes += 1
            i += 1
    return _gens

max_cicles = 100000
i = 0
while i < max_cicles:
    i += 1

    for gen in generations:
        gen.update_diff(learning_list)
    generations = sort_generations(generations)

    # print("-----")
    # for gen in generations:
    #     print(gen.get_diff())

    part = int(len(generations) / 3)
    m = 0
    while m < part:
        _gen = Generation()
        _gen.set_genom(generations[random.randint(0,part)].get_genom())
        _gen.shaffle(generations[random.randint(0,part)])
        generations[m + part] = _gen

        _gen2 = Generation()
        _gen2.set_genom(generations[random.randint(0,part)].get_genom())
        _gen2.mutate()
        generations[m + part*2] = _gen2
        m += 1

for gen in generations:
    gen.update_diff(learning_list)
generations = sort_generations(generations)

print("-----")
for gen in generations:
    print(gen.get_diff())
    if gen.get_diff() == 0.0:
        gen.print_genom()

print("-----")
generations[0].print_genom()
print(generations[0].get_diff())

def to_string(_res):
    s = ""
    for i in _res:
        s += chr(i)
    return s



res = generations[0].calculate_result(learning_list[0]['out'])
res_out = output_vector(res)
res_in =  output_vector(learning_list[0]['in'])

print("Output   = ", str(res_out))
print("Expected = ", str(res_in))

print(to_string(res_out))
print(to_string(res_in))


# print(res)
# print("slovar[0] = " + str(slovar[0]))
# print("output_vector(res) = " + str(res))
# _diff = calculate_diff(slovar[0], res)
# print("_diff = " + str(_diff))

# result = hashlib.md5(b'GeeksforGeeks').digest()



# print(len(result.digest()))
