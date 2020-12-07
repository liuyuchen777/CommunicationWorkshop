#include "../inc/awgn.h"

void ber(int loop, int (*tbit), int (*rbit), FILE *fp, double CNR)
{
	int n, error = 0;
	double Pe = 0.0;
	static double AverageBER = 0.0;

	for (n = 0; n < BITN; n++)
	{
		if (tbit[n] != rbit[n])
			error++;
	}

	Pe = (double)error;
	AverageBER += Pe;
	// printf("# %5d: Eb/N0 = %f, %e\t%e\n", loop, (CNR - 3.0), Pe, AverageBER);

	if(loop == LOOPN-1)
	{
		TT_PRINT("total error in 100000 loop is %f", AverageBER);
		AverageBER /= (LOOPN * BITN);
		printf("Eb/N0 = %f, Average BER = %1.10f\n", (CNR - 3.0), AverageBER);
		fprintf(fp, "%f\t%1.10f\n", (CNR - 3.0), AverageBER);
		AverageBER = 0.0;
	}
}
