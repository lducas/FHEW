#include <iostream>
#include <cstdlib>
#include "FHEW.h"
#include "FFT.h"
#include <cassert>

using namespace std;
namespace FHEW {

  typedef Ring_ModQ ct_ModQ[K2][2];   // Ciphertext in coefficient form
  typedef Ring_ModQ ct_ModQ1[2];   // Entry C[1] of ct_ModQ
  typedef Ring_ModQ dct_ModQ[K2][K2]; // Decomposed Ciphertext in coeff form
  typedef Ring_ModQ dct_ModQ1[K2]; // Entry C[1] of dct_ModQ
  typedef Ring_FFT  dct_FFT[K2][K2];  // Decomposed Ciphertext in FFT form
  typedef Ring_FFT  dct_FFT1[K2];  // Entry C[1] of dct_FFT
  
  Ring_FFT t_TestMSB;
  
  void Setup() {
    FFTsetup();
    Ring_ModQ tmsb;
    tmsb[0]=-1;
    for (int i = 1; i < N; ++i) tmsb[i]=1;
    FFTforward(t_TestMSB, tmsb);
  }
  
  void FHEWencrypt(ct_FFT ct, Ring_FFT sk_FFT, int m) { // Used by BootstrappingKeyGen
    Ring_FFT ai;
    ct_ModQ res;
    int mm = (((m % q) + q) % q) * (2*N/q);             // Reduce mod q (dealing with negative number as well)
    int sign = 1;
    if (mm >= N) { mm -= N; sign = -1; }
    
    for (int i = 0; i < K2; ++i) {
      for (int k = 0; k < N; ++k) res[i][0][k] = rand(); // % Q
      FFTforward(ai, res[i][0]);
      for (int k = 0; k < N2; ++k) 
        ai[k] = ((double complex) ai[k]) * ((double complex) sk_FFT[k]);
      FFTbackward(res[i][1], ai);
      for (int k = 0; k < N; ++k) res[i][1][k] += Sample(Chi1);    // Add error [a,as+e]
    }
    for (int i = 0; i < K; ++i) {
      res[2*i  ][0][mm] += sign*vgprime[i]; // Add G Multiple
      res[2*i+1][1][mm] += sign*vgprime[i]; // [a,as+e] + X^m *G
    }
    for (int i = 0; i < K2; ++i)
      for (int j = 0; j < 2; ++j)
	FFTforward(ct[i][j], res[i][j]);
  }
  
  void KeyGen(EvalKey* EK, const LWE::SecretKey LWEsk) {
    LWE::SecretKeyN FHEWsk;
    LWE::KeyGenN(FHEWsk);
    LWE::SwitchingKeyGen(EK->KSkey, LWEsk, FHEWsk);

    Ring_FFT FHEWskFFT;
    FFTforward(FHEWskFFT, FHEWsk);
    for (int i = 0; i < n; ++i)
      for (int j = 1; j < BS_base; ++j)
        for (int k = 0; k < BS_exp; ++k) {
          EK->BSkey[i][j][k] = (ct_FFT*) fftw_malloc(sizeof(ct_FFT));
          FHEWencrypt( (*EK->BSkey[i][j][k]), FHEWskFFT, LWEsk[i] * j * BS_table[k] );
        }
  }

  void fwrite_ek(const EvalKey& EK, FILE* f) {
    // Write bootstrapping key
    for (int i = 0; i < n; ++i)      
      for (int j = 1; j < BS_base; ++j)
        for (int k = 0; k < BS_exp; ++k) 
          assert(fwrite(EK.BSkey[i][j][k], sizeof(ct_FFT), 1, f));
    // Write switching key
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < KS_base; ++j)
        for (int k = 0; k < KS_exp; ++k)
	  assert(fwrite(EK.KSkey[i][j][k], sizeof(LWE::CipherTextQ), 1, f));
  }

  EvalKey* fread_ek(FILE* f) {
    EvalKey* EK = new EvalKey;
    // Read bootstrapping key
    for (int i = 0; i < n; ++i)
      for (int j = 1; j < BS_base; ++j)
        for (int k = 0; k < BS_exp; ++k) {
          EK->BSkey[i][j][k] = (ct_FFT*) fftw_malloc(sizeof(ct_FFT));
          assert(fread(EK->BSkey[i][j][k], sizeof(ct_FFT), 1, f));
        }
    // Read switching key
    cerr << "BSKey Read. \n";
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < KS_base; ++j)
        for (int k = 0; k < KS_exp; ++k) {
	  EK->KSkey[i][j][k] = new LWE::CipherTextQ;
	  assert(fread(EK->KSkey[i][j][k], sizeof(LWE::CipherTextQ), 1, f));
	}
    return EK;
  }

  typedef Ring_FFT ct_FFT1[2]; // C[1] entry of ct_FFT ciphertext
  
  void AddToACC(ct_FFT1 ACC, ct_FFT C) {
    ct_ModQ1 ct;
    dct_ModQ1 dct;
    dct_FFT1 dctFFT;
    // Decompose_ct(dctFFT, ACC);
    for (int j = 0; j < 2; ++j)
      FFTbackward(ct[j], ACC[j]);
    for (int j = 0; j < 2; ++j)
      for (int k = 0; k < N; ++k) {
	ZmodQ t = ct[j][k] * v_inverse;
	for (int l = 0; l < K; ++l) {
	  ZmodQ r = (t << g_bits_32[l]) >> g_bits_32[l];
	  //      if ((l==2) && (k ==0 ))
	  //       cout << r << ",";
	  t = (t-r) >> g_bits[l];
	  dct[j+2*l][k] = r;
	}
      }
    for (int j = 0; j < K2; ++j)
      FFTforward(dctFFT[j], dct[j]);
    // Mult_dct_ct(ACC, dct, C);
    for (int j = 0; j < 2; ++j)
      for (int k = 0; k < N2; ++k) {
	ACC[j][k] = (double complex) 0.0;
	for (int l = 0; l < K2; ++l)
	  ACC[j][k] += ((double complex) dctFFT[l][k]) * ((double complex) C[l][j][k]);
      }
      }
  
  void InitializeACC(ct_FFT1 ACC, int m) {	// Set a ciphertext to X^m * G (encryption of m without errors)
    ct_ModQ1 res;
    int mm = (((m % q) + q) % q) * (2*N/q);             // Reduce mod q (dealing with negative number as well)
    int sign = 1;
    if (mm >= N) { mm -= N; sign = -1; }
    
    for (int j = 0; j < 2; ++j)
      for (int k = 0; k < N; ++k)
	res[j][k]=0;
    res[1][mm] += sign*vgprime[0]; // [a,as+e] + X^m *G
    for (int j = 0; j < 2; ++j)
      FFTforward(ACC[j], res[j]);
  }
  
  LWE::CipherTextQN* MemberTest(Ring_FFT t, ct_FFT1 C) {
    Ring_FFT temp;
    Ring_ModQ temp_ModQ;
    
    LWE::CipherTextQN* ct = new LWE::CipherTextQN;
    for (int i = 0; i < N2; ++i)
      temp[i] = conj(((double complex) C[0][i]) * ((double complex)t[i]));  // Compute t*a
    FFTbackward(temp_ModQ, temp);
    for (int i = 0; i < N; ++i) ct->a[i] = temp_ModQ[i];
    for (int i = 0; i < N2; ++i)
      temp[i] = ((double complex) C[1][i]) * ((double complex) t[i]);
    FFTbackward(temp_ModQ, temp);
    ct->b = v+temp_ModQ[0];	
    return ct;    
  }
  
  void HomNAND(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {
    LWE::CipherText e12;
    for (int i = 0; i < n; ++i)
      e12.a[i] = (2*q - (ct1.a[i] + ct2.a[i])) % q;
    e12.b  =  (13 * q / 8) - (ct1.b + ct2.b) % q;

    ct_FFT1 ACC;
    InitializeACC(ACC, (e12.b + q/4) % q);
    for (int i = 0; i < n; ++i) {
      int a = (q - e12.a[i] % q) % q;
      for (int k = 0; k < BS_exp; ++k, 	a /= BS_base) {
	int a0 = a % BS_base;
	if (a0) AddToACC(ACC, *(EK.BSkey[i][a0][k]) );	
      }
    }
    LWE::CipherTextQN *eQN = MemberTest(t_TestMSB, ACC);
    LWE::CipherTextQ eQ;
    LWE::KeySwitch(&eQ, EK.KSkey, *eQN);
    LWE::ModSwitch(res, eQ);
    delete eQN;
  }
  
}
