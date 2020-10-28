#ifndef __CONST_H_
#define __CONST_H_

#include <stdio.h>
#include <math.h>
#include <time.h>				
#include <stdlib.h>				

/*---------------------------------------------------------*/
/* Parameters                                              */
/*---------------------------------------------------------*/
#define sqr(x)				((x)*(x))
#define PI					3.141592654	/* acos(-1.0) */
#define OneBySqrt2			0.707106781	/* 1.0/sqrt(2.0) */
//#define SYMBOL_RATE			42.0e3
//#define Ts					(1.0 / SYMBOL_RATE)
#define SNR_START			(0)
#define SNR_STOP			(11)
#define SYMBOLN				64
#define BITN				(SYMBOLN*2)
#define LOOPN				100000		/* 100 will occur zero error at SNR = 9, 10, 11 */
#define FILENAME			"awgn.dat"
#define SYMBOL				(4)
#define GRAY				(0) // 1 with gray; 0 without gray

/*---------------------------------------------------------*/
/* Debug_Function                                          */
/*---------------------------------------------------------*/
#define TT_LEVEL			(0)
#if TT_LEVEL > 0
	#define TT_PRINT(format, arg) do {\
				printf("[%s: %d] "format"\n", __FUNCTION__, __LINE__, arg);\
			} while(0)
#else
	#define TT_PRINT(format, arg)
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
void transmitter(int (*bit), Complex *signal);
void bit_generator(int (*bit));
void QPSK_modulator(int (*bit), Complex *signal);
void awgn(Complex *input_signal, Complex *output_signal, double CNR);
void receiver(Complex *signal, int (*bit));
void QPSK_demodulator(Complex *signal, int (*bit));
void ber(int loop, int (*tbit), int (*rbit), FILE *fp, double CNR);
/*---------------------------------------------------------*/

#endif