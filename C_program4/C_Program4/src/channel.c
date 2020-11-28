#include "const.h"

Complex h[PATH_NUMBER][(GROUP * SYMBOLN)];

void Rayleigh(Complex *input_signal, Complex *output_signal, double CNR);
void awgn(Complex *input_signal, Complex *output_signal, double CNR);
void select_channel(Complex *input_signal, Complex *output_signal, double CNR);

void channel(Complex *input_signal, Complex *output_signal, double CNR)
{
#if CHANNEL == RAYLEIGH
	Rayleigh(input_signal, output_signal, CNR);
#elif CHANNEL == AWGN
	awgn(input_signal, output_signal, CNR);
#elif CHANNEL == SELECT
	select_channel(input_signal, output_signal, CNR);
#endif
}

double Gaussian_generator(double sigma2)
{
	double r1, r2;	/* uniform distribution */
	double gaussian = 0.0;

	r1 = (double)rand() / RAND_MAX;
	r2 = (double)rand() / RAND_MAX;
	gaussian = sqrt(-sigma2 * log(r1));

	return gaussian * cos(2.0 * PI * r2);
}

void Rayleigh(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* for calculate channel state information */
	double An[WAVES];
	double Phin[WAVES];
	double Thetan[WAVES];
	double all_in_bracket = 0.0;
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	double another_sigma2 = ONE_PATH;

	/* calculate channel state information */
	for (count1 = 0; count1 < WAVES; count1++)
	{
		An[count1] = Gaussian_generator(another_sigma2);
		Phin[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
		Thetan[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		h[0][count1].real = 0.0;
		h[0][count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			all_in_bracket = 2 * PI * Fd * cos(Thetan[count2]) * count1 * Ts + Phin[count2];
			temp.real = An[count2] * cos(all_in_bracket);
			temp.image = An[count2] * sin(all_in_bracket);
			h[0][count1].real += temp.real;
			h[0][count1].image += temp.image;
		}
	}
	/* multiply to channel */
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real = h[0][count1].real * input_signal[count1].real 
					- h[0][count1].image * input_signal[count1].image;
		output_signal[count1].image = h[0][count1].real * input_signal[count1].image 
					+ h[0][count1].image * input_signal[count1].real;
	}
	/* add AWGN  */
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real += Gaussian_generator(sigma2);
		output_signal[count1].image += Gaussian_generator(sigma2);
	}
}

void select_channel(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* define val */
	double An[WAVES];
	double Phin[WAVES];
	double Thetan[WAVES];
	double all_in_bracket = 0.0;
	int count1 = 0, count2 = 0, count3 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	double another_sigma2 = ONE_PATH / 2;
	/* generate h */
	for (count3 = 0; count3 < PATH_NUMBER; count3++)
	{
		for (count1 = 0; count1 < WAVES; count1++)
		{
			An[count1] = Gaussian_generator(another_sigma2);
			Phin[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
			Thetan[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
		}
		for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
		{
			h[count3][count1].real = 0.0;
			h[count3][count1].image = 0.0;
			for (count2 = 0; count2 < WAVES; count2++)
			{
				all_in_bracket = 2 * PI * Fd * cos(Thetan[count2]) * count1 * Ts + Phin[count2];
				temp.real = An[count2] * cos(all_in_bracket);
				temp.image = An[count2] * sin(all_in_bracket);
				h[count3][count1].real += temp.real;
				h[count3][count1].image += temp.image;
			}
		}
	}
	/* handle delay */
	for (count1 = 0; count1 < DELAY; count1++)
	{
		output_signal[count1].real = h[0][count1].real * input_signal[count1].real 
					- h[0][count1].image * input_signal[0].image;
		output_signal[count1].image = h[0][count1].real * input_signal[count1].image 
					+ h[0][count1].image * input_signal[count1].real;
	}
	/* multiply to channel */
	for (count1 = DELAY; count1 < (SYMBOLN * GROUP); count1++)
	{
		for (count2 = 0; count2 < PATH_NUMBER; count2++)
		{
			output_signal[count1].real += h[count2][count1].real * input_signal[count1].real 
						- h[count2][count1].image * input_signal[count1].image;
			output_signal[count1].image = h[count2][count1].real * input_signal[count1].image 
						+ h[count2][count1].image * input_signal[count1].real;
		}
	}
	/* add gaussian noise*/
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real += Gaussian_generator(sigma2);
		output_signal[count1].image += Gaussian_generator(sigma2);
	}
}

void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double sigma2 = pow(10, (-CNR) / 10);

	/* decide channel */
	h[0][0].real = 1.0;
	h[0][0].image = 0.0;

	for (i = 0; i < (GROUP * SYMBOLN + GI); i++)
	{
		output_signal[i].real = input_signal[i].real + Gaussian_generator(sigma2);
		output_signal[i].image = input_signal[i].image + Gaussian_generator(sigma2);
	}
}