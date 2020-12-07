#include "const.h"

// #define TEMP // for temp test something
const Complex sym2sgnl1[SYMBOL] = {
		{ OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2, OneBySqrt2},
		{-OneBySqrt2,-OneBySqrt2},
		{ OneBySqrt2,-OneBySqrt2}
};

const Complex sym2sgnl2[SYMBOL] = {
		{1, 0},
		{0, 1},
		{-1, 0},
		{0, -1}
};

#ifndef TEMP
int main(int argc, char *argv[])
{
	u32 loop, Eb_N0;
	/* define transmission bit and signal */
	vector<u32> transmitted_bit(BITN);
	vector<u32> received_bit(BITN);
	vector<Complex> transmitted_signal(SYMBOLN + GI);
	vector<Complex> received_signal(SYMBOLN + GI);
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
			<< ", SNR from " << SNR_START << " ~ " << SNR_STOP << " dB, delay = " << DELAY << ", " 
			<< CHANNEL << ", " << RECEIVER << "." << endl;
		fp << "[" << __TIME__ << "] LOOPN = " << LOOPN << ", total symbol number is "<< SYMBOLN 
			<< ", SNR from " << SNR_START << " ~ " << SNR_STOP << " dB, delay = " << DELAY << ", " 
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
			transmitter(transmitted_bit, transmitted_signal);
			channel(transmitted_signal, received_signal, CNR);
			receiver(received_signal, received_bit);
			ber(loop, transmitted_bit, received_bit, fp, CNR);
		}	
	}

	fp.close();
	return 0;
}
#else
int main()
{
	// test function of complex class
	Complex c1(6.0, 7.0);
	Complex c2;
	Complex c3(1.0, 3.0);

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

}
#endif