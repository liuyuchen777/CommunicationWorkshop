#include "const.h"

void ber(int loop, int (*tbit), int (*rbit), FILE *fp, double CNR)
{
	int n, error = 0;
	static double error_count = 0.0;

	for (n = 0; n <  BITN; n++)
	{
		if (tbit[n] != rbit[n])
			error++;
	}

	error_count += (double)error;

	if(loop == LOOPN-1)
	{
		error_count /= (LOOPN * BITN);
		printf("Eb/N0 = %f, Average BER = %1.10f\n", (CNR - 3.0), error_count);
		fprintf(fp, "%f\t%1.10f\n", (CNR - 3.0), error_count);
		error_count = 0.0;
	}
}
