import const
import transmitter
import receiver
import channel
import ber
from random import random

transmit_bit = [0] * const.BITN
receive_bit = [0] * const.BITN

transmit_signal = [0+0j] * const.SYMBOLN
receive_signal = [0+0j] * const.SYMBOLN

f = open(const.FILENAME, 'w')

CNR = 0.0

for Eb_N0 in range(const.SNR_START, const.SNR_STOP + 1):
    CNR = float(Eb_N0) + 3.0
    for loop in range(const.LOOPN):
        transmitter.transmitter(transmit_bit, transmit_signal)
        channel.channel(transmit_signal, receive_signal, CNR)
        receiver.receiver(receive_signal, receive_bit)
        ber.ber(loop, transmit_bit, receive_bit, f, CNR)

f.close()