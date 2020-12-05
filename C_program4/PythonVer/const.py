# const number
SNR_START = 0
SNR_STOP = 11
SYMBOLN = 64
BITN = SYMBOLN * 2
FILENAME = "./data/OFDM.dat"
SYMBOL = 4
WAVES = 8
GI = 16
LOOPN = 100
DELAY_TIME = 0

# channel type
# AWGN/RAYLEIGH/SELECT
CHANNEL = "AWGN"

bit2sym = (
        0.707106781 + 0.707106781j,
        -0.707106781 + 0.707106781j,
        -0.707106781 - 0.707106781j,
        0.707106781 - 0.707106781j
    )

grey_map = (
        (0, 1),
        (3, 2)
    )