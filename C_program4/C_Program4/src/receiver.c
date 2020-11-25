#include "../inc/const.h"

#define LARGE_NUM	(6553600.0)

extern Complex channel_state_0[(GROUP * SYMBOLN)];
extern Complex channel_state_1[(GROUP * SYMBOLN)];

extern const double sym2sgnl1[4][2];
extern const double sym2sgnl2[4][2];

void receiver(Complex *signal, int *bit)
{
#if (RECEIVER == COHERENT)
	/* coherent demodulation */
	coherent_demodulator(signal, bit);
#elif (RECEIVER == NON_COHERENT)
	/* noncoherent demodulation */
	non_coherent_demodulator(signal, bit);
#elif (RECEIVER == OFDM)
	OFDM_demodulator(signal, bit);
#endif
}

int MLE1(Complex *symbol)
{
	int decision = 0;
	int i = 0;
	double current = 0.0;
	double minimun = LARGE_NUM;
	/* Maximun Likelihood Estimator */
	for (i = 0; i < SYMBOL; i++)
	{
		current = pow(symbol->real- sym2sgnl1[i][0], 2.0) + pow(symbol->image - sym2sgnl1[i][1], 2.0);
		if (current <= minimun)
		{
			minimun = current;
			decision = i;
		}
	}

	return decision;
}

int MLE2(Complex *symbol)
{
	int decision = 0;
	int i = 0;
	double current = 0.0;
	double minimun = LARGE_NUM;
	/* Maximun Likelihood Estimator */
	for (i = 0; i < SYMBOL; i++)
	{
		current = pow(symbol->real- sym2sgnl2[i][0], 2.0) + pow(symbol->image - sym2sgnl2[i][1], 2.0);
		if (current <= minimun)
		{
			minimun = current;
			decision = i;
		}
	}

	return decision;
}

void coherent_demodulator(Complex *signal, int *bit)
{
	int i = 0, j = 0;
	Complex temp = {0.0, 0.0};
	double norm = 0.0;
	/* channel estimation */
	for (i = 0; i < GROUP; i++)
	{
		for (j = 0; j < SYMBOLN; j++)
		{
			norm = pow(channel_state_0[j].real, 2.0) + pow(channel_state_0[j].image, 2.0);
			temp.real = (signal[i * SYMBOLN + j].real * channel_state_0[j].real 
							+ signal[i * SYMBOLN + j].image * channel_state_0[j].image) / norm;
			temp.image = (signal[i * SYMBOLN + j].image * channel_state_0[j].real 
							- signal[i * SYMBOLN + j].real * channel_state_0[j].image) / norm;
			signal[i * SYMBOLN + j].real = temp.real;
			signal[i * SYMBOLN + j].image = temp.image;
		}
	}
	/* caculate which point is nearest */
	for (i = 0; i < (GROUP * SYMBOLN); i++) 
	{
		switch (MLE1(&(signal[i])))
		{
			case 0: bit[i * 2] = 0; bit[i * 2 + 1] = 0; break;
			case 1: bit[i * 2] = 0; bit[i * 2 + 1] = 1; break;
			case 2: bit[i * 2] = 1; bit[i * 2 + 1] = 1; break;
			case 3: bit[i * 2] = 1; bit[i * 2 + 1] = 0; break;
		}
	}
}

void non_coherent_demodulator(Complex *signal, int *bit)
{
	/* need combine with previous symbol */
	int i = 0, j = 0;
	Complex previous_symbol;
	int diff_symbol = 0;
	Complex temp;
	double norm = 0.0;
	
	previous_symbol.real = 1.0;
	previous_symbol.image = 0.0;
	/* channel estimation */
	for (i = 0; i < GROUP; i++)
	{
		for (j = 0; j < SYMBOLN; j++)
		{
			norm = pow(channel_state_0[j].real, 2.0) + pow(channel_state_0[j].image, 2.0);
			temp.real = (signal[i * SYMBOLN + j].real * channel_state_0[j].real 
							+ signal[i * SYMBOLN + j].image * channel_state_0[j].image) / norm;
			temp.image = (signal[i * SYMBOLN + j].image * channel_state_0[j].real 
							- signal[i * SYMBOLN + j].real * channel_state_0[j].image) / norm;
			signal[i * SYMBOLN + j].real = temp.real;
			signal[i * SYMBOLN + j].image = temp.image;
		}
	}

	for (i = 0; i < (GROUP * SYMBOLN); i++)
	{
		/* dufferential */
		temp.real = signal[i].real * previous_symbol.real + signal[i].image * previous_symbol.image;
		temp.image = signal[i].image * previous_symbol.real - signal[i].real * previous_symbol.image;
		diff_symbol = MLE2(&(temp));
		switch (diff_symbol)
		{
			case 0: bit[i * 2] = 0; bit[i * 2 + 1] = 0; break;
			case 1: bit[i * 2] = 0; bit[i * 2 + 1] = 1; break;
			case 2: bit[i * 2] = 1; bit[i * 2 + 1] = 1; break;
			case 3: bit[i * 2] = 1; bit[i * 2 + 1] = 0; break;
		}
		previous_symbol.real = signal[i].real;
		previous_symbol.image = signal[i].image;
	}
}

void OFDM_demodulator(Complex *signal, int *bit)
{

}