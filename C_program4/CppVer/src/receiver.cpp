#include "const.h"

#define LARGE_NUM	(6553600.0)

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

}

void non_coherent_demodulator(Complex *signal, int *bit)
{

}

void OFDM_demodulator(Complex *signal, int *bit)
{
	Complex r[SYMBOLN];		/* receiverd signal */
	Complex R[SYMBOLN];		/* after DFT */
	Complex H[SYMBOLN];		/* channel state information */
	Complex temp = {0.0, 0.0};
	int m = 0, i = 0, k = 0;
	double norm = 0.0;
	/* remove GI */
	for (i = 0; i < SYMBOLN; i++)
	{
		r[i].real = signal[GI + i].real;
		r[i].image = signal[GI + i].image;
	}
	/* DFT, get Rm */
	for (m = 0; m < SYMBOLN; m++)
	{
		temp.real = 0.0;
		temp.image = 0.0;
		for (k = 0; k < N; k++)
		{
			temp = complex_add(temp, complex_multiply(r[k], Exp(-2 * PI * m * k / SYMBOLN)));
		}
		R[m].real = 1 / sqrt(SYMBOLN) * temp.real;
		R[m].image = 1 / sqrt(SYMBOLN) * temp.image;
	}

#if CHANNEL != AWGN
	// printf("channel.real = %f, channel.image = %f\n", h[0].real, h[0].image);
	/* generate H */
	for (m = 0; m < SYMBOLN; m++)
	{
		temp.real = 0.0;
		temp.image = 0.0;
		/* like DFT */
		temp = complex_add(temp, h[0]);
		temp = complex_add(temp, complex_multiply(h[1], Exp(-2 * PI * m * DELAY / N)));
		/* give value */
		H[m].real = temp.real;
		H[m].image = temp.image;
	}
	/* channel equalizer */
	for (m = 0; m < SYMBOLN; m++)
	{
		/* initialize */
		temp.real = 0.0;
		temp.image = 0.0;
		norm = 0.0;
		/* equalize */
		norm = H[m].real * H[m].real + H[m].image * H[m].image;
		temp = conjugate_multiply(R[m], H[m]);
		R[m].real = temp.real / norm;
		R[m].image = temp.image / norm;
	}
#endif

	/* symbol desision */
	for (m = 0; m < SYMBOLN; m++) 
	{
		switch (MLE1(&(R[m])))
		{
			case 0: bit[m * 2] = 0; bit[m * 2 + 1] = 0; break;
			case 1: bit[m * 2] = 0; bit[m * 2 + 1] = 1; break;
			case 2: bit[m * 2] = 1; bit[m * 2 + 1] = 1; break;
			case 3: bit[m * 2] = 1; bit[m * 2 + 1] = 0; break;
		}
	}
}