#include "../inc/awgn.h"

extern const double sym2sgnl1[4][2];
extern const double sym2sgnl2[4][2];

void transmitter(int *bit, Complex *signal)
{
	bit_generator(bit);
#if COHERENT == ON
	QPSK_modulator(bit, signal);
#else
	DQPSK_modulator(bit, signal);
#endif
}

void bit_generator(int *bit){
	int n;

	for(n=0; n < BITN; n++)
	{
		bit[n] = rand() % 2;
	}
}

void QPSK_modulator(int (*bit), Complex *signal){
	int n, bit1, bit2, symbol;
	int bin2sym[2][2] = {	/* gray bit map */
		{ 0, 1},
		{ 3, 2}
	};
	
	for(n=0; n < SYMBOLN; n++){
		bit1 = bit[n * 2];
		bit2 = bit[n * 2 + 1];
		symbol = bin2sym[bit1][bit2];
		signal[n].real = sym2sgnl1[symbol][0];
		signal[n].image = sym2sgnl1[symbol][1];
	}
}

void DQPSK_modulator(int *bit, Complex *signal)
{
	/* 假设第一个符号为00 */
	int previous_symbol = 0;
	int now_symbol = 0;
	int i = 0;

	for (i = 0; i < SYMBOLN; i++)
	{
		if (bit[2 * i] == 0)
		{
			if (bit[2 * i + 1] == 0)
			{
				now_symbol = 0;
			}
			else
			{
				now_symbol = 1;
			}
			
		}
		else
		{
			if (bit[2 * i + 1] == 0)
			{
				now_symbol = 3;
			}
			else
			{
				now_symbol = 2;
			}
		}
		now_symbol = (now_symbol + previous_symbol) % 4;
		signal[i].real = sym2sgnl2[now_symbol][0];
		signal[i].image = sym2sgnl2[now_symbol][1];
		previous_symbol = now_symbol;
	}
}
