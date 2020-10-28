#include "../inc/awgn.h"

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
	int transmitted_bit[BITN], received_bit[BITN];
	Complex transmitted_signal[SYMBOLN], received_signal[SYMBOLN];
	FILE *fp = NULL;
	double CNR = 0.0;

	srand((unsigned)time(NULL));

	if (!(fp = fopen(FILENAME, "a+")))
	{
		printf("[Error] File Open Fail!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* run record parameter */
		fprintf(fp, "[%s] LOOPN = %d, total symbol number is %d, SNR from %d~%d dB, ", __TIME__, LOOPN, SYMBOLN, SNR_START, SNR_STOP);
#if COHERENT == ON
			fprintf(fp, "coherent reception, ");
#else
			fprintf(fp, "non-coherent reception, ");
#endif
#if PHASE_SHIFT == ON
			fprintf(fp, "with phase shift.\n");
#else
			fprintf(fp, "without phase shift.\n");
#endif
	}

	for(Eb_N0 = SNR_START; Eb_N0 <= SNR_STOP; Eb_N0++)	/* SNR from 0-11 dB */
	{
		CNR = (double)Eb_N0 + 3.0;	/* QPSK provide 3dB improvement */

		for(loop = 0; loop < LOOPN; loop++) 
		{
			transmitter(transmitted_bit, transmitted_signal);
			awgn(transmitted_signal, received_signal, CNR);
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
	double val = 0.0001;
	printf("val = %e", val);

	return 0;
}
#endif