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

Complex Gaussian_generator(double sigma2)
{
	double r1, r2;	/* uniform distribution */
	double gaussian = 0.0;
	Complex temp = {0.0, 0.0};

	r1 = (double)rand() / RAND_MAX;
	r2 = (double)rand() / RAND_MAX;
	gaussian = sqrt(-sigma2 * log(r1));
	temp.real = gaussian * cos(2.0 * PI * r2);
	temp.image = gaussian * sin(2.0 * PI * r2);

	return temp;
}

void Rayleigh(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* for calculate channel state information */
	Complex An[WAVES];
	double Phin[WAVES];
	int count = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	/* calculate channel state information */
	for (count = 0; count < WAVES; count++)
	{
		An[count] = Gaussian_generator(0.125);
		Phin[count] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	h[0].real = 0.0;
	h[0].image = 0.0;
	for (count = 0; count < WAVES; count++)
	{
		h[0] = complex_add(h[0], complex_multiply(An[count], Exp(Phin[count])));
	}
	/* multiply to channel */
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] = complex_multiply(h[0], input_signal[count]);
	}
	/* add AWGN  */
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] = complex_add(output_signal[count], Gaussian_generator(sigma2));
	}
	// printf("channel.real = %f, channel.image = %f\n", h[0].real, h[0].image);
}

void select_channel(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* define val */
	Complex An[WAVES];
	double Phin[WAVES];
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	/* generate h */
	for (count1 = 0; count1 < PATH_NUMBER; count1++)
	{
		for (count2 = 0; count2 < WAVES; count2++)
		{
			An[count2] = Gaussian_generator(0.0625);
			Phin[count2] = ((double)rand()/RAND_MAX) * (2 * PI);
		}
		h[count1].real = 0.0;
		h[count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			h[count1] = complex_add(h[count1], complex_multiply(An[count2], Exp(Phin[count2])));
		}
	}
	/* handle delay */
	for (count1 = 0; count1 < DELAY; count1++)
	{
		output_signal[count1] = complex_multiply(h[0], input_signal[count1]);
	}
	/* multiply to channel */	
	for (count1 = DELAY; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] = complex_add(complex_multiply(h[0], input_signal[count1]),
										complex_multiply(h[1], input_signal[count1 - DELAY]));
	}
	/* add gaussian noise*/
	for (count1 = 0; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] = complex_add(output_signal[count1], Gaussian_generator(sigma2));
	}
}

void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double sigma2 = pow(10, (-CNR) / 10);

	for (i = 0; i < (SYMBOLN + GI); i++)
	{
		output_signal[i] = complex_add(input_signal[i], Gaussian_generator(sigma2));
	}
}