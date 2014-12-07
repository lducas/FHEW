#ifndef PARAM_H
#define PARAM_H

#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include <stdint.h>

const int n = 500;

const int N = 1024;
const int N2 = N/2+1;

const int K = 3;
const int K2 = 6;

const long int Q = (long int) 1 << 32;
const int q = 512;
const int q2 = 256;

typedef int32_t ZmodQ;
typedef uint32_t uZmodQ;
const ZmodQ v = (1 << 29) +1;       // Q/8 +1
const ZmodQ v_inverse = 3758096385; // 1/v mod Q

const ZmodQ vgprime[3] = {v, v<<11, v<<22};
const int g_bits[3] = {11, 11, 10};
const int g_bits_32[3] = {21, 21, 22};


const int KS_base = 25;
const int KS_exp = 7;
const ZmodQ KS_table[7] = {1,
			   25,
			   25*25,
			   25*25*25,
			   25*25*25*25,
			   25*25*25*25*25,
			   25*25*25*25*25*25};

const int BS_base = 23;
const int BS_exp = 2;
const int BS_table[2] = {1,23};

typedef ZmodQ Ring_ModQ[N];
typedef fftw_complex Ring_FFT[N2];

#endif
