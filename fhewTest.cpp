#include <iostream>
#include <cstdlib>
#include "LWE.h"
#include "FHEW.h"
#include "distrib.h"

using namespace std;

void help(char* cmd) {
  cerr << "\nusage: " << cmd << " n\n\n" 
  << "  Generate a secret key sk and evaluation key ek, and repeat the following test n times:\n"
  << "   - generate random bits b1,b2,b3,b4\n"
  << "   - compute ciphertexts c1, c2, c3 and c4 encrypting b1, b2, b3 and b4  under sk\n"
  << "   - homomorphically compute the encrypted (c1 NAND c2) NAND (c3 NAND c4) \n"
  << "   - decrypt all the intermediate results and check correctness \n"
  << "\n If any of the tests fails, print ERROR and stop immediately.\n\n";
  exit(0);
}

int cleartext_gate(int v1, int v2, BinGate gate){
  switch(gate)
  {
    case OR: return v1 || v2;
    case AND: return v1 && v2;
    case NOR: return not(v1 || v2);
    case NAND: return not(v1 && v2);
    default: cerr << "\n This gate does not exists \n"; exit(1); return 0;
  }
}

void cerr_gate(BinGate gate){
  switch(gate)
  {
    case OR: cerr << " OR\t"; return;
    case AND: cerr << " AND\t"; return;
    case NOR: cerr << " NOR\t"; return;
    case NAND: cerr << " NAND\t"; return;
  }
}


int main(int argc, char *argv[]) {
  if (argc != 2) help(argv[0]);
  int count = atoi(argv[1]); 

  cerr << "Setting up FHEW \n";
  FHEW::Setup();
  cerr << "Generating secret key ... ";
  LWE::SecretKey LWEsk;
  LWE::KeyGen(LWEsk);
  cerr << " Done.\n";
  cerr << "Generating evaluation key ... this may take a while ... ";
  FHEW::EvalKey EK;
  FHEW::KeyGen(&EK, LWEsk);
  cerr << " Done.\n\n";
  cerr << "Testing depth-2 homomorphic circuits " << count << " times.\n"; 
  cerr << "Circuit shape : (a GATE NOT(b)) GATE (c GATE d)\n\n";

  int v1,v2,sv1 = 2,sv2 = 2;
  LWE::CipherText se1, se2, e1, e2, e12;

  for (int i = 1; i <= 3*count; ++i) {

    if (i % 3){
      v1 = rand()%2;  
      v2 = rand()%2;
      LWE::Encrypt(&e1, LWEsk, v1);
      LWE::Encrypt(&e2, LWEsk, v2);
      if (i%3 == 1){
        cerr << "       NOT\tEnc(" << v2 << ") = ";
        FHEW::HomNOT(&e2, e2);
        int notv2 = LWE::Decrypt(LWEsk, e2);
        cerr << "Enc(" << notv2 << ")" << endl;
        if (not (notv2 == not(v2))){
          cerr << "\n ERROR: incorrect NOT Homomorphic computation at iteration " << i+1 << "\n";
          exit(1);
        }
        v2 = not v2;
      }
    }
    else { 
      v1 = sv1;
      v2 = sv2;
      e1 = se1;
      e2 = se2;
    }

    BinGate gate = static_cast<BinGate>(rand()%4);

    cerr << "Enc(" << v1 << ")";
    cerr_gate(gate);
    cerr << "Enc(" << v2 << ") = ";

    FHEW::HomGate(&e12, gate, EK, e1, e2);
    int v12 = LWE::Decrypt(LWEsk, e12);

    cerr << "Enc(" << v12 << ")";
    cerr << endl;

    if (i % 3 == 1){
      sv1 = v12;
      se1 = e12;
    }
    if (i % 3 == 2){
      sv2 = v12;
      se2 = e12;
    }
    if (i % 3 == 0)
      cerr << endl;

    if (cleartext_gate(v1, v2, gate) != v12) 
      { cerr << "\n ERROR: incorrect Homomorphic Gate computation at iteration " << i+1 << "\n"; 
    exit(1); 
  }
}

cerr << "\nPassed all tests!\n\n";
}


