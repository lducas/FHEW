#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include "common.h"
#include <cassert>

using namespace std;

void help(char* cmd) {
  cerr << "\nusage: " << cmd << " Message SecretKeyFileName CipherTextFileName  \n\n" 
       << "  Encrypt the Message under some SecretKey and store it in a File.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc != 4) help(argv[0]);
  int message = atoi(argv[1]);
  char* sk_fn = argv[2]; 
  char* ct_fn = argv[3];

  if (!((message ==0)||(message ==1))){
      cerr << " The message must be 0 or 1.\n";
  exit(0);
  }

  LWE::SecretKey* SK = LoadSecretKey(sk_fn);

  LWE::CipherText ct;
  LWE::Encrypt(&ct, *SK, message);  SaveCipherText(&ct,ct_fn);
}
