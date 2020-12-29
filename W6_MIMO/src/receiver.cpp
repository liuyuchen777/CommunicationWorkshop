#include "const.h"

#define LARGE_NUM	(6553600.0)

void OFDM_demodulator(vector<Complex> &signal, vector<Complex> &demo_signal);

void MLD(vector<Complex> &signal1, vector<Complex> &signal2, vector<u32> &bit1, vector<u32> &bit2);

void receiver(vector<Complex> &signal1, vector<Complex> &signal2, vector<u32> &bit1, vector<u32> &bit2)
{
	vector<Complex> demodualte_signal1(SYMBOLN + 2);
	vector<Complex> demodualte_signal2(SYMBOLN + 2);

	OFDM_demodulator(signal1, demodualte_signal1);
	OFDM_demodulator(signal2, demodualte_signal2);

	MLD(demodualte_signal1, demodualte_signal2, bit1, bit2);
}

void MLD(vector<Complex> &signal1, vector<Complex> &signal2, 
			vector<u32> &bit1, vector<u32> &bit2)
{
	int i = 0;
	double min = 0.0;
	u32 bit_1_1 = 0, bit_1_2 = 0, bit_2_1 = 0, bit_2_2 = 0;
	u32 bit_1_1_record = 0, bit_1_2_record = 0, bit_2_1_record = 0, bit_2_2_record = 0;
	Complex H[2][2];
	double calculate = 0.0;
	int symbol1 = 0, symbol2 = 0;
	int bin2sym[2][2] = {	/* gray bit map */
		{ 0, 1},
		{ 3, 2}
	};
	// channel estimation
	H[0][0] = signal1[0].divide(sym2sgnl[0]);
	H[0][1] = signal1[1].divide(sym2sgnl[0]);
	H[1][0] = signal2[0].divide(sym2sgnl[0]);
	H[1][1] = signal2[1].divide(sym2sgnl[0]);
	// detection
	for (i = 2; i < (SYMBOLN + 2); i++)
	{
		min = 65536.0;
		for (bit_1_1 = 0; bit_1_1 < 2; bit_1_1++)
		{
			for (bit_1_2 = 0; bit_1_2 < 2; bit_1_2++)
			{
				for (bit_2_1 = 0; bit_2_1 < 2; bit_2_1++)
				{
					for (bit_2_2 = 0; bit_2_2 < 2; bit_2_2++)
					{
						symbol1 = bin2sym[bit_1_1][bit_1_2];
						symbol2 = bin2sym[bit_2_1][bit_2_2];
						calculate = (signal1[i] - (H[0][0] * sym2sgnl[symbol1] + H[0][1] * sym2sgnl[symbol2])).norm() 
									+ (signal2[i] - (H[1][0] * sym2sgnl[symbol1] + H[1][1] * sym2sgnl[symbol2])).norm();
						if (calculate < min)
						{
							min = calculate;
							bit_1_1_record = bit_1_1;
							bit_1_2_record = bit_1_2;
							bit_2_1_record = bit_2_1;
							bit_2_2_record = bit_2_2;
						}
					}
				}
			}
		}
		bit1[(i - 2) * 2] = bit_1_1_record;
		bit1[(i - 2) * 2 + 1] = bit_1_2_record;
		bit2[(i - 2) * 2] = bit_2_1_record;
		bit2[(i - 2) * 2 + 1] = bit_2_2_record;
	}
}

void OFDM_demodulator(vector<Complex> &signal, vector<Complex> &demo_signal)
{
	Complex r[SYMBOLN];		/* receiverd signal */
	vector<u32> rec_bit(BITN * 2);
	Complex temp = {0.0, 0.0};
	int m = 0, i = 0, k = 0;
	
	// remove pilot
	for (i = 0; i < SYMBOLN; i++)
	{
		r[i] = signal[2 + i];
	}
	/* DFT, get Rm */
	for (m = 0; m < SYMBOLN; m++)
	{
		temp.set(0.0, 0.0);
		for (k = 0; k < SYMBOLN; k++)
		{
			temp += r[k] * Exp(-2 * PI * m * k / SYMBOLN);
		}
		demo_signal[2 + m] = ((double)1 / sqrt(SYMBOLN)) * temp;
	}

	demo_signal[0] = signal[0];
	demo_signal[1] = signal[1];
}