#include "const.h"

Complex h[PATH_NUMBER];

void Rayleigh(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR);
void awgn(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR);
void select_channel(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR);

void channel(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR)
{
	if (strcmp(CHANNEL, "RAYLEIGH") == 0)
		Rayleigh(input_signal, output_signal, CNR);
	else if (strcmp(CHANNEL, "AWGN") == 0)
		awgn(input_signal, output_signal, CNR);
	else if (strcmp(CHANNEL, "SELECT") == 0)
		select_channel(input_signal, output_signal, CNR);
}

Complex Gaussian_generator(f32 sigma2)
{
	double r1, r2;	/* uniform distribution */
	double gaussian = 0.0;
	Complex temp = {0.0, 0.0};

	r1 = (double)rand() / RAND_MAX;
	r2 = (double)rand() / RAND_MAX;
	gaussian = sqrt(-sigma2 * log(r1));
	temp = {gaussian * cos(2.0 * PI * r2), gaussian * sin(2.0 * PI * r2)};

	return temp;
}

void Rayleigh(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR)
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
	h[0] = {0.0, 0.0};
	for (count = 0; count < WAVES; count++)
	{
		h[0] += An[count] * Exp(Phin[count]);
	}
	/* multiply to channel */
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] = h[0] * input_signal[count];
	}
	/* add AWGN  */
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] += Gaussian_generator(sigma2);
	}
	// printf("channel.real = %f, channel.image = %f\n", h[0].real, h[0].image);
}

void select_channel(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR)
{
	/* define val */
	Complex An[WAVES];
	double Phin[WAVES];
	int count1 = 0, count2 = 0;
	double sigma2 = pow(10, (-CNR) / 10);
	/* generate h */
	for (count1 = 0; count1 < 2; count1++)
	{
		for (count2 = 0; count2 < WAVES; count2++)
		{
			An[count2] = Gaussian_generator(0.0625);
			Phin[count2] = ((double)rand()/RAND_MAX) * (2 * PI);
		}
		h[count1] = {0.0, 0.0};
		for (count2 = 0; count2 < WAVES; count2++)
		{
			h[count1] +=  An[count2] * Exp(Phin[count2]);
		}
	}
	/* handle delay */
	for (count1 = 0; count1 < DELAY; count1++)
	{
		output_signal[count1] = h[0] * input_signal[count1];
	}
	/* multiply to channel */	
	for (count1 = DELAY; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] = h[0] * input_signal[count1] + h[1] * input_signal[count1 - DELAY];
	}
	/* add gaussian noise*/
	for (count1 = 0; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] += Gaussian_generator(sigma2);
	}
}

void awgn(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR)
{
	int i = 0;
	double sigma2 = 2 * pow(10, (-CNR) / 10);

	for (i = 0; i < (SYMBOLN + GI); i++)
	{
		output_signal[i] = input_signal[i] + Gaussian_generator(sigma2);
		// output_signal[i] = input_signal[i];
	}
}