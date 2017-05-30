#include <iostream>
#include <cstdlib>
#include "../LWE.h"
#include "../FHEW.h"
#include "common.h"
#include "../distrib.h"
#include <cassert>

using namespace std;

FHEW::EvalKey EK;
LWE::SecretKey LWEsk;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " SecretKeyFileName EvalKeyFileName  \n\n" 
       << "  Generate a secret key sk and evaluation key ek, and store them in two separate files.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc != 3) help(argv[0]);
  char* sk_fn = argv[1]; 
  char* ek_fn = argv[2]; 

  FHEW::Setup();
  LWE::KeyGen(LWEsk);
  FHEW::KeyGen(&EK, LWEsk);
  SaveEvalKey(&EK,ek_fn);
  SaveSecretKey(&LWEsk,sk_fn);
}
