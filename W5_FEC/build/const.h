
#ifndef __CONST_H_
#define __CONST_H_

/*---------------------------------------------------------*/
/* Cmake Version                                           */
/*---------------------------------------------------------*/
#define Tutorial_VERSION_MAJOR 
#define Tutorial_VERSION_MINOR 

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <fstream>
#include <cstdlib>
#include <string.h>				

using namespace std;

/*---------------------------------------------------------*/
/* Typedef                                                 */
/*---------------------------------------------------------*/
typedef int u32;
typedef double f32;

/*---------------------------------------------------------*/
/* Parameters                                              */
/*---------------------------------------------------------*/
#define PI					3.141592654				// acos(-1.0)
#define OneBySqrt2			0.707106781				// 1.0/sqrt(2.0)
#define SNR_START			(17)
#define SNR_STOP			(30)
// bit length
#define SYMBOLN				(128)
#define BITN				(128)
#define TRANSMIT_BIT		(122)
#define CLEAN_BIT			(6)
#define FILENAME			"../data/FEC.dat"
#define SYMBOL				(4)						// number of symbol
#define WAVES				(8)						// number of Rayleigh waves
#define GI					(0)					// guard interval length
/* loop time */
#define LOOPN				(100000)					// total loop time
/* setting */
#define RECEIVER			"OFDM"					// "OFDM" / "COHERENT" / "NON_COHERENT"
#define CHANNEL				"RAYLEIGH"					// "AWGN" / "RAYLEIGH" / "SELECT"
#define PATH_NUMBER			1						// only support 1 and 2
#define DELAY				0

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
class Complex{
	private:
		f32 m_real;
		f32 m_image;
	public:
		// construct function
		Complex() : m_real(0.0), m_image(0.0) {}
		Complex(double r, double i) : m_real(r), m_image(i) {}
		Complex(const Complex &c);
		~Complex() {};
		// other function
		Complex conj(void);
		f32 real(void) const { return m_real; }
		f32 image(void) const { return m_image; }
		void set_real(f32 r) { m_real = r; }
		void set_image(f32 i) { m_image = i; }
		void set(f32 r, f32 i) { m_real = r; m_image = i; }
		void set(const Complex &c) { m_real = c.real(); m_image = c.image(); }
		f32 norm();
		// operator overload
		Complex & operator=(const Complex c);
		Complex operator+(const Complex c);
		Complex operator-(const Complex c);
		Complex & operator+=(const Complex c);
		Complex & operator-=(const Complex c);
		Complex operator*(const Complex c);
		Complex operator*(const u32 num);
		Complex operator*(const f32 num);
		// friend overload
		friend ostream & operator<<(ostream &os, const Complex &c);
		friend Complex operator*(u32 num, const Complex & c);
		friend Complex operator*(f32 num, const Complex & c);
		friend f32 distance(const Complex &c1, const Complex &c2);
};

/*---------------------------------------------------------*/
/* Prototype                                               */
/*---------------------------------------------------------*/
void transmitter(vector<u32> &bit, vector<Complex> &signal);
void receiver(vector<Complex> &signal, vector<u32> &bit);
/* modulator */
void DQPSK_modulator(vector<u32> &bit, vector<Complex> &signal);
void QPSK_modulator(vector<u32> &bit, vector<Complex> &signal);
void OFDM_modulator(vector<u32> &bit, vector<Complex> &signal);
/* channel */
void channel(vector<Complex> &input_signal, vector<Complex> &output_signal, f32 CNR);
/* demodulator */
void coherent_demodulator(vector<Complex> &signal, vector<u32> &bit);
void non_coherent_demodulator(vector<Complex> &signal, vector<u32> &bit);
void OFDM_demodulator(vector<Complex> &signal, vector<u32> &bit);
/* support function */
void ber(u32 loop, vector<u32> &tbit, vector<u32> &rbit, fstream &fp, f32 CNR);
Complex Gaussian_generator(f32 sigma2);
/* complex operate function */
Complex Exp(f32 input);

// global varible
extern const Complex sym2sgnl1[SYMBOL];
extern const Complex sym2sgnl2[SYMBOL];
extern Complex h[PATH_NUMBER];

#endif
