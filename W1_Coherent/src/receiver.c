#include "../inc/awgn.h"

#define LARGE_NUM	(6553600.0)

extern const double sym2sgnl[4][2];

void receiver(Complex *signal, int(*bit))
{
	QPSK_demodulator(signal, bit);
}

int MLE(Complex *symbol)
{
	int decision = 0;
	int i = 0;
	double current = 0.0;
	double minimun = LARGE_NUM;
	/* Maximun Likelihood Estimator */
	for (i = 0; i < SYMBOL; i++)
	{
		current = pow(symbol->real- sym2sgnl[i][0], 2.0) + pow(symbol->image - sym2sgnl[i][1], 2.0);
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
	if (symbol->real > 0)
	{
		if (symbol->image > 0)
		{
			return 0;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		if (symbol->image > 0)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
}

void QPSK_demodulator(Complex *signal, int(*bit))
{
	int decision_flag = 0;
	int i = 0;

	/* caculate which point is nearest */
	for (i = 0; i < SYMBOLN; i++) {
		switch (MLE2(&(signal[i])))
		{
#if GRAY > 0
			case 0: bit[i * 2] = 0; bit[i * 2 + 1] = 0; break;
			case 1: bit[i * 2] = 0; bit[i * 2 + 1] = 1; break;
			case 2: bit[i * 2] = 1; bit[i * 2 + 1] = 1; break;
			case 3: bit[i * 2] = 1; bit[i * 2 + 1] = 0; break;
#else
			case 0: bit[i * 2] = 0; bit[i * 2 + 1] = 0; break;
			case 1: bit[i * 2] = 0; bit[i * 2 + 1] = 1; break;
			case 2: bit[i * 2] = 1; bit[i * 2 + 1] = 0; break;
			case 3: bit[i * 2] = 1; bit[i * 2 + 1] = 1; break;
#endif
		}
	}
}