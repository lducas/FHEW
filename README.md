# FHEW 

## A Fully Homomorphic Encryption library

**version 2.0-alpha** -- *Release date: 2017.05.30*

**Updates**
- Made Homomorphic gate computation 6x faster, by noticing that it is sufficient to compute ACC[1]. This trick is somehow equivalent to the external product from [this paper](http://eprint.iacr.org/2016/870)
- Implemented support for more gates: AND, OR, NAND, NOR, NOT
- Forbids operation on non-independant ciphertext: (x OP x) or (x OP (not x))
- Bugfixes

**Authors:** Leo Ducas <leo@ducas.org> and Daniele Micciancio <daniele@cs.ucsd.edu>

FHEW is open-source software distributed under the terms of the GNU
General Public License. See the file LICENSE for complete details on the licensing of FHEW. 
The FHEW library is based on the Fully Homorphic Encryption scheme
described in the paper ["FHE bootstrapping in less than a second"](http://eprint.iacr.org/2014/816)
(L. Ducas and D. Micciancio, Cryptology  ePrint Archive 2014/816,)
and makes use of the [FFTW](http://www.fftw.org) library (the "Fastest
Fourier Transform in the West"). Hence the name FHEW, which you may
read as the "Fastest Homomorphic Encryption in the West", though the
name is more of a reference to FFTW than a claim about performance. The
library provides a symmetric encryption scheme to encrypt (and
decrypt) single bit messages, supporting the homomorphic evaluation of
arbitrary boolean circuits on encrypted data using a public (evaluation) key.

**Note** Please do not expect maintenance of this code.

### Requirements 

FHEW requires the FFTW 3 library available at <http://www.fftw.org/download.html>, 
and a c++ compiler.  The library is written primarily in C, but a C++
compiler is needed  to support a few syntactical extensions (like
namespaces and reference parameters) used to improve the readibility
of the code.  Editing the library so to require just a c compiler
should be straightforward. The library has been tested using the g++
compiler from the  [GNU compiler collection](http://gcc.gnu.org) (gcc
version 4.9.1), but should work fine with other compilers as well.

### Installation

To build the library, just run ```make```.  This will produce a
library (libfhew.a), and a number of test/example programs. (See
below.) You can also run ``` make install``` to install the header
files and fhew library required to compile your own programs making
use of FHEW. By default, these are installed in $(HOME)/include and
$(HOME)/lib. Edit the Makefile to fit your needs. 

You can test the library by running the test program ```fhewTest```.
Running the test program with no arguments prints a simple usage message. 

### Command Line Usage

FHEW is a library and was not designed to be used directly from the
command line, but we do provide a command line interface for testing purposes. 
The *cmd* subfolder contains four programs: *gen*, *enc*, *nand* and
*dec* for key generation, encryption, homorphic computation and
decryption. Running each command with no arguments prints out a simple
usage message. The commands can be used as follows: 

```
gen sec.key ev.key
enc 0 sec.key a.ct
enc 1 sec.key b.ct
gate nand ev.key a.ct b.ct c.ct
dec sec.key c.ct 
```
This generates a secret key and corresponding evaluation key, which
are stored in the files sec.key and ev.key. 
The secret key is then used to encrypt the message 0 and the message
1, storing the ciphertexts in a.ct and b.ct.
The homomorphic NAND of the two ciphertexts is computed using the
evaluation key. Finally, the resulting ciphertext stored in c.ct is decrypted.
The output of the last command should be 1.

### Library Interface

```
void FHEW::Setup();
```
Should be run once (and only once) before any other function is used.

```
void LWE::KeyGen(LWE::SecretKey sk);
```
Generate an LWE secret key.

```
void LWE::Encrypt(LWE::CipherText* ct, const LWE::SecretKey sk, int m);
```
Encrypt a message.
(Note: please initialize your randomness with srand())

```
int LWE::Decrypt(const LWE::SecretKey sk, const LWE::CipherText& ct);
```
Decrypt a ciphertext.

```
void FHEW::KeyGen(FHEW::EvalKey* EK, const LWE::SecretKey sk);
```
Generate an Evaluation Key from a secret key. 
(Note: please initialize your randomness with srand())

```
void FHEW::HomNAND(LWE::CipherText* res, const FHEW::EvalKey& EK, 
					const LWE::CipherText& ct1, const LWE::CipherText& ct2);
```
Perform a homomorphic NAND operation. Deprecated due to the more general function below.

```
void HomGate(LWE::CipherText* res, const BinGate gate, const EvalKey& EK, 
					const LWE::CipherText& ct1, const LWE::CipherText& ct2);
```
Perform a homomorphic OP operation where OP={OR,AND,NOR,NAND}.

```
void HomNOT(LWE::CipherText* res, const LWE::CipherText& ct);
```
Perform a homomorphic NOT operation.
(note: does not require any key material).

