#include <complex.h>
#include <fftw3.h>
#include "FFT.h"
#include "params.h"

double *in;
fftw_complex *out;
fftw_plan plan_fft_forw, plan_fft_back;
  
void FFTsetup() {
  in = (double*) fftw_malloc(sizeof(double) * 2*N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N + 2));
  plan_fft_forw = fftw_plan_dft_r2c_1d(2*N, in, out,  FFTW_PATIENT);
  plan_fft_back = fftw_plan_dft_c2r_1d(2*N, out, in,  FFTW_PATIENT);
}
  
void FFTforward(Ring_FFT res, const Ring_ModQ val) {
  for (int k = 0; k < N; ++k)	{
    in[k] = (double) (val[k]);
    in[k+N] = 0.0;			
  }
  fftw_execute(plan_fft_forw); 
  for (int k = 0; k < N2; ++k) 
    res[k] = (double complex) out[2*k+1];				
}

void FFTbackward(Ring_ModQ res, const Ring_FFT val){
  for (int k = 0; k < N2; ++k) {
    out[2*k+1] = (double complex) val[k]/N;
    out[2*k]   = (double complex) 0;
  }
  fftw_execute(plan_fft_back); 
  for (int k = 0; k < N; ++k)	
    res[k] = (long int) round(in[k]);
}
