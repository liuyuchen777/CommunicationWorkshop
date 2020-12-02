import const

total_error = 0

def ber(loop, transmit_bit, receive_bit, f, CNR):
    for i in range(const.BITN):
        if transmit_bit[i] != receive_bit[i]:
            total_error += 1
    
    if loop == (const.LOOPN - 1):
        Pe = total_error / (const.LOOPN * const.BITN)
        print("Eb/N0 = %f, Average BER = %f" % 
            ((CNR - 3.0), Pe))
        f.write("Eb/N0 = %f, Average BER = %f" % 
            ((CNR - 3.0), Pe))
        total_error = 0