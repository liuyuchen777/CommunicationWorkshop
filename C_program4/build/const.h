
#ifndef __CONST_H_
#define __CONST_H_

#define Tutorial_VERSION_MAJOR 
#define Tutorial_VERSION_MINOR 

#include <stdio.h>
#include <math.h>
#include <time.h>				
#include <stdlib.h>				

/*---------------------------------------------------------*/
/* Parameters                                              */
/*---------------------------------------------------------*/
#define PI					3.141592654				/* acos(-1.0) */
#define OneBySqrt2			0.707106781				/* 1.0/sqrt(2.0) */
#define SNR_START			(0)
#define SNR_STOP			(30)
#define SYMBOLN				(64)
#define BITN				(SYMBOLN * 2)
#define FILENAME			"../data/OFDM.dat"
#define SYMBOL				(4)						/* number of symbol */
#define Fd					(0)						/* doppler shift frequency */
#define WAVES				(8)						/* number of Rayleigh waves */
/* symbol time */
#define SYMBOL_RATE			(42.0e3)				/* symbol rate */
#define Ts					(1.0 / SYMBOL_RATE)		/* symbol time */
#define ONE_PATH			(1.0 / WAVES)
#define GI					(16)					/* guard interval length */
#define N					(64)					/* number of subcarrier */
/* loop time */
#define LOOPN				(10000)					/* total loop time */
#define DELAY_TIME			(0)
/* channel type */
#define AWGN				0
#define RAYLEIGH			1
#define SELECT				2
/* receiver type */
#define COHERENT			0
#define NON_COHERENT		1
#define OFDM				2
/* setting */
#define RECEIVER			OFDM					/* choose receiver type */
#define CHANNEL				AWGN					/* choose channel type */
#define PATH_NUMBER			1						/* only 1 and 2 */
#define DELAY				1
/*---------------------------------------------------------*/
/* Debug_Function                                          */
/*---------------------------------------------------------*/
#define TT_LEVEL			(0)
#if TT_LEVEL > 0
	#define TT_PRINT(format, args...) do {\
				printf("[%s: %d] "format"\n", __FUNCTION__, __LINE__, ##args);\
				getchar();\
			} while(0)
#else
	#define TT_PRINT(format, args...)
#endif
/*---------------------------------------------------------*/
/* Complex Number                                          */
/*---------------------------------------------------------*/
typedef struct ComplexNumber {
	double real;
	double image;
} Complex;
/*---------------------------------------------------------*/
/* Prototype                                               */
/*---------------------------------------------------------*/
void transmitter(int *bit, Complex *signal);
void receiver(Complex *signal, int *bit);
/* modulator */
void DQPSK_modulator(int *bit, Complex *signal);
void QPSK_modulator(int *bit, Complex *signal);
void OFDM_modulator(int *bit, Complex *signal);
/* channel */
void channel(Complex *input_signal, Complex *output_signal, double CNR);
/* demodulator */
void coherent_demodulator(Complex *signal, int *bit);
void non_coherent_demodulator(Complex *signal, int *bit);
void OFDM_demodulator(Complex *signal, int *bit);
/* support function */
void ber(int loop, int *tbit, int *rbit, FILE *fp, double CNR);
Complex Gaussian_generator(double sigma2);
void bit_generator(int *bit);
/* complex operate function */
Complex complex_add(Complex c1, Complex c2);
Complex complex_multiply(Complex c1, Complex c2);
Complex conjugate(Complex c);
Complex Exp(double input);
/*---------------------------------------------------------*/

#endif
