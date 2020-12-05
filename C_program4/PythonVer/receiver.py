import const
import numpy as np

def ofdm_receiver(receive_signal, receive_bit):
    # remove GI
    r = receive_signal[const.GI:]
    # FFT
    R = np.fft.fft(r)
    # symbol decision
    QPSK_demodulator(R, receive_bit)

def MLE(symbol):
    """
    symbol is complex number
    """
    decision = 0
    minimum = 65536.0
    for i in range(const.SYMBOL):
        current = (np.real(symbol) - np.real(const.bit2sym[i]))** 2 \
                + (np.imag(symbol) - np.imag(const.bit2sym[i])) ** 2
        if current <= minimum:
            minimum = current
            decision = i

    return decision

def QPSK_demodulator(receive_signal, receive_bit):
    for i in range(const.SYMBOLN):
        decision = MLE(receive_signal[i])
        if decision == 0:
            receive_bit[i * 2] = 0
            receive_bit[i * 2 + 1] = 0
        elif decision == 1:
            receive_bit[i * 2] = 0
            receive_bit[i * 2 + 1] = 1
        elif decision == 2:
            receive_bit[i * 2] = 1
            receive_bit[i * 2 + 1] = 1
        elif decision == 3:
            receive_bit[i * 2] = 1
            receive_bit[i * 2 + 1] = 0
