#coding:utf-8
import pyaudio
import time
import struct
import sys
import wave
import numpy as np
import scipy.fftpack
from pylab import *

#################################
palette = ["r", "g", "b", "c"]

#################################
CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 48000
RECORD_SECONDS = 2
SOUND_VELOCITY = 340.0

#################################
N = 32    # FFTのサンプル数
SHIFT_N = int(N / 4)
target_freq = 6000 # 搬送波の周波数
freqList = np.fft.fftfreq(N, d=1.0/RATE)  # 周波数軸の値を計算
def get_nearest_idx(list, num):
    idx = np.abs(np.asarray(list) - num).argmin()
    return idx
target_idx = get_nearest_idx(freqList, target_freq)

SAMPLE_RANGE = int(4.0 * RATE / (SOUND_VELOCITY * SHIFT_N)) # 10m分の差
#################################
# code = [[1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0], [1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0]]

code = [[0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0], [0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1], [1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1]]


temp_phase = [[np.exp(1.0j * (1 - c) * np.pi) for c in element] for element in code]

code_phase = []
for element in temp_phase:
    temp = []
    for c in element:
        temp.extend(4 * [c])
    code_phase.append(temp)

def plot_ch(corr_abs, ch):
    c = corr_abs[ch]
    plot(c, color=palette[ch])
    axis([0, len(corr_abs[0]), array(corr_abs).min(), array(corr_abs).max()])
    xlabel("time [/32 sample]")
    ylabel("correlation ch{}".format(ch))

def get_data(stream):
    x = []
    for i in range(0, int(RATE * RECORD_SECONDS / CHUNK)):
        for j in range(CHUNK):
            b = stream.read(1)
            val = struct.unpack("h", b)[0]
            x.append(val)
    return x

def get_data_from_stdin():
        # wf = open(rawfile, "rb")
        x = []
        for i in range(int(RATE * RECORD_SECONDS)):
            b = sys.stdin.buffer.read(2)
            if b == b"": break;
            val = struct.unpack("h", b)[0]
            x.append(val)
        return x

def get_time(x):
    start = 0  # サンプリングする開始位置

    X_time = []
    amp_time = []
    phase_time = []

    while start + N < len(x):
        Xs = np.fft.fft(x[start:start+N])  # FFT

        X = Xs[target_idx]
        amp = np.sqrt(X.real ** 2 + X.imag ** 2)
        phase = np.arctan2(int(X.imag), int(X.real))

        X_time.append(X)
        amp_time.append(amp)
        phase_time.append(phase)

        start += int(SHIFT_N)


    corr = [np.correlate(X_time, c, "valid") for c in code_phase]

    corr_abs = [[np.sqrt(c.real ** 2 + c.imag ** 2) for c in element] for element in corr]

    v = np.array(corr_abs)

    max_tupple = np.unravel_index(v.argmax(), v.shape)
    max_idx = max_tupple[1]
    idx_min = max(0, max_idx - SAMPLE_RANGE)
    idx_max = min(v.shape[1], max_idx + SAMPLE_RANGE + 1)

    peak_idx = []
    for ch in v:
        peak_idx.append(ch[idx_min:idx_max].argmax() + idx_min)

    subplot(411)
    plot(amp_time)

    subplot(412)
    plot(phase_time)

    ch = 0
    subplot(413)
    for c in corr_abs:
        plot(range(0, len(c)), c[:], color=palette[ch])
        axvline(x=peak_idx[ch], linestyle="dotted", color=palette[ch])
        ch += 1
    axis([0, len(corr_abs[0]), array(corr_abs).min(), array(corr_abs).max()])
    xlabel("time [/8 sample]")
    ylabel("correlation")


    time = [float(i) * SHIFT_N / RATE for i in peak_idx]
    return time

class calculator:
    """3つのスピーカーからの位相の時間差から，録音機器の位置を推定する"""
    speaker_pos = np.array([])
    k = 1.0
    epsilon = 2.0
    osc_th = 100
    pos = np.array([0.0, 1.0])

    def __init__(self, speaker_pos):
        self.speaker_pos = np.array(speaker_pos)

    def get_pos(self, x):
        t = self.get_time(x)
        if not t:
            print("cannot detect valid peak")
            return self.pos

        return self.calc_pos(t)

    def sekine_calc_pos(self, t):
        L01 = SOUND_VELOCITY * (t[2] - t[0])
        L12 = SOUND_VELOCITY * (t[0] - t[1])
        alpha = np.linalg.norm(self.speaker_pos[1] - self.speaker_pos[0])
        beta = np.linalg.norm(self.speaker_pos[2] - self.speaker_pos[0])

        R12 = np.sqrt(alpha ** 2 - L12 ** 2) / 2
        R01 = np.sqrt(beta ** 2 - L01 ** 2) / 2

        A = (L12 ** 2) / (L01 ** 2) - (R12 ** 2) / (R01 ** 2)
        B = L12 * (alpha + beta) / (L01 ** 2)
        C = ((alpha + beta) ** 2) / (L01 ** 2) + (R12 ** 2) / (R01 ** 2) - 1

        x1 = (-B + np.sqrt(B ** 2 - A * C)) / A
        print("x1: {}".format(x1))
        y1 = np.sqrt(x1 ** 2 - 1)

        x = L12 * x1 / 2 + alpha / 2
        y = R12 * y1

        return [x, y]

    def calc_pos(self, t):
        g1 = SOUND_VELOCITY * (t[1] - t[0])
        g2 = SOUND_VELOCITY * (t[2] - t[0])
        print("time: {}".format(t))
        print("gap: {}".format([g1, g2]))

        dp_norm = 0.0
        pos = np.array([0.0, 1.0])
        while True:
            if dp_norm > self.osc_th:
                print("newton method oscilated")
                break;

            diff_F = np.array([[self.diff(pos, 1, 0), self.diff(pos, 1, 1)],
                               [self.diff(pos, 2, 0), self.diff(pos, 2, 1)]])
            P = np.array([g1 - self.calc_gap(pos, 1),
                          g2 - self.calc_gap(pos, 2)])

            dp = np.linalg.inv(diff_F).dot(P)
            pos += self.k * dp
            dp_norm = np.linalg.norm(dp)
            if dp_norm < self.epsilon:
                print("newton method succeeded")
                break;

        self.pos = pos
        self.pos[1] = np.absolute(self.pos[1]) # y軸をプラスに限定
        print("position: {}".format(self.pos))
        return self.pos

    def calc_gap(self, pos, i): # スピーカiとスピーカ0の距離の差
        return np.linalg.norm(self.speaker_pos[i] - pos) - np.linalg.norm(self.speaker_pos[0] - pos)

    def diff(self, pos, i, j): # i: f_i, j:0のときxで微分，1の時yで微分
        r_0 = self.speaker_pos[0] - pos
        r_i = self.speaker_pos[i] - pos
        return r_0[j] / np.linalg.norm(r_0) - r_i[j] / np.linalg.norm(r_i)

    def plot_position(self):
        for i in range(3):
            scatter(self.speaker_pos[i][0], self.speaker_pos[i][1], color="r")
        scatter(self.pos[0], self.pos[1], color="b")

    def get_time(self, x):
        start = 0  # サンプリングする開始位置

        X_time = []
        amp_time = []
        phase_time = []

        while start + N < len(x):
            Xs = np.fft.fft(x[start:start+N])  # FFT

            X = Xs[target_idx]
            amp = np.sqrt(X.real ** 2 + X.imag ** 2)
            phase = np.arctan2(int(X.imag), int(X.real))

            X_time.append(X)
            amp_time.append(amp)
            phase_time.append(phase)

            start += int(SHIFT_N)


        corr = [np.correlate(X_time, c, "valid") for c in code_phase]

        corr_abs = [[np.sqrt(c.real ** 2 + c.imag ** 2) for c in element] for element in corr]

        v = np.array(corr_abs)
        sorted_idx = np.array([np.argsort(c)[::-1] for c in v])

        valid_sets = [] # 上から何番目のサンプルかを格納
        range_limit = 5 # 上からrange_limit番目までのサンプルについて調べる
        for i in range(range_limit):
            idx0 = sorted_idx[0][i]
            for j in range(range_limit):
                idx1 = sorted_idx[1][j]
                for k in range(range_limit):
                    idx2 = sorted_idx[2][k]
                    if self.idx_validity_check([idx0, idx1, idx2]):
                        valid_sets.append([i, j, k])

        most_reliable_idx = []
        if valid_sets: # valid_setsが空でなかったら
            most_reliable_set = valid_sets[np.argmin([np.linalg.norm(c) for c in valid_sets])]
            for i in range(len(most_reliable_set)):
                most_reliable_idx.append(sorted_idx[i][most_reliable_set[i]])
        # max_tupple = np.unravel_index(v.argmax(), v.shape)
        # max_idx = max_tupple[1]
        # idx_min = max(0, max_idx - SAMPLE_RANGE)
        # idx_max = min(v.shape[1], max_idx + SAMPLE_RANGE + 1)
        #
        # peak_idx = []
        # for ch in v:
        #     peak_idx.append(ch[idx_min:idx_max].argmax() + idx_min)

        subplot(411)
        plot(amp_time)

        subplot(412)
        plot(phase_time)

        ch = 0
        subplot(413)
        for c in corr_abs:
            plot(range(0, len(c)), c[:], color=palette[ch])
            if most_reliable_idx:
                axvline(x=most_reliable_idx[ch], linestyle="dotted", color=palette[ch])
            ch += 1

        if most_reliable_idx:
            print("most_reliable_idx: {}".format(most_reliable_idx))
            axis([np.min(most_reliable_idx) - 100, np.max(most_reliable_idx) +  100, array(corr_abs).min(), array(corr_abs).max()])
        xlabel("time [/8 sample]")
        ylabel("correlation")


        time = [float(i * SHIFT_N) / RATE for i in most_reliable_idx]
        return time

    def idx_validity_check(self, idxs):
        g1 = idxs[1] - idxs[0]
        g2 = idxs[2] - idxs[0]
        # s0が原点，s1はx軸上正の位置，s1, s2はs0から等距離にある，という前提
        # まず，そもそも距離の差がスピーカーの距離の差を超えていないかチェック
        # s1, s0間距離
        l = np.linalg.norm(self.speaker_pos[1] - self.speaker_pos[0])
        # idxの尺度での差
        l_index = float(l * RATE) / (SOUND_VELOCITY * SHIFT_N)
        if not ((np.absolute(g1) < l_index) and (np.absolute(g2) < l_index)):
            return False

        # 次に，s1 - s0, s2 - s0が幾何関係を満足しているかチェック
        if g1 + g2 <= 0:
            return False

        # すべてのチェックをパスしたか
        return True



def test_calculator():
    speaker_pos = np.array([[0.0, 0.0],
                            [2.0, 0.0],
                            [-2.0, 0.0]])
    calc = calculator(speaker_pos)

    pos = np.array([1, 3])
    t0 = np.linalg.norm(pos - speaker_pos[0]) / SOUND_VELOCITY
    t1 = np.linalg.norm(pos - speaker_pos[1]) / SOUND_VELOCITY
    t2 = np.linalg.norm(pos - speaker_pos[2]) / SOUND_VELOCITY

    t = np.array([t0, t1, t2])
    print("true pos: {}".format(pos))
    print("ans pos:  {}".format(calc.sekine_calc_pos(t)))

def len_to_pos(length):
    pos = []
    pos.append([0.0, 0.0])
    pos.append([length[0], 0.0])
    cos_theta = (length[0] * length[0] + length[2] * length[2] - length[1] * length[1]) / (2 * length[0] * length[2])
    theta = np.arccos(cos_theta)
    x = length[2] * np.cos(theta)
    y = length[2] * np.sin(theta)
    pos.append([x, y])
    return pos


######################################
if __name__ == "__main__" :
    test_calculator()

    p = pyaudio.PyAudio()
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)


    # lengths = [4.23, 8.60, 4.38]
    # speaker_pos = len_to_pos(lengths)
    speaker_pos = [[0, 0], [2, 0], [-2, 0]]
    calc = calculator(speaker_pos)

    scatter(0, 0)
    temp = 0
    while True:
        clf()
        print()
        x = get_data(stream)
        # x = get_data_from_stdin()
        # time = get_time(x)
        pos = calc.get_pos(x)

        # temp_x = x[0:1000]
        # plot(range(0, len(temp_x)), temp_x[:])
        # axis([0, len(temp_x), min(temp_x), max(temp_x)])
        # xlabel("time [/48000]")
        # ylabel("wave")

        subplot(414)
        calc.plot_position()
        pause(0.001)


    # 描画
    subplot(611)  # 3行1列のグラフの1番目の位置にプロット
    plot(range(0, len(amp_time)), amp_time[:])
    axis([0, len(amp_time), min(amp_time), max(amp_time)])
    xlabel("time [/32 sample]")
    ylabel("amplitude")

    subplot(612)  # 3行1列のグラフの1番目の位置にプロット
    plot(range(0, len(phase_time)), phase_time[:])
    axis([0, len(phase_time), min(phase_time), max(phase_time)])
    xlabel("time [/32 sample]")
    ylabel("phase")

    subplot(613)  # 3行1列のグラフの1番目の位置にプロット

    ch = 0
    for c in corr_abs:
        plot(c, color=palette[ch])
        ch += 1
    axis([0, len(corr_abs[0]), array(corr_abs).min(), array(corr_abs).max()])
    xlabel("time [/32 sample]")
    ylabel("correlation")

    subplot(614)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 0)

    subplot(615)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 1)

    subplot(616)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 2)

    show()
