#ifndef FHEW_H
#define FHEW_H

#include "params.h"
#include "LWE.h"

namespace FHEW {

  typedef Ring_FFT  ct_FFT[K2][2];  // Ciphertext in FFT form
  typedef ct_FFT* BootstrappingKey[n][BS_base][BS_exp];
  typedef struct {
    BootstrappingKey BSkey;
    LWE::SwitchingKey KSkey;
  } EvalKey;

  void Setup();
  void KeyGen(EvalKey* EK, const LWE::SecretKey LWEsk);
  void HomNAND(LWE::CipherText* res, const EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2);

  void fwrite_ek(const EvalKey& EK, FILE* f);
  EvalKey* fread_ek(FILE* f);
}

#endif
