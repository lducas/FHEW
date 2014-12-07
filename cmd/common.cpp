#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include <cassert>

using namespace std;


void SaveSecretKey(const LWE::SecretKey* LWEsk, char* filepath) {
  FILE * f;
  f = fopen(filepath, "wb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    exit(1);
  }
  cerr << "Writing Secret key to "<< filepath <<  ".\n";
  fwrite(LWEsk, sizeof(LWE::SecretKey), 1, f);
  fclose(f);
}

LWE::SecretKey* LoadSecretKey(char* filepath) {
  FILE * f;
  f = fopen(filepath, "rb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Read-Binary mode .\n";
    exit(1);
  }
  LWE::SecretKey* LWEsk = (LWE::SecretKey*) malloc(sizeof(LWE::SecretKey));  
  cerr << "Reading Secret key From "<< filepath <<  ".\n";
  assert(fread(LWEsk, sizeof(LWE::SecretKey), 1, f));
  cerr << "Secret Key read.\n";
  fclose(f);
  return LWEsk;
}





void SaveEvalKey(const FHEW::EvalKey *EK, char* filepath) {
  FILE * f;
  f = fopen(filepath, "wb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    exit(1);
  }
  cerr << "Writing Evaluation key to "<< filepath <<  ".\n";
  FHEW::fwrite_ek(*EK, f);
  fclose(f);
}

FHEW::EvalKey* LoadEvalKey(char* filepath) {
  FHEW::EvalKey* EK;
  FILE * f;
  f = fopen(filepath, "rb"); // rb -read binary
  if (f == NULL){
    cerr << "Failed to open "<< filepath <<  " in Read-Binary mode .\n";
    exit(1);
  }
  cerr << "Reading Evaluation key from "<< filepath <<  ".\n";
  EK = FHEW::fread_ek(f);
  cerr << "KSKey Read : " << N << "\t" << KS_base << "\t" << KS_exp << " .\n";
  fclose(f);
  return EK;
}







void SaveCipherText(const LWE::CipherText* ct, char* filepath){
  FILE * f;
  f = fopen(filepath, "wb"); // wb -write binary
  if (f == NULL){
    cerr << "Failed to open "<< filepath <<  " in Write-Binary mode .\n";
    exit(1);
  }
  cerr << "Writing CipherText to "<< filepath <<  ".\n";
  assert(fwrite(ct, sizeof(LWE::CipherText), 1, f));
  fclose(f);
}

LWE::CipherText* LoadCipherText(char* filepath) {
  FILE * f;
  f = fopen(filepath, "rb"); // wb -write binary
  if (f == NULL) {
    cerr << "Failed to open "<< filepath <<  " in Read-Binary mode.\n";
    exit(1);
  }
  cerr << "Loading CipherText from "<< filepath <<  ".\n";
  LWE::CipherText* ct = new LWE::CipherText;
  assert(fread(ct, sizeof(LWE::CipherText), 1, f));
  fclose(f);
  return ct;
}