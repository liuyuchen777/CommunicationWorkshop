#include "const.h"

extern const double sym2sgnl[4][2];

void transmitter(int (*bit), Complex *signal)
{
	bit_generator(bit);
	QPSK_modulator(bit, signal);
}

void bit_generator(int (*bit)){
	int n;

	for(n=0; n < BITN; n++){
    	bit[n] = rand() % 2;
	}
}

void QPSK_modulator(int (*bit), Complex *signal){
	int n, bit1, bit2, symbol;
#if GRAY > 0
	int bin2sym[2][2] = {	/* gray bit map */
		{ 0, 1},
		{ 3, 2}
	};
#else
	int bin2sym[2][2] = {
		{ 0, 1},
		{ 2, 3}
};
#endif
	
	for(n=0; n < SYMBOLN; n++){
		bit1 = bit[n * 2];
		bit2 = bit[n * 2 + 1];
		symbol = bin2sym[bit1][bit2];
		signal[n].real = sym2sgnl[symbol][0];
		signal[n].image = sym2sgnl[symbol][1];
	}
}
