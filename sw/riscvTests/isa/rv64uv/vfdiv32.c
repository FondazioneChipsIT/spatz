// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matteo Perotti <mperotti@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "float_macros.h"
#include "vector_macros.h"

// Simple random test with similar values + 1 subnormal
void TEST_CASE1(void) {

  VSET(16, e32, m2);
  //               0.64838839,  0.00666664, -0.13619921,  0.21094505,
  //               -0.51040554, -0.77216595,  0.42111391,  0.82974166,
  //               -0.31227046,  0.68854737, -0.72970057,  0.10843290,
  //               -0.38442346,  0.18102080,  0.57249051,  0.76465768
  VLOAD_32(v4, 0x3f25fcc8, 0x3bda73da, 0xbe0b77ce, 0x3e5801fb, 0xbf02a9f0,
           0xbf45acab, 0x3ed79c3e, 0x3f5469f3, 0xbe9fe1ea, 0x3f3044a4,
           0xbf3acda8, 0x3dde1212, 0xbec4d327, 0x3e395d84, 0x3f128ebd,
           0x3f43c09b);
  //              -0.59629226, -0.46890569,  0.99662799, -0.49397555,
  //              0.80701596,  0.55786854, -0.26524273, -0.04642257,
  //              -0.67671824,  0.64403933,  0.06642481,  0.26544699,
  //              -0.00225505,  0.27478188,  0.76509053,  0.36194146
  VLOAD_32(v6, 0xbf18a69c, 0xbef01468, 0x3f7f2303, 0xbefcea5d, 0x3f4e9899,
           0x3f0ed079, 0xbe87cde5, 0xbd3e2597, 0xbf2d3d68, 0x3f24dfc3,
           0x3d8809bb, 0x3e87e8ab, 0xbb13c97d, 0x3e8cb036, 0x3f43dcf9,
           0x3eb95064);
  asm volatile("vfdiv.vv v2, v4, v6");
  //              -1.08736682, -0.01421745, -0.13666002, -0.42703542,
  //              -0.63246030, -1.38413608, -1.58765483, -17.87367058,
  //              0.46144828,  1.06910765, -10.98536205,  0.40849173,
  //              170.47213745,  0.65877998,  0.74826509,  2.11265564
  VCMP_U32(1, v2, 0xbf8b2ed5, 0xbc68f04d, 0xbe0bf09b, 0xbedaa462, 0xbf21e8ea,
           0xbfb12b5e, 0xbfcb3846, 0xc18efd46, 0x3eec42f2, 0x3f88d884,
           0xc12fc40a, 0x3ed125d4, 0x432a78dd, 0x3f28a5cd, 0x3f3f8e4c,
           0x400735c0);

};

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();
  // Change RM to RTZ since there are issues with FDIV + RNE in fpnew
  // Update: there are issues also with RTZ...
  //CHANGE_RM(RM_RTZ);

  TEST_CASE1();

  EXIT_CHECK();
}
