#include <cstdlib>
#include <iostream>
#include "LWE.h"
#include "params.h"
#include <cassert>

using namespace std;
namespace LWE {

  void KeyGen(SecretKey sk) {
    KeyGenRestart:
    int s=0, ss=0;
    for (int i = 0; i < n; ++i) {

          sk[i] = Sample(Chi_Binary);
          s+= sk[i];
          ss+= abs(sk[i]);
        }
      if (abs(s)>5) goto KeyGenRestart;
      if (abs(ss - n/2)>5) goto KeyGenRestart;
  }

  void KeyGenN(SecretKeyN sk) {
    for (int i = 0; i < N; ++i) sk[i] = Sample(Chi1);
  }
  
  void Encrypt(CipherText* ct, const SecretKey sk, int m) {
    ct->b = (m % 4) * q / 4;
    ct->b += Sample(Chi3);
    for (int i = 0; i < n; ++i)	{
      ct->a[i] = rand() % q;
      ct->b = (ct->b + ct->a[i] * sk[i]) % q;
    }
  }

  int Decrypt(const SecretKey sk, const CipherText& ct) {
    int r = ct.b;
    for (int i = 0; i < n; ++i) r -= ct.a[i] * sk[i];
    r = ((r % q) + q + q/8) % q;
    return 4*r/q;    
  }

  void DecryptDetail(const SecretKey sk, const CipherText& ct) {
    int r = ct.b;
    for (int i = 0; i < n; ++i) r -= ct.a[i] * sk[i];
    r = ((r % q) + q + q/8) % q;
    int m = 4*r/q;
    cerr << "\t Value " << r - q/8 << "\t Decoded as " << m << " * " << q/4 << "\t + " << r - m * q/4 - q/8<< endl;
    cout << r - m * q/4 - q/8 << ", "; 
  }

    
  int round_qQ(ZmodQ v) {
    return floor(.5 + (double) v * (double) q / (double) Q) + q % q;
  }
  
  void ModSwitch(CipherText* ct, const CipherTextQ& c) {
    for (int i = 0; i < n; ++i) 
      ct->a[i] = round_qQ(c.a[i]);	
    ct->b = round_qQ(c.b);
  }
  
  void SwitchingKeyGen(SwitchingKey res, const SecretKey new_sk, const SecretKeyN old_sk) {
    for (int i = 0; i < N; ++i) 
      for (int j = 0; j < KS_base; ++j)
	for (int k = 0; k < KS_exp; ++k) {
	  CipherTextQ* ct = new CipherTextQ;    
	  ct->b = -old_sk[i]*j*KS_table[k] + Sample(Chi2);
	  for (int l = 0; l < n; ++l) {
	    ct->a[l] = rand();
	    ct->b += ct->a[l] * new_sk[l];
	  }
	  res[i][j][k] = ct;
	}
  }
  
  void KeySwitch(CipherTextQ* res, const SwitchingKey K, const CipherTextQN& ct) {
    for (int k = 0; k < n; ++k) res->a[k] = 0;
    res->b = ct.b;
    for (int i = 0; i < N; ++i) {
      uZmodQ a = -ct.a[i];
      for (int j = 0; j < KS_exp; ++j, a /= KS_base) {
	uZmodQ a0 = a % KS_base;
	for (int k = 0; k < n; ++k)
	  res->a[k] -= (K[i][a0][j])->a[k];
	res->b -= (K[i][a0][j])->b;
      }
    } 
  }
  
}
