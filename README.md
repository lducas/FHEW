
FHEW -- A library for doing Fully Homomorphic Encryption -- version 1.0 -- Release date: 2014.12.03

Authors: Leo Ducas (leo@ducas.org) and Daniele Micciancio (daniele@cs.ucsd.edu)

FHEW is open-source software distributed under the terms of the GNU General Public License.
See the file LICENSE for complete details on the licensing of FHEW.


== Requirement

FHEW requires the FFTW 3 library, the Fastest Fourier Transform of the West, 
available at http://www.fftw.org/download.html.

== Installation

Run Make.

== Command Line Usage

FHEW is a library and was not designed to be used directly from command line, but we do provide such interface for testing purposes. The cmd subfloder contains four programs: gen, enc, nand and dec. For example, you could run sequencially

cmd/gen sec.key ev.key
cmd/enc 0 sec.key a.ct
cmd/enc 1 sec.key b.ct
cmd/nand ev.key a.ct b.ct c.ct
cmd/dec sec.key c.ct 

The result should be 1.

== Library Interface

void FHEW::Setup();
//  Should be ran once (and only once) before any other function is used.

void LWE::KeyGen(SecretKey sk);
//  Generate an LWE secret key.

void Encrypt(CipherText* ct, const SecretKey sk, int m);
// Encrypt a message.

int Decrypt(const SecretKey sk, const CipherText& ct);
// Decrypt a ciphertext.

void FHEW::KeyGen(FHEW::EvalKey* EK, const LWE::SecretKey sk);
//  Generate an Evaluation Key from a secret key.

void FHEW::HomNAND(LWE::CipherText* res, const FHEW::EvalKey& EK, 
					const LWE::CipherText& ct1, const LWE::CipherText& ct2);
//  Perform a homomorphic NAND operation.
