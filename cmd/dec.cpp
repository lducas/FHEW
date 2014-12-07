#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "../LWE.h"
#include "common.h"
#include <cassert>

using namespace std;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Decrypt the CipherText under some SecretKey and print it on the std output.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  if (argc != 3) help(argv[0]);
  char* sk_fn = argv[1]; 
  char* ct_fn = argv[2]; 

  FHEW::Setup();


  LWE::SecretKey* SK = LoadSecretKey(sk_fn);
  LWE::CipherText* ct = LoadCipherText(ct_fn);
  int m = LWE::Decrypt(*SK,*ct);
  cout << m << "\n";

}
