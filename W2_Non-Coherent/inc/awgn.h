/*
 * code for non-coherent demodulation 
 */
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
#define SNR_START			(0)
#define SNR_STOP			(11)
#define SYMBOLN				64
#define BITN				(SYMBOLN * 2)
#define LOOPN				100000		/* 100 will occur zero error at SNR = 9, 10, 11 */
#define FILENAME			"./data/non-coherent.dat"
#define SYMBOL				(4)

#define ON					1
#define OFF					0

#define PHASE_SHIFT			ON
#define COHERENT			OFF
/*---------------------------------------------------------*/
/* Debug_Function                                          */
/*---------------------------------------------------------*/
#define TT_LEVEL			(0)
#if TT_LEVEL > 0
	#define TT_PRINT(format, args...) do {\
				printf("[%s: %d] "format"\n", __FUNCTION__, __LINE__, ##args);\
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
void bit_generator(int *bit);
void DQPSK_modulator(int *bit, Complex *signal);
void QPSK_modulator(int *bit, Complex *signal);
void channel(Complex *input_signal, Complex *output_signal, double CNR, double rand_phase);
void receiver(Complex *signal, int *bit, double rand_phase);
void coherent_demodulator(Complex *signal, int *bit);
void non_coherent_demodulator(Complex *signal, int *bit, double rand_phase);
void ber(int loop, int *tbit, int *rbit, FILE *fp, double CNR);
/*---------------------------------------------------------*/

#endif