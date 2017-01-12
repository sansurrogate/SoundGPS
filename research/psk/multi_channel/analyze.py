#coding:utf-8
import struct
import sys
import wave
import numpy as np
import scipy.fftpack
from pylab import *

from position import get_time

palette = ["r", "g", "b", "c"]

def get_nearest_idx(list, num):
    idx = np.abs(np.asarray(list) - num).argmin()
    return idx

def plot_ch(corr_abs, ch):
    c = corr_abs[ch]
    plot(range(0, len(c)), c[:], color=palette[ch])
    axis([0, len(corr_abs[0]), array(corr_abs).min(), array(corr_abs).max()])
    xlabel("time [/32 sample]")
    ylabel("correlation ch{}".format(ch))


if __name__ == "__main__" :
    fs = 48000 # サンプリング周波数

    # rawデータをxに読み込み
    rawfile = sys.argv[1]
    wf = open(rawfile, "rb")
    x = []
    while True:
        b = wf.read(2)
        if b == b"": break;
        val = struct.unpack("h", b)[0]
        x.append(val)
    wf.close()

    start = 0  # サンプリングする開始位置
    N = 32    # FFTのサンプル数
    target_freq = 6000 # 搬送波の周波数

    X_time = []
    amp_time = []
    phase_time = []

    freqList = np.fft.fftfreq(N, d=1.0/fs)  # 周波数軸の値を計算
    target_idx = get_nearest_idx(freqList, target_freq)

    while start + N < len(x):
        X = np.fft.fft(x[start:start+N])  # FFT

        amplitudeSpectrum = [np.sqrt(c.real ** 2 + c.imag ** 2) for c in X]  # 振幅スペクトル
        phaseSpectrum = [np.arctan2(int(c.imag), int(c.real)) for c in X]    # 位相スペクトル

        X_time.append(X[target_idx])
        amp_time.append(amplitudeSpectrum[target_idx])
        phase_time.append(phaseSpectrum[target_idx])

        start += N

    # Gold系列
    # code = [[0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    #     1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0],
    #    [0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
    #     0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1],
    #    [0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    #     1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1],
    #    [0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0,
    #     1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1]]

    code = [[1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0], [1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0], [1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0]]

    temp_phase = [[np.exp(1.0j * (1 - c) * np.pi) for c in element] for element in code]

    code_phase = []
    for element in temp_phase:
        temp = []
        for c in element:
            temp.extend(4 * [c])
        code_phase.append(temp)

    corr = [np.correlate(X_time, c, "valid") for c in code_phase]

    corr_abs = [[np.sqrt(c.real ** 2 + c.imag ** 2) for c in element] for element in corr]

    time = get_time(x)

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
        plot(range(0, len(c)), c[:], color=palette[ch])
        axvline(x=time[ch], linestyle="dotted", color=palette[ch])
        ch += 1
    axis([0, len(corr_abs[0]), array(corr_abs).min(), array(corr_abs).max()])
    xlabel("time [/32 sample]")
    ylabel("correlation")

    subplot(614)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 0)
    axvline(x=time[0], linestyle="dotted", color=palette[0])


    subplot(615)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 1)
    axvline(x=time[1], linestyle="dotted", color=palette[1])


    subplot(616)  # 3行1列のグラフの1番目の位置にプロット
    plot_ch(corr_abs, 2)
    axvline(x=time[2], linestyle="dotted", color=palette[2])

    # # 波形を描画
    # subplot(311)  # 3行1列のグラフの1番目の位置にプロット
    # plot(range(start, start+N), x[start:start+N])
    # axis([start, start+N, -32768, 32767])
    # xlabel("time [sample]")
    # ylabel("amplitude")
    #
    # # 振幅スペクトルを描画
    # subplot(312)
    # plot(freqList, amplitudeSpectrum, marker= 'o', linestyle='-')
    # axis([0, fs/2, 0, max(amplitudeSpectrum) + 10000000])
    # xlabel("frequency [Hz]")
    # ylabel("amplitude spectrum")
    #
    # # 位相スペクトルを描画
    # subplot(313)
    # plot(freqList, phaseSpectrum, marker= 'o', linestyle='-')
    # axis([0, fs/2, -np.pi, np.pi])
    # xlabel("frequency [Hz]")
    # ylabel("phase spectrum")


    show()
