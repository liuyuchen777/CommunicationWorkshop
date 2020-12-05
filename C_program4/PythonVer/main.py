# import my script
import const
import transmitter
import receiver
import channel
import ber
from random import random
# import lib
import numpy as np
import time

def main():
    # define array
    receive_bit = np.zeros(const.BITN, dtype=int)
    transmit_bit = np.zeros(const.BITN, dtype=int)
    receive_signal = np.zeros((const.SYMBOLN + const.GI), dtype=complex)
    transmit_signal = np.zeros((const.SYMBOLN + const.GI), dtype=complex)

    try:
        f = open(const.FILENAME, 'w')
    except IOError as err:
        print("File error", str(err))
    finally:
        print("[%s] LOOPN=%d, symbol number=%d, SNR from %d~%d dB, channel=%s"
            %(time.asctime(time.localtime(time.time())), const.LOOPN, const.SYMBOLN, const.SNR_START, const.SNR_STOP, const.CHANNEL))
        f.write("[ %s ] LOOPN=%d, symbol number=%d, SNR from %d~%d dB, channel=%s"
            %(time.asctime(time.localtime(time.time())), const.LOOPN, const.SYMBOLN, const.SNR_START, const.SNR_STOP, const.CHANNEL))

    CNR = 0.0
    for Eb_N0 in range(const.SNR_START, const.SNR_STOP + 1):
        CNR = float(Eb_N0) + 3.0
        for loop in range(const.LOOPN):
            # transmission
            transmitter.ofdm_transmitter(transmit_bit, transmit_signal)
            if const.CHANNEL == "AWGN":
                channel.AWGN(transmit_signal, receive_signal, CNR)
            elif const.CHANNEL == "RAYLEIGH":
                channel.Rayleigh(transmit_signal, receive_signal, CNR)
            receiver.ofdm_receiver(receive_signal, receive_bit)
            ber.ber(loop, transmit_bit, receive_bit, f, CNR)

    f.close()

if __name__ == "__main__":
    main()