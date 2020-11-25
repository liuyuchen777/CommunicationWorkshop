#include "../inc/const.h"

//#define TEMP

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

#ifndef TEMP
int main(void)
{
	int loop, Eb_N0;
	int transmitted_bit[(GROUP * BITN)], 
		received_bit[(GROUP * BITN)];
	Complex transmitted_signal[(GROUP * SYMBOLN) + GI], 
			received_signal[(GROUP * SYMBOLN) + GI];
	FILE *fp = NULL;
	double CNR = 0.0;

	srand((unsigned)time(NULL));
	/* writing log */
	if (!(fp = fopen(FILENAME, "a+")))
	{
		printf("[Error] File Open Fail!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* run record parameter */
		printf("[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, fd = %d, GROUP = %d, ", 
					__TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP, Fd, GROUP);
		fprintf(fp, "[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, fd = %d, GROUP = %d, ", 
					__TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP, Fd, GROUP);
#if RECEIVER == COHERENT
			printf("QPSK / coherent reception, ");
			fprintf(fp, "QPSK / coherent reception, ");
#elif RECEIVER == NON_COHERENT
			printf("DQPSK / non-coherent reception, ");
			fprintf(fp, "DQPSK / non-coherent reception, ");
#elif RECEIVER == OFDM
			printf("OFDM, ");
			fprintf(fp, "OFDM, ");
#endif
#if CHANNEL == AWGN
			printf("awgn channel.\n");
			fprintf(fp, "awgn channel.\n");
#elif CHANNEL == RAYLEIGH
			printf("rayleigh fading channel.\n");
			fprintf(fp, "rayleigh fading channel.\n");
#elif CHANNEL == SELECT
			printf("selective fading channel.\n");
			fprintf(fp, "selective fading channel.\n");
#endif
	}
	/* main loop */
	for(Eb_N0 = SNR_START; Eb_N0 <= SNR_STOP; Eb_N0++)	/* SNR from 0-11 dB */
	{
		CNR = (double)Eb_N0 + 3.0;	/* QPSK provide 3dB improvement */
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
	double x = 2.0;
	printf("x ** 100 = %f", pow(x, 10000.0));
}
#endif