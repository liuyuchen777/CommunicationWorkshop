#include "const.h"

#define LARGE_NUM	(6553600.0)

void receiver(vector<Complex> &signal, vector<u32> &bit)
{
	OFDM_demodulator(signal, bit);
}

int MLE1(Complex &symbol)
{
	int decision = 0;
	int i = 0;
	double current = 0.0;
	double minimun = LARGE_NUM;
	/* Maximun Likelihood Estimator */
	for (i = 0; i < SYMBOL; i++)
	{
		current = distance(symbol, sym2sgnl1[i]);
		if (current <= minimun)
		{
			minimun = current;
			decision = i;
		}
	}

	return decision;
}

int MLE2(Complex &symbol)
{
	int decision = 0;
	int i = 0;
	double current = 0.0;
	double minimun = LARGE_NUM;
	/* Maximun Likelihood Estimator */
	for (i = 0; i < SYMBOL; i++)
	{
		current = distance(symbol, sym2sgnl2[i]);
		if (current <= minimun)
		{
			minimun = current;
			decision = i;
		}
	}

	return decision;
}

void coherent_demodulator(vector<Complex> &signal, vector<u32> &bit)
{

}

void non_coherent_demodulator(vector<Complex> &signal, vector<u32> &bit)
{

}

void OFDM_demodulator(vector<Complex> &signal, vector<u32> &bit)
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
		r[i] = signal[GI + i];
	}
	/* DFT, get Rm */
	for (m = 0; m < SYMBOLN; m++)
	{
		temp.set(0.0, 0.0);
		for (k = 0; k < SYMBOLN; k++)
		{
			temp += r[k] * Exp(-2 * PI * m * k / SYMBOLN);
		}
		R[m] = ((double)1 / sqrt(SYMBOLN)) * temp;
	}

	if (strcmp(CHANNEL, "AWGN") != 0)
	{
		/* generate H */
		for (m = 0; m < SYMBOLN; m++)
		{
			temp.set(0.0, 0.0);
			/* like DFT */
			temp += h[0];
			temp += h[1] * Exp(-2 * PI * m * DELAY / SYMBOLN);
			/* give value */
			H[m] = temp;
		}
		/* channel equalizer */
		for (m = 0; m < SYMBOLN; m++)
		{
			/* initialize */
			temp.set(0.0, 0.0);
			norm = H[m].norm();
			/* equalize */
			temp = R[m] * H[m].conj();
			R[m] = temp * (1 / norm);
		}
	}

	/* symbol desision */
	for (m = 0; m < SYMBOLN; m++) 
	{
		switch (MLE1(R[m]))
		{
			case 0: bit[m * 2] = 0; bit[m * 2 + 1] = 0; break;
			case 1: bit[m * 2] = 0; bit[m * 2 + 1] = 1; break;
			case 2: bit[m * 2] = 1; bit[m * 2 + 1] = 1; break;
			case 3: bit[m * 2] = 1; bit[m * 2 + 1] = 0; break;
		}
	}
}