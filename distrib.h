#ifndef DISTRIB_H
#define DISTRIB_H

#include "params.h"

typedef struct {
  double std_dev;
  int max; // Size of table. Set to 0 to ignore table and use rejection sampling
  int offset;
  const float* table; // CDF of Gaussian of standard deviation std_dev centered around offset
} Distrib;

int Sample(const Distrib& Chi); /* sample integer with gaussian distribution */ 

// Distribution of std dev 1.2
const float Chi1_Table[23] = {
1.12011750313263e-14, 2.38717233762211e-12, 3.04966971020178e-10,
2.34394541319773e-8, 1.08538196465647e-6, 0.0000303513786306856,
0.000514575939439740, 0.00532464699317562, 0.0340111330223921,
0.136723892128727, 0.357520614142345, 0.642479385857655,
0.863276107871273, 0.965988866977608, 0.994675353006824,
0.999485424060560, 0.999969648621369, 0.999998914618035,
0.999999976560546, 0.999999999695033, 0.999999999997613,
0.999999999999989, 1.00000000000000};

const Distrib Chi1 = {
  1.4,
  23,
  11,
  Chi1_Table
};

const float NoTable[1] = {1};

const Distrib Chi2 = {
  (double) (1 << 17),
  0,
  0,
  NoTable

 };

const Distrib Chi3 = {
  (double) 6,
  0,
  0,
  NoTable

 };

const float Binary_Table[3] = {
 .25,
 .75,
1.0 };


const Distrib Chi_Binary = {
  0,
  3,
  1,
  Binary_Table
};




#endif
