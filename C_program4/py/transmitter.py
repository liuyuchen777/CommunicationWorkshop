import const
from random import random
from math import cos
from math import sin
from math import pi

def transmitter(transmit_bit, transmit_signal):
    bit_generator(transmit_bit)
    QPSK_modulator(transmit_bit, transmit_signal)
    ofdm(transmit_signal)

def bit_generator(transmit_bit):
    for i in range(const.BITN):
        transmit_bit[i] = random() % 2 

def QPSK_modulator(transmit_bit, transmit_signal):
    map = (
        (0, 1),
        (3, 2)
    )
    for i in range(const.SYMBOLN):
        transmit_signal = const.bit2sym[
                map[transmit_bit[i * 2]][transmit_bit[i * 2 + 1]]
                ]

def ofdm(transmit_signal):
    for k in range(const.SYMBOLN):
        for n in range(const.SYMBOLN):
            temp = temp + transmit_signal[n] * (cos(2 * pi * k * n / const.SYMBOLN) + sin(2 * pi * k * n / const.SYMBOLN) * j)