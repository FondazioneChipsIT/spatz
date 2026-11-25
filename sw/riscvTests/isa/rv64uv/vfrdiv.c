// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// vfrdiv.vf vd, vs2, rs1, vm  ->  vd[i] = f[rs1] / vs2[i]
//
// Golden values computed with numpy (IEEE-754, RNE).

#include "float_macros.h"
#include "vector_macros.h"

// Operand order: exact power-of-two quotients, so no flags are raised.
// A swapped implementation returns vs2 instead of 1/vs2.
void TEST_CASE1(void) {
  CLEAR_FFLAGS;
  VSET(16, e16, m8);
  //            2, 4, 8, -0.5, 0.25, -16, 1, 0.125, -2, -4, -8, 0.5, -0.25, 16, -1, -0.125
  VLOAD_16(v16, 0x4000, 0x4400, 0x4800, 0xb800, 0x3400, 0xcc00, 0x3c00, 0x3000,
           0xc000, 0xc400, 0xc800, 0x3800, 0xb400, 0x4c00, 0xbc00, 0xb000);
  float s16;
  BOX_HALF_IN_FLOAT(s16, 0x3c00);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s16));
  VCMP_U16(1, v8, 0x3800, 0x3400, 0x3000, 0xc000, 0x4400, 0xac00, 0x3c00, 0x4800,
            0xb800, 0xb400, 0xb000, 0x4000, 0xc400, 0x2c00, 0xbc00, 0xc800);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(0);
  CLEAR_FFLAGS;
  VSET(16, e32, m8);
  //            2, 4, 8, -0.5, 0.25, -16, 1, 0.125, -2, -4, -8, 0.5, -0.25, 16, -1, -0.125
  VLOAD_32(v16, 0x40000000, 0x40800000, 0x41000000, 0xbf000000, 0x3e800000,
           0xc1800000, 0x3f800000, 0x3e000000, 0xc0000000, 0xc0800000,
           0xc1000000, 0x3f000000, 0xbe800000, 0x41800000, 0xbf800000,
           0xbe000000);
  float s32;
  BOX_FLOAT_IN_FLOAT(s32, 0x3f800000);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s32));
  VCMP_U32(2, v8, 0x3f000000, 0x3e800000, 0x3e000000, 0xc0000000, 0x40800000,
            0xbd800000, 0x3f800000, 0x41000000, 0xbf000000, 0xbe800000,
            0xbe000000, 0x40000000, 0xc0800000, 0x3d800000, 0xbf800000,
            0xc1000000);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(0);
#if ELEN == 64
  CLEAR_FFLAGS;
  VSET(16, e64, m8);
  //            2, 4, 8, -0.5, 0.25, -16, 1, 0.125, -2, -4, -8, 0.5, -0.25, 16, -1, -0.125
  VLOAD_64(v16, 0x4000000000000000, 0x4010000000000000, 0x4020000000000000,
           0xbfe0000000000000, 0x3fd0000000000000, 0xc030000000000000,
           0x3ff0000000000000, 0x3fc0000000000000, 0xc000000000000000,
           0xc010000000000000, 0xc020000000000000, 0x3fe0000000000000,
           0xbfd0000000000000, 0x4030000000000000, 0xbff0000000000000,
           0xbfc0000000000000);
  double s64;
  BOX_DOUBLE_IN_DOUBLE(s64, 0x3ff0000000000000);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s64));
  VCMP_U64(3, v8, 0x3fe0000000000000, 0x3fd0000000000000, 0x3fc0000000000000,
            0xc000000000000000, 0x4010000000000000, 0xbfb0000000000000,
            0x3ff0000000000000, 0x4020000000000000, 0xbfe0000000000000,
            0xbfd0000000000000, 0xbfc0000000000000, 0x4000000000000000,
            0xc010000000000000, 0x3fb0000000000000, 0xbff0000000000000,
            0xc020000000000000);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(0);
#endif
}

// Random values, unmasked
void TEST_CASE2(void) {
  VSET(16, e16, m8);
  VLOAD_16(v16, 0x3927, 0x3b38, 0x3a4c, 0x3439, 0x355d, 0x3b0a, 0xab0a, 0x3aa4,
           0xba75, 0xb7ea, 0x3568, 0xb508, 0x34ad, 0x3791, 0xb83c, 0xb89b);
  float s16;
  BOX_HALF_IN_FLOAT(s16, 0xbb6e);  // -0.928711
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s16));
  VCMP_U16(4, v8, 0xbdc5, 0xbc1e, 0xbcb8, 0xc30a, 0xc18b, 0xbc39, 0x4c39, 0xbc7a,
            0x3c9a, 0x3f83, 0xc17f, 0x41e8, 0xc25b, 0xbfdb, 0x3f05, 0x3e74);
  VSET(16, e32, m8);
  VLOAD_32(v16, 0x3f0a055e, 0x3efc5cd4, 0xbf6bdaef, 0x3f25d3ec, 0x3f09d55a,
           0x3f05a3be, 0x3e91fdc3, 0x3d7ab163, 0xbe6e5e6a, 0x3f351a2b,
           0xbe7659a8, 0xbecd57ac, 0xbd5b54a8, 0x3f56aae8, 0x3e4975b1,
           0x3e9bc2a3);
  float s32;
  BOX_FLOAT_IN_FLOAT(s32, 0x3f3e2032);  // 0.742679
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s32));
  VCMP_U32(5, v8, 0x3fb0525e, 0x3fc0ddb9, 0xbf4e5d5f, 0x3f92c156, 0x3fb08fca,
            0x3fb61a38, 0x4026b212, 0x41422692, 0xc04c3049, 0x3f8660b0,
            0xc04592bc, 0xbfed0785, 0xc15de992, 0x3f62bb9b, 0x407198ff,
            0x401c3d9e);
#if ELEN == 64
  VSET(16, e64, m8);
  VLOAD_64(v16, 0xbfd92a33ec634f60, 0xbfe3c8e85e05279a, 0x3fbb35c2859a521d,
           0xbfdac49c8881bccc, 0xbfd6d73235112dae, 0x3fc8aa0eb7683d3c,
           0x3fea6aab6ec8666e, 0xbfda45342a552e3f, 0xbfef5a9b8774b114,
           0x3fe38926befbec98, 0xbfe3fe63c943c2a8, 0xbfe4febe9d78bfe2,
           0xbfe62a01a1f3d55b, 0x3fc8bc5ee79abe1a, 0xbfddf8974f5225ba,
           0x3fd1c3f71445c41f);
  double s64;
  BOX_DOUBLE_IN_DOUBLE(s64, 0xbfe7939b41dde2ca);  // -0.736768
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(s64));
  VCMP_U64(6, v8, 0x3ffdfaf751aed77f, 0x3ff311018065f6d9, 0xc01bba1aa33c3d87,
            0x3ffc2f4e52d09cdc, 0x400083fb92fe3bf5, 0xc00e96bb3d31a6bf,
            0xbfec8f4487e04de1, 0x3ffcb7ff9f9e012e, 0x3fe80ff92b33f4cf,
            0xbff34f3b04edfca3, 0x3ff2de01126a2dfa, 0x3ff1f7a1a50a4a30,
            0x3ff10508f2c17eb6, 0xc00e8015bc3ac807, 0x3ff92c326e6c9299,
            0xc0053bcda6ee6a3a);
#endif
}

// Random values, masked: inactive elements must keep the old vd value
void TEST_CASE3(void) {
  VSET(16, e16, m8);
  VLOAD_16(v16, 0x3927, 0x3b38, 0x3a4c, 0x3439, 0x355d, 0x3b0a, 0xab0a, 0x3aa4,
           0xba75, 0xb7ea, 0x3568, 0xb508, 0x34ad, 0x3791, 0xb83c, 0xb89b);
  VLOAD_16(v8, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead,
           0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead);
  VLOAD_8(v0, 0xAA, 0xAA);
  float s16;
  BOX_HALF_IN_FLOAT(s16, 0xbb6e);
  asm volatile("vfrdiv.vf v8, v16, %[A], v0.t" ::[A] "f"(s16));
  VCMP_U16(7, v8, 0xdead, 0xbc1e, 0xdead, 0xc30a, 0xdead, 0xbc39, 0xdead, 0xbc7a,
            0xdead, 0x3f83, 0xdead, 0x41e8, 0xdead, 0xbfdb, 0xdead, 0x3e74);
  VSET(16, e32, m8);
  VLOAD_32(v16, 0x3f0a055e, 0x3efc5cd4, 0xbf6bdaef, 0x3f25d3ec, 0x3f09d55a,
           0x3f05a3be, 0x3e91fdc3, 0x3d7ab163, 0xbe6e5e6a, 0x3f351a2b,
           0xbe7659a8, 0xbecd57ac, 0xbd5b54a8, 0x3f56aae8, 0x3e4975b1,
           0x3e9bc2a3);
  VLOAD_32(v8, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef);
  VLOAD_8(v0, 0xAA, 0xAA);
  float s32;
  BOX_FLOAT_IN_FLOAT(s32, 0x3f3e2032);
  asm volatile("vfrdiv.vf v8, v16, %[A], v0.t" ::[A] "f"(s32));
  VCMP_U32(8, v8, 0xdeadbeef, 0x3fc0ddb9, 0xdeadbeef, 0x3f92c156, 0xdeadbeef,
            0x3fb61a38, 0xdeadbeef, 0x41422692, 0xdeadbeef, 0x3f8660b0,
            0xdeadbeef, 0xbfed0785, 0xdeadbeef, 0x3f62bb9b, 0xdeadbeef,
            0x401c3d9e);
#if ELEN == 64
  VSET(16, e64, m8);
  VLOAD_64(v16, 0xbfd92a33ec634f60, 0xbfe3c8e85e05279a, 0x3fbb35c2859a521d,
           0xbfdac49c8881bccc, 0xbfd6d73235112dae, 0x3fc8aa0eb7683d3c,
           0x3fea6aab6ec8666e, 0xbfda45342a552e3f, 0xbfef5a9b8774b114,
           0x3fe38926befbec98, 0xbfe3fe63c943c2a8, 0xbfe4febe9d78bfe2,
           0xbfe62a01a1f3d55b, 0x3fc8bc5ee79abe1a, 0xbfddf8974f5225ba,
           0x3fd1c3f71445c41f);
  VLOAD_64(v8, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef);
  VLOAD_8(v0, 0xAA, 0xAA);
  double s64;
  BOX_DOUBLE_IN_DOUBLE(s64, 0xbfe7939b41dde2ca);
  asm volatile("vfrdiv.vf v8, v16, %[A], v0.t" ::[A] "f"(s64));
  VCMP_U64(9, v8, 0xdeadbeefdeadbeef, 0x3ff311018065f6d9, 0xdeadbeefdeadbeef,
            0x3ffc2f4e52d09cdc, 0xdeadbeefdeadbeef, 0xc00e96bb3d31a6bf,
            0xdeadbeefdeadbeef, 0x3ffcb7ff9f9e012e, 0xdeadbeefdeadbeef,
            0xbff34f3b04edfca3, 0xdeadbeefdeadbeef, 0x3ff1f7a1a50a4a30,
            0xdeadbeefdeadbeef, 0xc00e8015bc3ac807, 0xdeadbeefdeadbeef,
            0xc0053bcda6ee6a3a);
#endif
}

// Special values. Flags are checked after VCMP, so the vector op has retired.
void TEST_CASE4(void) {
  // 1 / {+0, -0, +inf, -inf, qNaN, 2}: divide-by-zero only
  CLEAR_FFLAGS;
  VSET(6, e16, m8);
  VLOAD_16(v16, pZero, mZeroh, pInfh, mInfh, qNaNh, 0x4000);
  float one16;
  BOX_HALF_IN_FLOAT(one16, 0x3c00);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(one16));
  VCMP_U16(10, v8, pInfh, mInfh, pZero, mZeroh, qNaNh, 0x3800);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(DZ);
  // 1 / {+0, -0, +inf, -inf, qNaN, 2}: divide-by-zero only
  CLEAR_FFLAGS;
  VSET(6, e32, m8);
  VLOAD_32(v16, pZero, mZerof, pInff, mInff, qNaNf, 0x40000000);
  float one32;
  BOX_FLOAT_IN_FLOAT(one32, 0x3f800000);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(one32));
  VCMP_U32(11, v8, pInff, mInff, pZero, mZerof, qNaNf, 0x3f000000);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(DZ);
#if ELEN == 64
  // 1 / {+0, -0, +inf, -inf, qNaN, 2}: divide-by-zero only
  CLEAR_FFLAGS;
  VSET(6, e64, m8);
  VLOAD_64(v16, pZero, mZerod, pInfd, mInfd, qNaNd, 0x4000000000000000);
  double one64;
  BOX_DOUBLE_IN_DOUBLE(one64, 0x3ff0000000000000);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(one64));
  VCMP_U64(12, v8, pInfd, mInfd, pZero, mZerod, qNaNd, 0x3fe0000000000000);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(DZ);
#endif

  // 0 / {+0, -0, 2, sNaN, +inf, -2}: 0/0 and sNaN are invalid, no divide-by-zero
  CLEAR_FFLAGS;
  VSET(6, e32, m8);
  VLOAD_32(v16, pZero, mZerof, 0x40000000, sNaNf, pInff, 0xc0000000);
  float zero32;
  BOX_FLOAT_IN_FLOAT(zero32, pZero);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(zero32));
  VCMP_U32(13, v8, qNaNf, qNaNf, pZero, qNaNf, pZero, mZerof);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(NV);

  // inf / {+inf, 2, -0, +0}: inf/inf is invalid, inf/0 is an exact inf (no DZ)
  CLEAR_FFLAGS;
  VSET(4, e32, m8);
  VLOAD_32(v16, pInff, 0x40000000, mZerof, pZero);
  float inf32;
  BOX_FLOAT_IN_FLOAT(inf32, pInff);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(inf32));
  VCMP_U32(14, v8, qNaNf, pInff, mInff, pInff);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(NV);

  // qNaN scalar: every result is the canonical NaN and no flag is raised
  CLEAR_FFLAGS;
  VSET(4, e32, m8);
  VLOAD_32(v16, 0x3f800000, pZero, pInff, 0xc0000000);
  float nan32;
  BOX_FLOAT_IN_FLOAT(nan32, qNaNf);
  asm volatile("vfrdiv.vf v8, v16, %[A]" ::[A] "f"(nan32));
  VCMP_U32(15, v8, qNaNf, qNaNf, qNaNf, qNaNf);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(0);
}

// Masked-off elements must not raise flags: the zeros and the sNaN are all
// in inactive slots, and the active quotients are exact.
void TEST_CASE5(void) {
  CLEAR_FFLAGS;
  VSET(8, e32, m8);
  VLOAD_32(v16, pZero, 0x40000000, pZero, 0x40800000,
           sNaNf, 0x41000000, pZero, 0x3f000000);
  VLOAD_32(v8, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef);
  VLOAD_8(v0, 0xAA);
  float one32;
  BOX_FLOAT_IN_FLOAT(one32, 0x3f800000);
  asm volatile("vfrdiv.vf v8, v16, %[A], v0.t" ::[A] "f"(one32));
  VCMP_U32(16, v8, 0xdeadbeef, 0x3f000000, 0xdeadbeef, 0x3e800000,
            0xdeadbeef, 0x3e000000, 0xdeadbeef, 0x40000000);
  // TODO(fflags): re-enable once the VFU status-masking fix is merged
  // CHECK_FFLAGS(0);
}

// vl = 10 with vd == vs2: the first 10 elements are overwritten in place,
// the tail keeps the original source values.
void TEST_CASE6(void) {
  VSET(16, e32, m8);
  VLOAD_32(v8, 0x3f0a055e, 0x3efc5cd4, 0xbf6bdaef, 0x3f25d3ec, 0x3f09d55a,
           0x3f05a3be, 0x3e91fdc3, 0x3d7ab163, 0xbe6e5e6a, 0x3f351a2b,
           0xbe7659a8, 0xbecd57ac, 0xbd5b54a8, 0x3f56aae8, 0x3e4975b1,
           0x3e9bc2a3);
  float s32;
  BOX_FLOAT_IN_FLOAT(s32, 0x3f3e2032);
  VSET(10, e32, m8);
  asm volatile("vfrdiv.vf v8, v8, %[A]" ::[A] "f"(s32));
  VSET(16, e32, m8);
  VCMP_U32(17, v8, 0x3fb0525e, 0x3fc0ddb9, 0xbf4e5d5f, 0x3f92c156, 0x3fb08fca,
            0x3fb61a38, 0x4026b212, 0x41422692, 0xc04c3049, 0x3f8660b0,
            0xbe7659a8, 0xbecd57ac, 0xbd5b54a8, 0x3f56aae8, 0x3e4975b1,
            0x3e9bc2a3);
}

// Original test case: LMUL = 2, vl = 4, unmasked and masked (mask = 0b1101)
void TEST_CASE7(void) {
  VSET(4, e32, m2);
  //            9.812321, 3.14159265, -6.432423, -190.53
  VLOAD_32(v4, 0x411cff44, 0x40490fdb, 0xc0cdd669, 0xc33e87ae);
  float s32;
  BOX_FLOAT_IN_FLOAT(s32, 0x40eaf601);  // 7.34253
  asm volatile("vfrdiv.vf v2, v4, %[A]" ::[A] "f"(s32));
  //               0.748297, 2.3372, -1.141487, -0.0385374
  VCMP_U32(18, v2, 0x3f3f9064, 0x401594ae, 0xbf921c42, 0xbd1dd963);

  VLOAD_32(v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef);
  VLOAD_8(v0, 0x0D);
  asm volatile("vfrdiv.vf v2, v4, %[A], v0.t" ::[A] "f"(s32));
  VCMP_U32(19, v2, 0x3f3f9064, 0xdeadbeef, 0xbf921c42, 0xbd1dd963);
}

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();
  CHANGE_RM(RM_RNE);

  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();
  TEST_CASE6();
  TEST_CASE7();

  EXIT_CHECK();
}
