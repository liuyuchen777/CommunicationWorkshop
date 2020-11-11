#include "../inc/awgn.h"

// #define TEMP

const double sym2sgnl1[4][2] = {
		{ OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2,-OneBySqrt2},
		{ OneBySqrt2,-OneBySqrt2}
};

const double sym2sgnl2[4][2] = {
		{1, 0},
		{0, 1},
		{-1, 0},
		{0, -1}
};

Complex channel_state[SYMBOLN];

#ifndef TEMP
int main(void)
{
	int loop, Eb_N0;
	int transmitted_bit[BITN], received_bit[BITN];
	Complex transmitted_signal[SYMBOLN], received_signal[SYMBOLN];
	FILE *fp = NULL;
	double CNR = 0.0;
	int count1 = 0;		
	int count2 = 0; /* use for count */
	/* for calculate channel state information */
	Complex temp = {0.0, 0.0};
	double An = 0.0;
	double Phin = 0.0;
	double Thetan = 0.0;
	double all_in_bracket = 0.0;

	srand((unsigned)time(NULL));

	if (!(fp = fopen(FILENAME, "a+")))
	{
		printf("[Error] File Open Fail!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* run record parameter */
		fprintf(fp, "[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, fd = %d, ", 
					__TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP, Fd);
#if COHERENT == ON
			fprintf(fp, "coherent reception, ");
#else
			fprintf(fp, "non-coherent reception, ");
#endif
#if CHANNEL == AWGN
			fprintf(fp, "awgn channel.\n");
#elif CHANNEL == RAYLEIGH
			fprintf(fp, "rayleigh fading channel.\n");
#elif CHANNEL == SELECT
			fprintf(fp, "selective fading channel.\n");
#endif
	}

	for(Eb_N0 = SNR_START; Eb_N0 <= SNR_STOP; Eb_N0++)	/* SNR from 0-11 dB */
	{
		CNR = (double)Eb_N0 + 3.0;	/* QPSK provide 3dB improvement */

#if CHANNEL == RAYLEIGH
		/* calculate channel state information */
		for (count1 = 0; count1 < SYMBOLN; count1++)
		{
			channel_state[count1].real = 0.0;
			channel_state[count1].image = 0.0;
			for (count2 = 0; count2 < WAVES; count2++)
			{
				An = Guassian_Generator(1 / 8);
				Phin = Guassian_Generator(1 / 8);
				Thetan = (rand() / RAND_MAX) * (2 * PI);
				all_in_bracket = 2 * PI * Fd * cos(Thetan) * count1 * Ts + Phin;
				temp.real = An * cos(all_in_bracket);
				temp.image = An * sin(all_in_bracket);
				channel_state[count1].real += temp.real;
				channel_state[count1].image += temp.image;
			}
		}
#endif
		/* main loop */
		for(loop = 0; loop < LOOPN; loop++) 
		{
			transmitter(transmitted_bit, transmitted_signal);
			channel(transmitted_signal, received_signal, CNR);
			receiver(received_signal, received_bit);
			ber(loop, transmitted_bit, received_bit, fp, CNR);
		}	
	}

	fclose(fp);
	return 0;
}

#else

int main()
{
	/* cos和sin是弧度制没问题  */
	printf("cos(pi) = %f\n", cos(3.1415926));
	printf("sin(pi) = %f\n", sin(3.1415926));
	while(1)
	{
		double rand_phase = rand_phase = ((double)rand() / RAND_MAX) * 2 * PI;;
		printf("rand_phase = %f\n", rand_phase);
		printf("cos(rand_phase) = %f\n", cos(rand_phase));
		printf("sin(rand_phase) = %f\n", sin(rand_phase));
		getchar();
	}
}
#endif