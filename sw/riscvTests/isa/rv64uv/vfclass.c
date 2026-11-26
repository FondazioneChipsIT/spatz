// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matheus Cavalcante <matheusd@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "float_macros.h"
#include "vector_macros.h"

// vfclass.v writes a 10-bit one-hot mask per element, zero-extended to SEW:
//   bit 0: -inf        bit 5: +subnormal
//   bit 1: -normal     bit 6: +normal
//   bit 2: -subnormal  bit 7: +inf
//   bit 3: -0          bit 8: sNaN
//   bit 4: +0          bit 9: qNaN
// Every element below sits next to an element of a different class, so a
// wrong per-lane result encoding (e.g. FPnew's packed 8-bit vectorial class
// format instead of the RISC-V 10-bit mask) or lane swapping is caught.

// All ten classes plus boundary values (max/min normal, max/min subnormal,
// NaNs with sign and payload), unmasked
void TEST_CASE1(void) {
  CLEAR_FFLAGS;
  VSET(16, e16, m8);
  //            -inf,   -1.0,   -max, -minsub, -maxsub,     -0,     +0, +minsub,
  //         +maxsub, +minnorm,  1.0,   +max,   +inf,   sNaN,   qNaN, -qNaN(payload)
  VLOAD_16(v16, mInfh, 0xbc00, mMaxh, 0x8001, 0x83ff, mZeroh, pZero, 0x0001,
           0x03ff, 0x0400, 0x3c00, pMaxh, pInfh, sNaNh, qNaNh, 0xfe01);
  asm volatile("vfclass.v v8, v16");
  VCMP_U16(1, v8, 0x001, 0x002, 0x002, 0x004, 0x004, 0x008, 0x010, 0x020,
           0x020, 0x040, 0x040, 0x040, 0x080, 0x100, 0x200, 0x200);
  // Classification never raises exceptions, not even for sNaN inputs
  CHECK_FFLAGS(0);

  CLEAR_FFLAGS;
  VSET(16, e32, m8);
  VLOAD_32(v16, mInff, 0xbf800000, mMaxf, 0x80000001, 0x807fffff, mZerof,
           pZero, 0x00000001, 0x007fffff, 0x00800000, 0x3f800000, pMaxf, pInff,
           sNaNf, qNaNf, 0xffc00001);
  asm volatile("vfclass.v v8, v16");
  VCMP_U32(2, v8, 0x001, 0x002, 0x002, 0x004, 0x004, 0x008, 0x010, 0x020,
           0x020, 0x040, 0x040, 0x040, 0x080, 0x100, 0x200, 0x200);
  CHECK_FFLAGS(0);

#if ELEN == 64
  CLEAR_FFLAGS;
  VSET(16, e64, m8);
  VLOAD_64(v16, mInfd, 0xbff0000000000000, mMaxd, 0x8000000000000001,
           0x800fffffffffffff, mZerod, pZero, 0x0000000000000001,
           0x000fffffffffffff, 0x0010000000000000, 0x3ff0000000000000, pMaxd,
           pInfd, sNaNd, qNaNd, 0xfff8000000000001);
  asm volatile("vfclass.v v8, v16");
  VCMP_U64(3, v8, 0x001, 0x002, 0x002, 0x004, 0x004, 0x008, 0x010, 0x020,
           0x020, 0x040, 0x040, 0x040, 0x080, 0x100, 0x200, 0x200);
  CHECK_FFLAGS(0);
#endif
}

// NaN corner cases: sNaN with sign bit set and with a full payload, qNaN
// with all bits set, and canonical NaNs, mixed with ordinary values
void TEST_CASE2(void) {
  VSET(8, e16, m8);
  //            -sNaN, sNaN(max payload), all-ones qNaN, canonical qNaN,
  //            2.0, -0.5, +0, -inf
  VLOAD_16(v16, 0xfc01, 0x7dff, 0xffff, qNaNh, 0x4000, 0xb800, pZero, mInfh);
  asm volatile("vfclass.v v8, v16");
  VCMP_U16(4, v8, 0x100, 0x100, 0x200, 0x200, 0x040, 0x002, 0x010, 0x001);

  VSET(8, e32, m8);
  VLOAD_32(v16, 0xff800001, 0x7fbfffff, 0xffffffff, qNaNf, 0x40000000,
           0xbf000000, pZero, mInff);
  asm volatile("vfclass.v v8, v16");
  VCMP_U32(5, v8, 0x100, 0x100, 0x200, 0x200, 0x040, 0x002, 0x010, 0x001);

#if ELEN == 64
  VSET(8, e64, m8);
  VLOAD_64(v16, 0xfff0000000000001, 0x7ff7ffffffffffff, 0xffffffffffffffff,
           qNaNd, 0x4000000000000000, 0xbfe0000000000000, pZero, mInfd);
  asm volatile("vfclass.v v8, v16");
  VCMP_U64(6, v8, 0x100, 0x100, 0x200, 0x200, 0x040, 0x002, 0x010, 0x001);
#endif
}

// Same values as TEST_CASE1 (masked)
void TEST_CASE3(void) {
  VSET(16, e16, m8);
  VLOAD_16(v16, mInfh, 0xbc00, mMaxh, 0x8001, 0x83ff, mZeroh, pZero, 0x0001,
           0x03ff, 0x0400, 0x3c00, pMaxh, pInfh, sNaNh, qNaNh, 0xfe01);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v8);
  asm volatile("vfclass.v v8, v16, v0.t");
  VCMP_U16(7, v8, 0x0, 0x002, 0x0, 0x004, 0x0, 0x008, 0x0, 0x020, 0x0, 0x040,
           0x0, 0x040, 0x0, 0x100, 0x0, 0x200);

  VSET(16, e32, m8);
  VLOAD_32(v16, mInff, 0xbf800000, mMaxf, 0x80000001, 0x807fffff, mZerof,
           pZero, 0x00000001, 0x007fffff, 0x00800000, 0x3f800000, pMaxf, pInff,
           sNaNf, qNaNf, 0xffc00001);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v8);
  asm volatile("vfclass.v v8, v16, v0.t");
  VCMP_U32(8, v8, 0x0, 0x002, 0x0, 0x004, 0x0, 0x008, 0x0, 0x020, 0x0, 0x040,
           0x0, 0x040, 0x0, 0x100, 0x0, 0x200);

#if ELEN == 64
  VSET(16, e64, m8);
  VLOAD_64(v16, mInfd, 0xbff0000000000000, mMaxd, 0x8000000000000001,
           0x800fffffffffffff, mZerod, pZero, 0x0000000000000001,
           0x000fffffffffffff, 0x0010000000000000, 0x3ff0000000000000, pMaxd,
           pInfd, sNaNd, qNaNd, 0xfff8000000000001);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v8);
  asm volatile("vfclass.v v8, v16, v0.t");
  VCMP_U64(9, v8, 0x0, 0x002, 0x0, 0x004, 0x0, 0x008, 0x0, 0x020, 0x0, 0x040,
           0x0, 0x040, 0x0, 0x100, 0x0, 0x200);
#endif
}

// vl < VLMAX: tail elements must be left untouched
void TEST_CASE4(void) {
  VSET(16, e32, m8);
  VLOAD_32(v16, mInff, 0xbf800000, mMaxf, 0x80000001, 0x807fffff, mZerof,
           pZero, 0x00000001, 0x007fffff, 0x00800000, 0x3f800000, pMaxf, pInff,
           sNaNf, qNaNf, 0xffc00001);
  VCLEAR(v8);

  VSET(13, e32, m8);
  asm volatile("vfclass.v v8, v16");
  VCMP_U32(10, v8, 0x001, 0x002, 0x002, 0x004, 0x004, 0x008, 0x010, 0x020,
           0x020, 0x040, 0x040, 0x040, 0x080);

  VSET(16, e32, m8);
  VCMP_U32(11, v8, 0x001, 0x002, 0x002, 0x004, 0x004, 0x008, 0x010, 0x020,
           0x020, 0x040, 0x040, 0x040, 0x080, 0x0, 0x0, 0x0);
}

// vl < VLMAX with an odd vl and a mask: only the active body elements change
void TEST_CASE5(void) {
  VSET(16, e16, m8);
  VLOAD_16(v16, mInfh, 0xbc00, mMaxh, 0x8001, 0x83ff, mZeroh, pZero, 0x0001,
           0x03ff, 0x0400, 0x3c00, pMaxh, pInfh, sNaNh, qNaNh, 0xfe01);
  VLOAD_8(v0, 0xAA, 0xAA);
  VCLEAR(v8);

  VSET(13, e16, m8);
  asm volatile("vfclass.v v8, v16, v0.t");
  VCMP_U16(12, v8, 0x0, 0x002, 0x0, 0x004, 0x0, 0x008, 0x0, 0x020, 0x0, 0x040,
           0x0, 0x040, 0x0);

  VSET(16, e16, m8);
  VCMP_U16(13, v8, 0x0, 0x002, 0x0, 0x004, 0x0, 0x008, 0x0, 0x020, 0x0, 0x040,
           0x0, 0x040, 0x0, 0x0, 0x0, 0x0);
}

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();

  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();

  EXIT_CHECK();
}
