#include <math.h>
#include "distrib.h" 
#include <cstdlib>
#include <iostream>

int Sample(const Distrib& Chi) { 
  if (Chi.max) {
    double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    for (int i = 0; i < Chi.max; ++i) 
      if (r<= Chi.table[i]) return i - Chi.offset;
    std::cerr << "Sampling Error: distribution table ending before (double) 1.0" << std::endl;
    exit(1);
  }

  double r, s = Chi.std_dev;
  if (s < 500){  
    int x, maxx = ceil(s*8);
    while(true) {
      x = rand() % (2*maxx + 1)  - maxx;
      r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      if (r < exp(- x*x / (2*s*s))) return x;
    }
  }

  // For some reason unknown to us, the previous implementation provides a bad distribution for large s...
  // We switch from "discrete gaussian" to rounded gaussian when s gets larger

  double x;

    while(true) {
  x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  x = 16 *x -8;
  r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      if (r < exp(- x*x / 2 )) return floor(.5 + x*s) ;
    }



}

