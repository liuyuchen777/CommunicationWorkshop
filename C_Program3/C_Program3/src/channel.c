#include "../inc/awgn.h"

Complex channel_state[SYMBOLN];

void Rayleigh(Complex *input_signal, Complex *output_signal, double CNR);
void awgn(Complex *input_signal, Complex *output_signal, double CNR);

void channel(Complex *input_signal, Complex *output_signal, double CNR)
{
#if CHANNEL == RAYLEIGH
	Rayleigh(input_signal, output_signal, CNR);
#elif CHANNEL == AWGN
	awgn(input_signal, output_signal, CNR);
#elif CHANNEL == SELECT
#endif
}

double Guassian_Generator(double sigma2)
{
	double r1, r2;	/* uniform distribution */
	double gaussian = 0.0;

	r1 = (double)rand() / RAND_MAX;
	if (r1 <= 1.0e-10)
		r1 = 1.0e-10;
	r2 = (double)rand() / RAND_MAX;
	if (r2 <= 1.0e-10)
		r2 = 1.0e-10; 
	gaussian = sqrt(-sigma2 * log(r1));

	return gaussian * cos(2.0 * PI * r2);
}

void Rayleigh(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	Complex temp = {0.0, 0.0};
	Complex noise_orign = {0.0, 0.0};
	Complex noise_after = {0.0, 0.0};

	for (i = 0; i < SYMBOLN; i++)
	{
		noise_orign.real = Guassian_Generator(sigma2);
		noise_orign.image = Guassian_Generator(sigma2);
		temp.real = channel_state[i].real / (pow(channel_state[i].real, 2) + pow(channel_state[i].image, 2));
		temp.image = -channel_state[i].image / (pow(channel_state[i].real, 2) + pow(channel_state[i].image, 2));
		noise_after.real = noise_orign.real * temp.real - noise_orign.image * temp.image;
		noise_after.image = noise_orign.real * temp.image + noise_orign.image * temp.real;

		output_signal[i].real = input_signal[i].real + noise_after.real;
		output_signal[i].image = input_signal[i].image + noise_after.image;
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