#include "const.h"

Complex h[PATH_NUMBER];

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
	Complex An[WAVES];
	double Phin[WAVES];
	double Thetan[WAVES];
	int count = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	double another_sigma2 = 0.125;

	/* calculate channel state information */
	for (count = 0; count < WAVES; count++)
	{
		An[count].real = Gaussian_generator(another_sigma2);
		An[count].image = Gaussian_generator(another_sigma2);
		Phin[count] = ((double)rand()/RAND_MAX) * (2 * PI);
		Thetan[count] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	h[0].real = 0.0;
	h[0].image = 0.0;
	for (count = 0; count < WAVES; count++)
	{
		temp.real = An[count].real * cos(Phin[count]) - An[count].image * sin(Phin[count]);
		temp.image = An[count].image * cos(Phin[count]) + An[count].real * sin(Phin[count]);
		h[0].real += temp.real;
		h[0].image += temp.image;
	}
/* multiply to channel */
	for (count = 0; count < SYMBOLN; count++)
	{
		output_signal[count].real = h[0].real * input_signal[count].real 
					- h[0].image * input_signal[count].image;
		output_signal[count].image = h[0].real * input_signal[count].image 
					+ h[0].image * input_signal[count].real;
	}
	/* add AWGN  */
	for (count = 0; count < SYMBOLN; count++)
	{
		output_signal[count].real += Gaussian_generator(sigma2);
		output_signal[count].image += Gaussian_generator(sigma2);
	}
}

void select_channel(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* define val */
	double An[WAVES];
	double Phin[WAVES];
	double Thetan[WAVES];
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	double another_sigma2 = 0.0625;
	/* generate h */
	for (count1 = 0; count1 < PATH_NUMBER; count1++)
	{
		for (count2 = 0; count2 < WAVES; count2++)
		{
			An[count2] = Gaussian_generator(another_sigma2);
			Phin[count2] = ((double)rand()/RAND_MAX) * (2 * PI);
			Thetan[count2] = ((double)rand()/RAND_MAX) * (2 * PI);
		}
		h[count1].real = 0.0;
		h[count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			temp.real = An[count2] * cos(Phin[count2]);
			temp.image = An[count2] * sin(Phin[count2]);
			h[count1].real += temp.real;
			h[count1].image += temp.image;
		}
	}
	/* handle delay */
	for (count1 = 0; count1 < DELAY; count1++)
	{
		output_signal[count1].real = h[0].real * input_signal[count1].real 
					- h[0].image * input_signal[0].image;
		output_signal[count1].image = h[0].real * input_signal[count1].image 
					+ h[0].image * input_signal[count1].real;
	}
	/* multiply to channel */
	for (count1 = DELAY; count1 < SYMBOLN; count1++)
	{
		for (count2 = 0; count2 < PATH_NUMBER; count2++)
		{
			output_signal[count1].real += h[count2].real * input_signal[count1].real 
						- h[count2].image * input_signal[count1].image;
			output_signal[count1].image = h[count2].real * input_signal[count1].image 
						+ h[count2].image * input_signal[count1].real;
		}
	}
	/* add gaussian noise*/
	for (count1 = 0; count1 < SYMBOLN; count1++)
	{
		output_signal[count1].real += Gaussian_generator(sigma2);
		output_signal[count1].image += Gaussian_generator(sigma2);
	}
}

void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double sigma2 = pow(10, (-CNR) / 10);

	for (i = 0; i < (SYMBOLN + GI); i++)
	{
		output_signal[i].real = input_signal[i].real + Gaussian_generator(sigma2);
		output_signal[i].image = input_signal[i].image + Gaussian_generator(sigma2);
	}
}