#ifndef PARAM_H
#define PARAM_H

#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include <stdint.h>

const int32_t n = 500;

const int32_t N = 1024;
const int32_t N2 = N/2+1;

const int32_t K = 3;
const int32_t K2 = 6;

const int64_t Q = (int64_t) 1 << 32;
const int32_t q = 512;
const int32_t q2 = 256;

typedef int32_t ZmodQ;
typedef uint32_t uZmodQ;

const ZmodQ v = (1 << 29) +1;       // Q/8 +1
const ZmodQ v_inverse = -536870911; // 1/v mod Q

const ZmodQ vgprime[3] = {v, v<<11, v<<22};
const int32_t g_bits[3] = {11, 11, 10};
const int32_t g_bits_32[3] = {21, 21, 22};


const int32_t KS_base = 25;
const int32_t KS_exp = 7;
const ZmodQ KS_table[7] = {1,
			   25,
			   25*25,
			   25*25*25,
			   25*25*25*25,
			   25*25*25*25*25,
			   25*25*25*25*25*25};

const int32_t BS_base = 23;
const int32_t BS_exp = 2;
const int32_t BS_table[2] = {1,23};

typedef ZmodQ Ring_ModQ[N];
typedef fftw_complex Ring_FFT[N2];

enum BinGate {OR, AND , NOR, NAND};
const int32_t GateConst[4] = {15*q/8, 9*q/8, 11*q/8, 13*q/8};


#endif
