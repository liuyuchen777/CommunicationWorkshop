#include "const.h"

void ber(int loop, int (*tbit), int (*rbit), FILE *fp, double CNR)
{
	int n, error = 0;
	double Pe = 0.0;
	static double AverageBER = 0.0;

	for (n = 0; n <  SYMBOLN; n++)
	{
		if (tbit[n] != rbit[n])
			error++;
	}

	Pe = (double)error;
	AverageBER += Pe;
	// printf("# %5d: Eb/N0 = %f, %e\t%e\n", loop, (CNR - 3.0), Pe, AverageBER);

	if(loop == LOOPN-1)
	{
		AverageBER /= (LOOPN * BITN);
		printf("Eb/N0 = %f, Average BER = %1.10f\n", (CNR), AverageBER);
		fprintf(fp, "%f\t%1.10f\n", (CNR), AverageBER);
		AverageBER = 0.0;
	}
}
