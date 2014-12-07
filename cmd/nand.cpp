#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

FHEW::EvalKey* EK;


void help(char* cmd) {
  cerr << "\nusage: " << cmd << " EvalKeyFileName InCTFileName1 InCTFileName2 OutCTFileName  \n\n" 
       << "  Perform Homomorphic NAND computation.\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  if (argc != 5) help(argv[0]);
  char* ek_fn = argv[1]; 
  char* ict1_fn = argv[2]; 
  char* ict2_fn = argv[3]; 
  char* oct_fn = argv[4]; 

  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

  LWE::CipherText *ct1,*ct2,*ct3;

  ct1 = LoadCipherText(ict1_fn);
  ct2 = LoadCipherText(ict2_fn);
  ct3 = new LWE::CipherText;

  FHEW::HomNAND(ct3, *EK,*ct1,*ct2);

  SaveCipherText(ct3,oct_fn);
}
