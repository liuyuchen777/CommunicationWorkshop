#include "const.h"

void transmitter(vector<u32> &bit, vector<Complex> &signal)
{
	bit_generator(bit);
	if (RECEIVER == "COHERENT")
		QPSK_modulator(bit, signal);
	else if (RECEIVER == "NON_COHERENT")
		DQPSK_modulator(bit, signal);
	else if (RECEIVER == "OFDM")
		OFDM_modulator(bit, signal);
}

void bit_generator(vector<int> &bit)
{
	int n;

	for(n = 0; n < BITN; n++)
	{
		bit[n] = rand() % 2;
	}
}

void QPSK_modulator(vector<u32> &bit, vector<Complex> &signal)
{
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

void DQPSK_modulator(vector<u32> &bit, vector<Complex> &signal)
{
	/* 假设第一个符号为00 */
	u32 previous_symbol = 0;
	u32 now_symbol = 0;
	u32 i = 0;

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
		signal[i].real() = sym2sgnl2[now_symbol][0];
		signal[i].image() = sym2sgnl2[now_symbol][1];
		previous_symbol = now_symbol;
	}
}

void OFDM_modulator(vector<u32> &bit, vector<Complex> &signal)
{
	Complex Z[SYMBOLN];
	Complex temp = {0.0, 0.0};
	int n = 0, k = 0, i = 0;
	/* bit -> QPSK symbol */
	QPSK_modulator(bit, Z);
	/* IDFT */
	for (k = 0; k < SYMBOLN; k++)
	{
		temp.real() = 0.0;
		temp.image() = 0.0;
		for (n = 0; n < SYMBOLN; n++)
		{
			temp = complex_add(temp, complex_multiply(Z[n], Exp(2 * PI * k * n / SYMBOLN)));
		}
		signal[GI + k].real = 1 / sqrt(SYMBOLN) * temp.real;
		signal[GI + k].image = 1 / sqrt(SYMBOLN) * temp.image;
	}
	/* insert GI */
	for (i = 0; i < GI; i++)
	{
		signal[i].real = signal[N + i].real;
		signal[i].image = signal[N + i].image;
	}
}