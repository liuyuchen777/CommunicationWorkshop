import numpy as np
import const

# Rayleigh channel
def Rayleigh(transmit_signal, receive_signal, CNR):
    pass

def AWGN(transmit_signal, receive_signal, CNR):
    """
    AWGN channel, simplely add some Gaussian noise
    transmit_signal: ndarray complex
    receive_signak: ndarray complex
    """
    # calculate sigma2
    sigma2 = 10 ** ((-CNR) / 10)
    # add Gaussian noise to channel
    receive_signal[:] = transmit_signal[:] + np.random.randn((const.SYMBOLN + const.GI)) * sigma2