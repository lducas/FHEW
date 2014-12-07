#include <iostream>
#include <cstdlib>
#include "LWE.h"
#include "FHEW.h"
#include "distrib.h"

using namespace std;

void help(char* cmd) {
  cerr << "\nusage: " << cmd << " n\n\n" 
  << "  Generate a secret key sk and evaluation key ek, and repeat the following test n times:\n"
  << "   - generate two random bits b1,b2\n"
  << "   - compute ciphertexts c1, c2, c3 and c4 encrypting b1, b2, b3 and b4  under sk\n"
  << "   - homomorphically compute the encrypted (c1 NAND c2) NAND (c3 NAND c4) \n"
  << "   - decrypt the all intermediate results and check correctness \n"
  << "\n If any of the tests fails, print ERROR and stop immediately.\n\n";
  exit(0);
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
  cerr << "Testing homomorphic NAND " << count << " times.\n"; 
  cerr << "Circuit shape : (a NAND b) NAND (c NAND d)\n\n";

  int v1,v2,sv1 = 2,sv2 = 2;
  LWE::CipherText se1, se2, e1, e2, e12;

  for (int i = 1; i <= 3*count; ++i) {

    if (i % 3){
      v1 = rand()%2;  
      v2 = rand()%2;
      LWE::Encrypt(&e1, LWEsk, v1);
      LWE::Encrypt(&e2, LWEsk, v2);
    }
    else { 
      v1 = sv1;
      v2 = sv2;
      e1 = se1;
      e2 = se2;
    }

    cerr << "Enc(" << v1 << ")  NAND  Enc(" << v2 << ")  =  ";

    FHEW::HomNAND(&e12, EK, e1, e2);
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

    if (1 - v1*v2 != v12) 
    { cerr << "ERROR at iteration " << i+1 << "\n"; 
    exit(1); 
  }
}

  cerr << "\nPassed all tests!\n\n";
}


