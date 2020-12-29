#include "const.h"

int main(int argc, char *argv[])
{
	if ((argc == 2) && (strcmp(argv[1], "test") == 0))
	{
		// test code
		Complex c1(6.0, 7.0);
		Complex c2;
		Complex c3(1.0, 3.0);
		/*
		cout << c1 << endl;
		cout << c2 << endl;
		cout << c3 << endl;
		cout << c1 + c3 << endl;
		cout << c1 * c3 << endl;
		cout << c1 + c3.conj() << endl;
		cout << c1 * c1.conj() << endl;
		cout << c3 * 3 << endl;
		cout << 3 * c3 << endl;
		cout << 3.0 * c3 << endl;
		*/

		Complex c4(1.0, 2.0);
		Complex c5(1.0, 2.0);

		cout << c4.divide(c5) << endl;
	}
	else
	{
		u32 loop, Eb_N0;
		/* define transmission bit and signal */
		vector<u32> transmitted_bit_1(BITN);
		vector<u32> received_bit_1(BITN);
		vector<u32> transmitted_bit_2(BITN);
		vector<u32> received_bit_2(BITN);
		vector<Complex> transmitted_signal_1(SYMBOLN + 2);
		vector<Complex> transmitted_signal_2(SYMBOLN + 2);
		vector<Complex> received_signal_1(SYMBOLN + 2);
		vector<Complex> received_signal_2(SYMBOLN + 2);
		fstream fp;
		f32 CNR = 0.0;
		// rand seed
		srand((unsigned)time(NULL));
		// writing log
		fp.open(FILENAME, ios::app);
		if (!fp.is_open())
		{
			cout << "[Error] File Open Fail!" << endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			/* run record parameter */
			cout << "[" << __TIME__ << "] LOOPN = " << LOOPN << ", total symbol number is "<< SYMBOLN 
				<< ", SNR from " << SNR_START << " ~ " << SNR_STOP << " dB, "
				<< CHANNEL << ", " << RECEIVER << "." << endl;
			fp << "[" << __TIME__ << "] LOOPN = " << LOOPN << ", total symbol number is "<< SYMBOLN 
				<< ", SNR from " << SNR_START << " ~ " << SNR_STOP << " dB, "
				<< CHANNEL << ", " << RECEIVER << "." << endl;
			if (argc >= 2)
			{
				cout << "[Note] " << argv[1] << endl;
				fp << "[Note] " << argv[1] << endl;
			}
		}
		// main loop
		for(Eb_N0 = SNR_START; Eb_N0 <= SNR_STOP; Eb_N0++)	/* SNR from 0-11 dB */
		{
			CNR = (double)Eb_N0 + 3.0;	/* QPSK provide 3dB improvement */
			/* main loop */
			for(loop = 0; loop < LOOPN; loop++) 
			{
				transmitter(transmitted_bit_1, transmitted_bit_2, transmitted_signal_1, transmitted_signal_2);
				channel(transmitted_signal_1, transmitted_signal_2, received_signal_1, received_signal_2, CNR);
				receiver(received_signal_1, received_signal_2, received_bit_1, received_bit_2);
				ber(loop, transmitted_bit_1, transmitted_bit_2, received_bit_1, received_bit_2, fp, CNR);
			}	
		}

		fp.close();
	}
	return 0;
}