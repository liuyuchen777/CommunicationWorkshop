#include "../inc/awgn.h"

#define LARGE_NUM	(6553600.0)

extern const double sym2sgnl1[4][2];
extern const double sym2sgnl2[4][2];

void receiver(Complex *signal, int(*bit))
{
#if COHERENT == ON
	/* coherent demodulation */
	coherent_demodulator(signal, bit);
#else
	/* noncoherent demodulation */
	non_coherent_demodulator(signal, bit);
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

void coherent_demodulator(Complex *signal, int *bit)
{
	int decision_flag = 0;
	int i = 0;

	/* caculate which point is nearest */
	for (i = 0; i < SYMBOLN; i++) {
		switch (MLE1(&(signal[i])))
		{
			case 0: bit[i * 2] = 0; bit[i * 2 + 1] = 0; break;
			case 1: bit[i * 2] = 0; bit[i * 2 + 1] = 1; break;
			case 2: bit[i * 2] = 1; bit[i * 2 + 1] = 1; break;
			case 3: bit[i * 2] = 1; bit[i * 2 + 1] = 0; break;
		}
	}
}

void non_coherent_demodulator(Complex *signal, int*bit)
{
	/* need combine with previous symbol */
	int previous_symbol = 0;
	int i = 0;

	for (i = 0; i < SYMBOLN; i++)
	{

	}
}