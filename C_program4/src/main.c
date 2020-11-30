#include "const.h"

/* #define TEMP */

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
int main(int argc, char *argv[])
{
	int loop, Eb_N0;
	/* define transmission bit and signal */
	int transmitted_bit[BITN], 
		received_bit[BITN];
	Complex transmitted_signal[SYMBOLN + GI], 
			received_signal[SYMBOLN + GI];
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
		printf("[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, fd = %d, ", 
					__TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP, Fd);
		fprintf(fp, "[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, fd = %d, ", 
					__TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP, Fd);
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
		if (argc >= 2)
		{
			printf("[Note] %s\n", argv[1]);
			fprintf(fp, "[Note] %s\n", argv[1]);
		}
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
/* do some quick test */
int main()
{
	double x = 2.0;
	x += 1.0 - 2.0;

	printf("x = %f\n", x);
}
#endif