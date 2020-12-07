import const
import numpy as np
import random

def coherent_transmitter(transmit_bit, transmit_signal):
    pass

def ofdm_transmitter(transmit_bit, transmit_signal):
    # generate bit
    generate_bit(transmit_bit)
    # bit -> symbol
    z = np.zeros(const.SYMBOLN, dtype=complex)
    QPSK_modulator(transmit_bit, z)
    # ifft
    transmit_signal[const.GI:] = np.fft.ifft(z)
    # add GI
    transmit_signal[:const.GI] = transmit_signal[const.SYMBOLN:] 

def QPSK_modulator(transmit_bit, transmit_signal):
    """
    transmit_bit: ndarray int
    transmit_signal: ndarray complex
    """
    for i in range(const.SYMBOLN):
        transmit_signal[i] = const.bit2sym[
                const.grey_map[np.int(transmit_bit[i * 2])][np.int(transmit_bit[i * 2 + 1])]
                ]

def generate_bit(transmit_bit):
    for i in range(const.BITN):
        if random.random() >= 0.5:
            transmit_bit[i] = 1
        else:
            transmit_bit[i] = 0