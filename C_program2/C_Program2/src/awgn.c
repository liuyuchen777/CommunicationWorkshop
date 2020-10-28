#include "../inc/awgn.h"

void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double r1, r2;	/* uniform distribution */
	double sigma2 = pow(10, (-CNR) / 10);
	double gaussian = 0.0;
	double rand_phase = 0.0;
	
	/* add phase shift */
#if PAHSE_SHIFT == ON
	for (i = 0; i < SYMBOLN; i++)
	{
		rand_phase = (double)rand() / RAND_MAX * 2 * PI;
		output_signal[i].real = input_signal[i].real * cos(rand_phase) - input_signal[i].image * sin(rand_phase);
		output_signal[i].real = input_signal[i].real * sin(rand_phase) + input_signal[i].image * cos(rand_phase);
	}
#endif

	/* add random noise */
	for (i = 0; i < SYMBOLN; i++)
	{
		r1 = (double)rand() / RAND_MAX;
		if (r1 <= 1.0e-10)
			r1 = 1.0e-10;
		r2 = (double)rand() / RAND_MAX;
		if (r2 <= 1.0e-10)
			r2 = 1.0e-10; 
		gaussian = sqrt(-sigma2 * log(r1));
		
#if PHASE_SHIFT == ON
		output_signal[i].real = output_signal[i].real + gaussian * cos(2.0 * PI * r2);
		output_signal[i].image = output_signal[i].image + gaussian * sin(2.0 * PI * r2);
#else
		output_signal[i].real = input_signal[i].real + gaussian * cos(2.0 * PI * r2);
		output_signal[i].image = input_signal[i].image + gaussian * sin(2.0 * PI * r2);
#endif
	}
}