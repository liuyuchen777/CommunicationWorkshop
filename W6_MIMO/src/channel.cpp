#include "const.h"

void Rayleigh(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR);
void awgn(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR);
void select_channel(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR);

void channel(vector<Complex> &input_signal1, vector<Complex> &input_signal2, 
				vector<Complex> &output_signal1, vector<Complex> &output_signal2, f32 CNR)
{
	if (strcmp(CHANNEL, "RAYLEIGH") == 0)
	{
		Rayleigh(input_signal1, input_signal2, output_signal1, CNR);
		Rayleigh(input_signal1, input_signal2, output_signal2, CNR);
	}
	else if (strcmp(CHANNEL, "AWGN") == 0)
	{
		awgn(input_signal1, input_signal2, output_signal1, CNR);
		awgn(input_signal1, input_signal2, output_signal2, CNR);
	}
	else if (strcmp(CHANNEL, "SELECT") == 0)
	{
		select_channel(input_signal1, input_signal2, output_signal1, CNR);
		select_channel(input_signal1, input_signal2, output_signal2, CNR);
	}
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

void Rayleigh(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR)
{
	// for calculate channel state information
	Complex An[WAVES];
	Complex H[2];
	double 	Phin[WAVES];
	int count = 0;
	double sigma2 = 2 * pow(10, (-CNR) / 10);
	// the first path
	for (count = 0; count < WAVES; count++)
	{
		An[count] = Gaussian_generator(0.125);
		Phin[count] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	H[0] = {0.0, 0.0};
	for (count = 0; count < WAVES; count++)
	{
		H[0] += An[count] * Exp(Phin[count]);
	}
	// the second path
	for (count = 0; count < WAVES; count++)
	{
		An[count] = Gaussian_generator(0.125);
		Phin[count] = ((double)rand()/RAND_MAX) * (2 * PI);
	}
	H[1] = {0.0, 0.0};
	for (count = 0; count < WAVES; count++)
	{
		H[1] += An[count] * Exp(Phin[count]);
	}
	// multiply to channel
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] = H[0] * input_signal1[count] + H[1] * input_signal2[count];
	}
	// add AWGN 
	for (count = 0; count < (SYMBOLN + GI); count++)
	{
		output_signal[count] += Gaussian_generator(sigma2);
	}
}

void select_channel(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR)
{
	// define val
	Complex An[WAVES];
	double 	Phin[WAVES];
	Complex H[2][2];
	int count1 = 0, count2 = 0, count3 = 0;
	double sigma2 = 2 * pow(10, (-CNR) / 10);
	// generate H
	for (count3 = 0; count3 < 2; count3++)
	{
		for (count1 = 0; count1 < 2; count1++)
		{
			for (count2 = 0; count2 < WAVES; count2++)
			{
				An[count2] = Gaussian_generator(0.0625);
				Phin[count2] = ((double)rand()/RAND_MAX) * (2 * PI);
			}
			H[count1][count3] = {0.0, 0.0};
			for (count2 = 0; count2 < WAVES; count2++)
			{
				H[count1][count3] +=  An[count2] * Exp(Phin[count2]);
			}
		}
	}
	// multiply to channel
	for (count1 = 0; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] = H[0][0] * input_signal1[count1] + H[0][1] * input_signal1[count1]
								+ H[1][0] * input_signal2[count1] + H[1][1] * input_signal2[count1];
	}

	// add gaussian noise
	for (count1 = 0; count1 < (SYMBOLN + GI); count1++)
	{
		output_signal[count1] += Gaussian_generator(sigma2);
	}
}

void awgn(vector<Complex> &input_signal1, vector<Complex> &input_signal2, vector<Complex> &output_signal, f32 CNR)
{
	int i = 0;
	double sigma2 = 2 * pow(10, (-CNR) / 10);

	for (i = 0; i < (SYMBOLN + GI); i++)
	{
		output_signal[i] = input_signal1[i] + input_signal2[i] + Gaussian_generator(sigma2);
		// output_signal[i] = input_signal[i];
	}
}