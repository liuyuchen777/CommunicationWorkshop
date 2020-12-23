#include "const.h"

const Complex sym2sgnl[SYMBOL] = {
		{ OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2,-OneBySqrt2},
		{ OneBySqrt2,-OneBySqrt2}
};

void bit_generator(vector<u32> &bit);

void convolution(vector<u32> &bit, vector<u32> &encode_bit);

void interleaver(vector<u32> &bit);

void bit_generator(vector<u32> &bit1, vector<u32> &bit2)
{
	int n;

	// insert pilot
	bit1[0] = 1;
	bit1[1] = 1;
	bit2[2] = 1;
	bit2[3] = 1;
	for(n = 4; n < BITN; n++)
	{
		bit1[n] = rand() % 2;
		bit2[n] = rand() % 2;
	}
}

void convolution(vector<u32> &bit, vector<u32> &encode_bit)
{
	u32 t_1 = 0, t_2 = 0, t_3 = 0, t_4 = 0, t_5 = 0, t_6 = 0;
	int count = 0;
	
	for (count = 0; count < BITN; count++)
	{
		encode_bit[count * 2] = bit[count] ^ t_2 ^ t_3 ^ t_5 ^ t_6;
		encode_bit[count * 2 + 1] = bit[count] ^ t_1 ^ t_2 ^ t_3 ^ t_6;
		// refresh t_1, t_2
		t_6 = t_5;
		t_5 = t_4;
		t_4 = t_3;
		t_3 = t_2;
		t_2 = t_1;
		t_1 = bit[count];
	}
}

void interleaver(vector<u32> &bit)
{
	vector<u32> temp(BITN * 2);
	int count1 = 0, count2 = 0, count3 = 0;
	
	for (count1 = 0; count1 < 32; count1++)
	{
		for (count2 = 0; count2 < 8; count2++)
		{
			temp[count3] = bit[count1 + count2 * 32];
			count3++;
		}
	}
	// copy result to original array
	for (count1 = 0; count1 < (BITN * 2); count1++)
	{
		bit[count1] = temp[count1];
	}
}

void transmitter(vector<u32> &bit1, vector<u32> &bit2, vector<Complex> &signal1, vector<Complex> &signal2)
{
	vector<int> encode_bit_1(BITN * 2);
	vector<int> encode_bit_2(BITN * 2);

	bit_generator(bit1, bit2);
	// antenna1
	OFDM_modulator(encode_bit_1, signal1);
	signal1[1] = {0.0, 0.0};
	// antenna2
	OFDM_modulator(encode_bit_2, signal2);
	signal2[0] = {0.0, 0.0};
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
		signal[n] = sym2sgnl[symbol];
	}
}

void OFDM_modulator(vector<u32> &bit, vector<Complex> &signal)
{
	vector<Complex> Z(SYMBOLN);
	Complex temp = {0.0, 0.0};
	int n = 0, k = 0, i = 0;
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
		signal[GI + k] = temp * ((double)1 / sqrt(SYMBOLN));
	}
	/* insert GI */
	for (i = 0; i < GI; i++)
	{
		signal[i] = signal[SYMBOLN + i];
	}
}