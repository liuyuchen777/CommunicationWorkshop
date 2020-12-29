#include "const.h"

Complex sym2sgnl[SYMBOL] = {
		{ OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2,-OneBySqrt2},
		{ OneBySqrt2,-OneBySqrt2}
};

/* modulator */
void QPSK_modulator(vector<u32> &bit, vector<Complex> &signal);

void OFDM_modulator(vector<u32> &bit, vector<Complex> &signal, int tag);

void bit_generator(vector<u32> &bit);

void bit_generator(vector<u32> &bit1, vector<u32> &bit2)
{
	int n;

	for(n = 0; n < BITN; n++)
	{
		bit1[n] = rand() % 2;
		bit2[n] = rand() % 2;
	}
}

void transmitter(vector<u32> &bit1, vector<u32> &bit2, vector<Complex> &signal1, vector<Complex> &signal2)
{
	bit_generator(bit1, bit2);
	// antenna1
	OFDM_modulator(bit1, signal1, 1);
	// antenna2
	OFDM_modulator(bit2, signal2, 2);
}

void QPSK_modulator(vector<u32> &bit, vector<Complex> &signal)
{
	int n, bit1, bit2, symbol;
	int bin2sym[2][2] = {	/* gray bit map */
		{ 0, 1},
		{ 3, 2}
	};
	
	for(n = 0; n < SYMBOLN; n++){
		bit1 = bit[n * 2];
		bit2 = bit[n * 2 + 1];
		symbol = bin2sym[bit1][bit2];
		signal[n] = sym2sgnl[symbol];
	}
}

void OFDM_modulator(vector<u32> &bit, vector<Complex> &signal, int tag)
{
	vector<Complex> Z(SYMBOLN);
	Complex temp = {0.0, 0.0};
	int n = 0, k = 0;
	/* bit -> QPSK symbol */
	QPSK_modulator(bit, Z);
	/* IDFT */
	for (k = 0; k < SYMBOLN; k++)
	{
		temp = {0.0, 0.0};
		for (n = 0; n < SYMBOLN; n++)
		{
			temp += Z[n] * Exp(2 * PI * k * n / SYMBOLN);
		}
		signal[2 + k] = temp * ((double)1 / sqrt(SYMBOLN));
	}
	if (tag == 1)
	{
		signal[0] = {OneBySqrt2, OneBySqrt2};
		signal[1] = {0.0, 0.0};
	}
	else if (tag == 2)
	{
		signal[0] = {0.0, 0.0};
		signal[1] = {OneBySqrt2, OneBySqrt2};
	}
}