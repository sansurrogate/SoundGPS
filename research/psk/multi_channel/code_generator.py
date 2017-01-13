#coding:utf-8
import struct
import sys
import wave
import numpy as np
import scipy.fftpack
from pylab import *

from correlation import correlate_calculator

def generate_m(n, gen_poly):
    m = []

    for i in range(n):
        m.append(0)
    m[0] = 1

    for i in range(n, 2 ** n - 1):
        bit = 0
        for j in gen_poly:
            bit ^= m[i - j]
        m.append(bit)

    return m

def generate_gold(m1, m2):
    golds = []
    for i in range(len(m1)):
        shift_m2 = m2[i:] + m2[:i]
        gold = [m1[i] ^ shift_m2[i] for i in range(len(m1))]
        golds.append(gold)
    return golds

def generate_kasami(n, m):
    s = int(2 ** (n / 2) + 1)
    kasamis = []
    u = [m[(j * s) % len(m)] for j in range(int(2 ** (n / 2)) - 1)]
    for i in range(int(2 ** (n / 2)) - 1):
        shift_u = u[i:] + u[:i]
        kasami = [m[j] ^ shift_u[j % len(shift_u)] for j in range(len(m))]
        # kasami += m[len(shift_u):]
        kasamis.append(kasami)
    return kasamis

if __name__ == '__main__':
    # n = 9
    # gen_polys = [[9, 4], [9, 6, 4, 3], [9, 8, 5, 4]]
    # gen_polys = [[9, 4], [9, 6, 4, 3]]
    # gen_polys = [[9, 4], [9, 8, 5, 4]]

    n = 10
    gen_polys = [[10, 3], [10, 3, 2, 1]]

    # codes = [generate_m(n, g) for g in gen_polys]
    ms = [generate_m(n, g) for g in gen_polys]
    golds = generate_gold(ms[0], ms[1])
    kasamis = generate_kasami(n, ms[0])
    # codes = golds[:3]
    codes = kasamis[:3]
    corr_c = correlate_calculator(codes)
    corr = corr_c.get_correlate()
    print(corr)
    # corr_c.plot_self_correlate()
    corr_c.plot_correlate()
