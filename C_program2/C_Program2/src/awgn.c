#include "../inc/awgn.h"

#define WAVES 8

void add_random_noise(Complex *input_signal, Complex *output_signal, double CNR);
void add_phase_shift(Complex *input_signal, Complex *output_signal, double rand_phase);

void channel(Complex *input_signal, Complex *output_signal, double CNR, double rand_phase)
{
#if PHASE_SHIFT == ON
	add_phase_shift(input_signal, input_signal, rand_phase);
	add_random_noise(input_signal, output_signal, CNR);
#else
	add_random_noise(input_signal, output_signal, CNR);
#endif
}

void add_random_noise(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double r1, r2;	/* uniform distribution */
	double sigma2 = pow(10, (-CNR) / 10);
	double gaussian = 0.0;

	for (i = 0; i < SYMBOLN; i++)
	{
		r1 = (double)rand() / RAND_MAX;
		if (r1 <= 1.0e-10)
			r1 = 1.0e-10;
		r2 = (double)rand() / RAND_MAX;
		if (r2 <= 1.0e-10)
			r2 = 1.0e-10; 
		gaussian = sqrt(-sigma2 * log(r1));

		output_signal[i].real = input_signal[i].real + gaussian * cos(2.0 * PI * r2);
		output_signal[i].image = input_signal[i].image + gaussian * sin(2.0 * PI * r2);
	}
}

void add_phase_shift(Complex *input_signal, Complex *output_signal, double rand_phase)
{
	int i = 0;
	Complex temp;
	for (i = 0; i < SYMBOLN; i++)
	{
		temp.real = input_signal[i].real * cos(rand_phase) - input_signal[i].image * sin(rand_phase);
		temp.image = input_signal[i].real * sin(rand_phase) + input_signal[i].image * cos(rand_phase);
		input_signal[i].real = temp.real;
		input_signal[i].image = temp.image;
	}
}