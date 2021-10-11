#include "../inc/awgn.h"

Complex channel_state_0[(GROUP*SYMBOLN)];
Complex channel_state_1[(GROUP*SYMBOLN)];

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

double Guassian_Generator(double sigma2)
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
	double all_in_bracket = 0.0;
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};

	/* calculate channel state information */
	for (count1 = 0; count1 < WAVES; count1++)
	{
		An[count1].real = Guassian_Generator(0.125);
		An[count1].image = Guassian_Generator(0.125);
		Phin[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
		Thetan[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		channel_state_0[count1].real = 0.0;
		channel_state_0[count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			all_in_bracket = 2 * PI * Fd * cos(Thetan[count2]) * count1 * Ts + Phin[count2];
			temp.real = An[count2].real * cos(all_in_bracket) - An[count2].image * sin(all_in_bracket);
			temp.image = An[count2].image * cos(all_in_bracket) + An[count2].real * sin(all_in_bracket);
			channel_state_0[count1].real += temp.real;
			channel_state_0[count1].image += temp.image;
		}
	}
	/* multiply to channel */
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real = channel_state_0[count1].real * input_signal[count1].real 
					- channel_state_0[count1].image * input_signal[count1].image;
		output_signal[count1].image = channel_state_0[count1].real * input_signal[count1].image 
					+ channel_state_0[count1].image * input_signal[count1].real;
	}
	/* add AWGN  */
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real += Guassian_Generator(sigma2);
		output_signal[count1].image += Guassian_Generator(sigma2);
	}
}

void select_channel(Complex *input_signal, Complex *output_signal, double CNR)
{
	/* define val */
	double An[WAVES];
	double Phin[WAVES];
	double Thetan[WAVES];
	double all_in_bracket = 0.0;
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	/* generate CSI0 */
	for (count1 = 0; count1 < WAVES; count1++)
	{
		An[count1] = Guassian_Generator(0.0625);
		Phin[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
		Thetan[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		channel_state_0[count1].real = 0.0;
		channel_state_0[count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			all_in_bracket = 2 * PI * Fd * cos(Thetan[count2]) * count1 * Ts + Phin[count2];
			temp.real = An[count2] * cos(all_in_bracket);
			temp.image = An[count2] * sin(all_in_bracket);
			channel_state_0[count1].real += temp.real;
			channel_state_0[count1].image += temp.image;
		}
	}
	/* generate CSI1 */
	for (count1 = 0; count1 < WAVES; count1++)
	{
		An[count1] = Guassian_Generator(0.0625);
		Phin[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
		Thetan[count1] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		channel_state_1[count1].real = 0.0;
		channel_state_1[count1].image = 0.0;
		for (count2 = 0; count2 < WAVES; count2++)
		{
			all_in_bracket = 2 * PI * Fd * cos(Thetan[count2]) * count1 * Ts + Phin[count2];
			temp.real = An[count2] * cos(all_in_bracket);
			temp.image = An[count2] * sin(all_in_bracket);
			channel_state_1[count1].real += temp.real;
			channel_state_1[count1].image += temp.image;
		}
	}
	/* multiple to channel */
	output_signal[0].real = channel_state_0[0].real * input_signal[0].real 
				- channel_state_0[0].image * input_signal[0].image;
	output_signal[0].image = channel_state_0[0].real * input_signal[0].image 
				+ channel_state_0[0].image * input_signal[0].real;
	for (count1 = 1; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real = channel_state_0[count1].real * input_signal[count1].real 
					- channel_state_0[count1].image * input_signal[count1].image
					+ channel_state_1[count1-1].real * input_signal[count1-1].real
					- channel_state_1[count1-1].image * input_signal[count1-1].image;
		output_signal[count1].image = channel_state_0[count1].real * input_signal[count1].image 
					+ channel_state_0[count1].image * input_signal[count1].real
					+ channel_state_1[count1-1].real * input_signal[count1-1].image
					+ channel_state_1[count1-1].image * input_signal[count1-1].real;
	}
	/* add gaussian noise*/
		for (count1 = 0; count1 < (SYMBOLN * GROUP); count1++)
	{
		output_signal[count1].real += Guassian_Generator(sigma2);
		output_signal[count1].image += Guassian_Generator(sigma2);
	}
}

void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double sigma2 = pow(10, (-CNR) / 10);

	for (i = 0; i < SYMBOLN; i++)
	{
		output_signal[i].real = input_signal[i].real + Guassian_Generator(sigma2);
		output_signal[i].image = input_signal[i].image + Guassian_Generator(sigma2);
	}
}