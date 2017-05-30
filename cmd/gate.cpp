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
  cerr << "\nusage: " << cmd << " Gate EvalKeyFileName InCTFileName1 InCTFileName2 OutCTFileName  \n\n" 
       << "  Perform Homomorphic Gate computation where Gate={AND, OR, NAND, NOR}\n\n";
  exit(0);
}


int main(int argc, char *argv[]) {
  if (argc != 6) help(argv[0]);
  string gate_n = argv[1]; 
  char* ek_fn = argv[2]; 
  char* ict1_fn = argv[3]; 
  char* ict2_fn = argv[4]; 
  char* oct_fn = argv[5]; 

  BinGate gate;
    if (gate_n=="OR") gate = OR; 
    else if (gate_n=="AND") gate = AND; 
    else if (gate_n=="NOR") gate = NOR; 
    else if (gate_n=="NAND") gate = NAND; 
    else 
      {
          cerr << "\n This gate does not exists (please choose {AND, OR, NAND, NOR}) \n"; 
      exit(1); 
    }



  FHEW::Setup();

  EK = LoadEvalKey(ek_fn);

  LWE::CipherText *ct1,*ct2,*ct3;

  ct1 = LoadCipherText(ict1_fn);
  ct2 = LoadCipherText(ict2_fn);
  ct3 = new LWE::CipherText;


  FHEW::HomGate(ct3, gate, *EK,*ct1,*ct2);

  SaveCipherText(ct3,oct_fn);
}
