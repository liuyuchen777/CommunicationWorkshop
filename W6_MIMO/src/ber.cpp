#include "const.h"

void ber(u32 loop, vector<u32> &tbit1, vector<u32> &tbit2, vector<u32> &rbit1, vector<u32> &rbit2, fstream &fp, f32 CNR)
{
	int n, error = 0;
	static double error_count = 0.0;

	for (n = 0; n <  BITN; n++)
	{
		if (tbit1[n] != rbit1[n])
			error++;
	}

	for (n = 0; n <  BITN; n++)
	{
		if (tbit2[n] != rbit2[n])
			error++;
	}

	error_count += (double)error;

	if(loop == LOOPN-1)
	{
		error_count /= (LOOPN * BITN * 2);
		cout << "Eb/N0 = " << (CNR - 3.0) << ", Average BER = " << error_count << endl;
		fp << (CNR - 3.0) << "\t" << error_count << endl;
		error_count = 0.0;
	}
}
