#include "../inc/awgn.h"

/*
double rand_noise()
{
	double r1, r2;
	double sigma2 = pow(10, (-CNR) / 10);
	r1 = (double)rand() / RAND_MAX;
	r2 = (double)rand() / RAND_MAX;
	return sqrt(-sigma2 * log(r1)) * cos(2 * PI * r2);
}
*/
void awgn(Complex *input_signal, Complex *output_signal, double CNR)
{
	int i = 0;
	double r1, r2;	/* uniform distribution */
	double sigma2 = pow(10, (-CNR) / 10);
	double gaussian = 0.0;

	for (i = 0; i < SYMBOLN; i++)
	{
		r1 = (double)rand() / RAND_MAX;
		if (r1 <= 1.0e-10)
			r1 = 1.0e-10; //��ֹ����log0�Ĳ���
		r2 = (double)rand() / RAND_MAX;
		if (r2 <= 1.0e-10)
			r2 = 1.0e-10; //��ֹ����log0�Ĳ���
		gaussian = sqrt(-sigma2 * log(r1));

		output_signal[i].real = input_signal[i].real + gaussian * cos(2.0 * PI * r2);
		output_signal[i].image = input_signal[i].image + gaussian * sin(2.0 * PI * r2);
	}
}