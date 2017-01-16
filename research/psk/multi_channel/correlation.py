#coding:utf-8
import struct
import sys
import wave
import numpy as np
import scipy.fftpack
from pylab import *

class correlate_calculator:
    codes = []
    code_phase = []
    count = 0
    corr = []
    pair = []


    def __init__(self, codes):
        self.codes = codes
        self.code_phase = [[-2 * c + 1 for c in e] for e in codes]
        self.calc_correlate()

    def correlate(a, b, k):
        new_a = a[k:] + a[0:k]
        return np.correlate(new_a, b)[0]

    def calc_correlate(self):
        self.count = 0
        self.corr = []
        self.pair = []
        for i in range(len(self.code_phase)):
            for j in range(i, len(self.code_phase)):
                self.corr.append([])
                self.pair.append([i, j])
                for k in range(len(self.code_phase[i])):
                    self.corr[self.count].append(correlate_calculator.correlate(self.code_phase[i], self.code_phase[j], k))
                self.count += 1

    def get_correlate(self):
        return self.corr

    def plot_correlate(self):
        arr = np.array(self.corr)

        minimum = arr.min()
        maximum = arr.max()

        subplot(self.count + 1, 1, 1)
        for i in range(self.count):
            plot(self.corr[i])
        ylabel("all")
        axis([-4, len(self.corr[0]), minimum, maximum])

        for i in range(self.count):
            subplot(self.count + 1, 1, i + 2)
            plot(self.corr[i])
            ylabel(str(self.pair[i]))
            axis([-4, len(self.corr[0]), minimum, maximum])

        show()

    def get_self_correlate(self):
        self_correlates = []
        for i in range(len(self.pair)):
            if self.pair[i][0] == self.pair[i][1]:
                self_correlates.append(self.corr[i])
        return self_correlates

    def plot_self_correlate(self):
        arr = np.array(self.corr)

        count = 0
        for i in range(len(self.pair)):
            if self.pair[i][0] == self.pair[i][1]:
                count += 1
                subplot(len(self.codes), 1, count)
                plot(self.corr[i])
                ylabel(str(self.pair[i]))
                axis([-4, len(self.corr[0]), arr[i].min(), arr[i].max()])

        show()


if __name__ == '__main__':
    # code = [[0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    #     1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0],
    #    [0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
    #     0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1],
    #    [0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    #     1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1],
    #    [0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0,
    #     1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1]]

    codes = [[1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0], [1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0]]

    corr_c = correlate_calculator(codes)
    corr_c.plot_correlate()

    # print(corr)

    # print(code_phase)
