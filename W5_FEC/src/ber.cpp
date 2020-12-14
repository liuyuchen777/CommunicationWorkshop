#include "const.h"

void ber(u32 loop, vector<u32> &tbit, vector<u32> &rbit, fstream &fp, f32 CNR)
{
	int n, error = 0;
	static double error_count = 0.0;

	for (n = 0; n <  (TRANSMIT_BIT); n++)
	{
		if (tbit[n] != rbit[n])
			error++;
	}

	error_count += (double)error;

	if(loop == LOOPN-1)
	{
		error_count /= (LOOPN * TRANSMIT_BIT);
		cout << "Eb/N0 = " << (CNR - 3.0) << ", Average BER = " << error_count << endl;
		fp << (CNR - 3.0) << "\t" << error_count << endl;
		error_count = 0.0;
	}
}
