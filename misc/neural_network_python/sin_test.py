#!/usr/bin/env python3

import math
import random

# angel_in1 = 

def random_k():
    return random.randint(-100,100) / 100

def random_c():
    return random.randint(-1000,1000) / 100

def random_genom():
    return {
        "c1": random_c(),
        "k1": random_k(),
        "k2": random_k(),
        "k3": random_k(),
        "k4": random_k(),
        "k5": random_k(),
        "k6": random_k(),
    }


def caluculate_result(_genom, a1):
    n1 = a1 * _genom["k1"] + _genom["k3"] * _genom["c1"]
    n2 = a1 * _genom["k2"] + _genom["k4"] * _genom["c1"]
    n3 = n1 * _genom["k5"] + n2 * _genom["k6"]
    return n3

genom_stable = {'c1': 5.14, 'k1': -0.37, 'k2': -0.81, 'k3': -0.3, 'k4': -0.97, 'k5': -0.85, 'k6': 0.4}
# random_genom()
print(genom_stable)

def caluculate_diff(_genom):
    angel = -3.14
    max_angel = 3.14
    _diff = 0.0
    while angel < max_angel:
        sa = math.sin(angel)
        nsa = caluculate_result(_genom, angel)
        # print(str(sa) + " == " + str(nsa))
        sa = math.fabs(sa)
        nsa = math.fabs(nsa)
        if sa > nsa:
            _diff += sa - nsa
        else:
            _diff += nsa - sa
        _diff += sa - sa
        angel += 0.1
    return _diff

diff_stable = caluculate_diff(genom_stable)

max_cicles = 1000000
i = 0
while i < max_cicles:
    i += 1
    genom_next = random_genom()
    diff_next = caluculate_diff(genom_next)
    if diff_next < diff_stable:
        genom_stable = genom_next
        diff_stable = diff_next
        print(genom_stable)
        print(diff_stable)

c1 = genom_stable["c1"]
k1 = genom_stable["k1"]
k2 = genom_stable["k2"]
k3 = genom_stable["k3"]
k4 = genom_stable["k4"]
k5 = genom_stable["k5"]
k6 = genom_stable["k6"]

result = "sin(a) = "
result += "a * " + str(k1 * k5 + k2 * k6)
result += " + " + str(k3 * c1 * k5 + k4 * k6 * c1)

result += "\n diff_stable = " + str(diff_stable)
print(result)


# angel = -3.14
# max_angel = 3.14
# _diff = 0.0
# while angel < max_angel:
#     sa = math.sin(angel)
#     nsa = angel * 0.632 + angel * 0.3655 + 0.0007
#     print(str(sa) + " == " + str(nsa))
#     angel += 0.1

# sin_r = (a1 * -0.79 + 0.36 * 7.14) * -0.8 (a1 * -0.85 + -0.67 * 7.14) * -0.43