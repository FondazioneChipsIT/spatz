// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matteo Perotti <mperotti@iis.ee.ethz.ch>

#include "vector_macros.h"

// Naive test
void TEST_CASE1(void) {
  VSET(16, e8, m4);
  VLOAD_8(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v8, 1);
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e8, m1);
  VCMP_U8(1, v24, 73);

  VSET(16, e16, m4);
  VLOAD_16(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v8, 1);
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e16, m1);
  VCMP_U16(2, v24, 73);

  VSET(16, e32, m4);
  VLOAD_32(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v8, 1);
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e32, m1);
  VCMP_U32(3, v24, 73);

#if ELEN == 64
  VSET(16, e64, m4);
  VLOAD_64(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v8, 1);
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e64, m1);
  VCMP_U64(4, v24, 73);
#endif
}

// Masked naive test
void TEST_CASE2(void) {
  VSET(16, e8, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_8(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v8, 1);
  VLOAD_8(v24, 1);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VSET(1, e8, m1);
  VCMP_U8(5, v24, 37);

  VSET(16, e16, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_16(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v8, 1);
  VLOAD_16(v24, 1);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VSET(1, e16, m1);
  VCMP_U16(6, v24, 37);

  VSET(16, e32, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_32(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v8, 1);
  VLOAD_32(v24, 1);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VSET(1, e32, m1);
  VCMP_U32(7, v24, 37);

#if ELEN == 64
  VSET(16, e64, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_64(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v8, 1);
  VLOAD_64(v24, 1);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VSET(1, e64, m1);
  VCMP_U64(8, v24, 37);
#endif
}

// Are we respecting the undisturbed tail policy?
void TEST_CASE3(void) {
  VSET(16, e8, m4);
  VLOAD_8(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U8(9, v24, 73, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(16, e16, m4);
  VLOAD_16(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U16(10, v24, 73, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(16, e32, m4);
  VLOAD_32(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U32(11, v24, 73, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

#if ELEN == 64
  VSET(16, e64, m4);
  VLOAD_64(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U64(12, v24, 73, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
#endif
}

// Odd number of elements, undisturbed policy
void TEST_CASE4(void) {
  VSET(15, e8, m4);
  VLOAD_8(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U8(13, v24, 65, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(1, e16, m4);
  VLOAD_16(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U16(14, v24, 2, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(3, e32, m4);
  VLOAD_32(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U32(15, v24, 7, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

#if ELEN == 64
  VSET(7, e64, m4);
  VLOAD_64(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U64(16, v24, 29, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(15, e64, m4);
  VLOAD_64(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_64(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8");
  VCMP_U64(17, v24, 65, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
#endif
}

// Odd number of elements, undisturbed policy, and mask
void TEST_CASE5(void) {
  VSET(15, e8, m4);
  VLOAD_8(v0, 0x00, 0x40);
  VLOAD_8(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v8, 100, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_8(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VCMP_U8(18, v24, 107, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

  VSET(1, e16, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_16(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_16(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VCMP_U16(19, v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);

#if ELEN == 64
  VSET(3, e32, m4);
  VLOAD_8(v0, 0xaa, 0x55);
  VLOAD_32(v16, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  VLOAD_32(v24, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VCMP_U32(20, v24, 3, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
#endif
}

// To check for FPU to IPU FSM transition in VFU
void TEST_CASE6(void) {
#if ELEN == 64
  VSET(4, e64, m8);
  VLOAD_64(v16, 0x0000000000000000, 0x1000000000000001, 0x0000000000000000,
           0x0000000000000000);
  VLOAD_64(v8, 0x0000000000000007);
  asm volatile("vfadd.vv v0, v16, v8");
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e64, m1);
  VCMP_U64(21, v24, 0x1000000000000008);

  VSET(4, e64, m8);
  VCLEAR(v24);
  asm volatile("vredsum.vs v24, v16, v8");
  asm volatile("vfadd.vv v0, v16, v8");
  VSET(1, e64, m1);
  VCMP_U64(22, v24, 0x1000000000000008);
#endif
}

// Masked reduction at e8/m8 with vl = VLMAX (= VLEN elements): the mask for
// the upper elements must come from the upper part of v0.
// Regression for:
//   - Reduction_Read_V0_t reading v1 word 0 instead of v0 word 1
//   - v0 read as only 2 VRF words: on configs with 128-bit VRF words
//     (ELEN=32, e.g. the 32b cluster config) v0 is 4 words and bits >= 256
//     were never read
// Sizes derive from VLENB (read at runtime); B = VLENB = bytes of v0:
// v0 bytes [0,    B/2)  = 0x00 -> lower half of the elements inactive
// v0 bytes [B/2,  3B/4) = 0x01 -> 1 active element per byte  (B/4 total)
// v0 bytes [3B/4, B)    = 0x03 -> 2 active elements per byte (B/2 total)
// v1 = 0x0f (decoy: a DUT reading v1 as the upper v0 word gets a wrong sum)
// v16 = 1 everywhere, v8[0] = 0 -> correct sum = 3B/4 (48 on VLEN=512)
void TEST_CASE7(void) {
  unsigned int vlenb;
  asm volatile("csrr %0, vlenb" : "=r"(vlenb));
  const unsigned int vlmax = vlenb * 8; // e8/m8

  // Control: unmasked, vl = VLMAX/2 + 44 (300 on VLEN=512) -> sum mod 256
  const unsigned int vl_ctrl = vlmax / 2 + 44;
  VSET(vl_ctrl, e8, m8);
  asm volatile("vmv.v.i v16, 1");
  asm volatile("vmv.v.i v8, 0");
  asm volatile("vmv.v.i v24, 0");
  asm volatile("vredsum.vs v24, v16, v8");
  VSET(1, e8, m1);
  VCMP_U8(23, v24, (uint8_t)vl_ctrl);

  uint8_t *mask = (uint8_t *)snrt_l1alloc(vlenb);
  for (unsigned int i = 0; i < vlenb; i++)
    mask[i] = (i < vlenb / 2) ? 0x00 : (i < 3 * vlenb / 4) ? 0x01 : 0x03;
  VSET(vlenb, e8, m1);
  asm volatile("vle8.v v0, (%0)" ::"r"(mask) : "memory");
  asm volatile("vmv.v.i v1, 15");
  VSET(vlmax, e8, m8);
  asm volatile("vmv.v.i v16, 1");
  asm volatile("vmv.v.i v8, 0");
  asm volatile("vmv.v.i v24, 0");
  asm volatile("vredsum.vs v24, v16, v8, v0.t");
  VSET(1, e8, m1);
  VCMP_U8(24, v24, (uint8_t)(3 * vlenb / 4));
}

int main(void) {
  INIT_CHECK();
  enable_vec();

  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();
  TEST_CASE6();
  TEST_CASE7();

  EXIT_CHECK();
}
