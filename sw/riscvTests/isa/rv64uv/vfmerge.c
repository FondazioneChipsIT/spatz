// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matheus Cavalcante <matheusd@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>
//         Matteo Perotti <mperotti@student.ethz.ch>

#include "float_macros.h"
#include "vector_macros.h"

void TEST_CASE1(void) {
  VSET(16, e16, m2);
  //              -0.1481, -0.1797, -0.5454,  0.3228,  0.3237, -0.7212, -0.5195,
  //              -0.4500,  0.2681,  0.7300,  0.5059,  0.5830,  0.3198, -0.1713,
  //              -0.6431,  0.4841
  VLOAD_16(v4, 0xb0bd, 0xb1c0, 0xb85d, 0x352a, 0x352e, 0xb9c5, 0xb828, 0xb733,
           0x344a, 0x39d7, 0x380c, 0x38aa, 0x351e, 0xb17b, 0xb925, 0x37bf);
  float fscalar_16;
  //                            -0.9380
  BOX_HALF_IN_FLOAT(fscalar_16, 0xbb81);
  VLOAD_8(v0, 0x0F, 0xAA);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fscalar_16));
  //               -0.9380, -0.9380, -0.9380, -0.9380,  0.3237, -0.7212,
  //               -0.5195, -0.4500,  0.2681, -0.9380,  0.5059, -0.9380, 0.3198,
  //               -0.9380, -0.6431, -0.9380
  VCMP_U16(1, v2, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0x352e, 0xb9c5, 0xb828,
           0xb733, 0x344a, 0xbb81, 0x380c, 0xbb81, 0x351e, 0xbb81, 0xb925,
           0xbb81);

  VSET(16, e32, m2);
  //               0.86539453, -0.53925377, -0.47128764,  0.99265540,
  //               0.32128176, -0.47335613, -0.30028856,  0.44394016,
  //               -0.72540921, -0.26464799,  0.77351445, -0.21725702,
  //               -0.25191557, -0.53123665,  0.80404943,  0.81841671
  VLOAD_32(v4, 0x3f5d8a7f, 0xbf0a0c89, 0xbef14c9d, 0x3f7e1eaa, 0x3ea47f0b,
           0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbe877ff1,
           0x3f46050b, 0xbe5e78a0, 0xbe80fb14, 0xbf07ff20, 0x3f4dd62f,
           0x3f5183c2);
  float fscalar_32;
  //                             -0.96056187
  BOX_FLOAT_IN_FLOAT(fscalar_32, 0xbf75e762);
  VLOAD_8(v0, 0x0F, 0xAA);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fscalar_32));
  //               -0.96056187, -0.96056187, -0.96056187, -0.96056187,
  //               0.32128176, -0.47335613, -0.30028856,  0.44394016,
  //               -0.72540921, -0.96056187,  0.77351445, -0.96056187,
  //               -0.25191557, -0.96056187,  0.80404943, -0.96056187
  VCMP_U32(2, v2, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0x3ea47f0b,
           0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbf75e762,
           0x3f46050b, 0xbf75e762, 0xbe80fb14, 0xbf75e762, 0x3f4dd62f,
           0xbf75e762);

  VSET(16, e64, m2);
  //               -0.3488917150781869, -0.4501495513738740, 0.8731197104152684,
  //               0.3256432550932964,  0.6502591178769535, -0.3169358689246526,
  //               -0.5396694979141685, -0.5417807430937591,
  //               -0.7971574213160249, -0.1764794100111047, 0.3564275916066595,
  //               -0.3754449946313438,  0.6580947137446858,
  //               -0.3328857144699515,  0.1761214464164236,  0.1429774118511240
  VLOAD_64(v4, 0xbfd6543dea86cb60, 0xbfdccf40105d6e5c, 0x3febf098bf37400c,
           0x3fd4d756ceb279f4, 0x3fe4ceec35a6a266, 0xbfd448ad61fd7c88,
           0xbfe144f8f7861540, 0xbfe1564491a616b8, 0xbfe9825047ca1cd6,
           0xbfc696e097352100, 0x3fd6cfb5ac55edec, 0xbfd8074a7158dd78,
           0x3fe50f1ca5268668, 0xbfd54dffe23d0eec, 0x3fc68b25c63dcaf0,
           0x3fc24d1575fbd080);
  double dscalar_64;
  //                               0.9108707261227378
  BOX_DOUBLE_IN_DOUBLE(dscalar_64, 0x3fed25da5d7296fe);
  VLOAD_8(v0, 0x0F, 0xAA);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(dscalar_64));
  //                0.9108707261227378,  0.9108707261227378, 0.9108707261227378,
  //                0.9108707261227378,  0.6502591178769535,
  //                -0.3169358689246526, -0.5396694979141685,
  //                -0.5417807430937591  -0.7971574213160249,
  //                0.9108707261227378,  0.3564275916066595, 0.9108707261227378,
  //                0.6580947137446858,  0.9108707261227378, 0.1761214464164236,
  //                0.9108707261227378
  VCMP_U64(3, v2, 0x3fed25da5d7296fe, 0x3fed25da5d7296fe, 0x3fed25da5d7296fe,
           0x3fed25da5d7296fe, 0x3fe4ceec35a6a266, 0xbfd448ad61fd7c88,
           0xbfe144f8f7861540, 0xbfe1564491a616b8, 0xbfe9825047ca1cd6,
           0x3fed25da5d7296fe, 0x3fd6cfb5ac55edec, 0x3fed25da5d7296fe,
           0x3fe50f1ca5268668, 0x3fed25da5d7296fe, 0x3fc68b25c63dcaf0,
           0x3fed25da5d7296fe);
};

// Same data as TEST_CASE1 at e16, in its own function so a failure at one SEW
// does not hide the others. vd is pre-filled with a sentinel: every element
// (active or not) must be overwritten with either the scalar or vs2.
void TEST_CASE2(void) {
  VSET(16, e16, m2);
  VLOAD_16(v4, 0xb0bd, 0xb1c0, 0xb85d, 0x352a, 0x352e, 0xb9c5, 0xb828, 0xb733,
           0x344a, 0x39d7, 0x380c, 0x38aa, 0x351e, 0xb17b, 0xb925, 0x37bf);
  VLOAD_16(v2, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead,
           0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead, 0xdead);
  VLOAD_8(v0, 0x0F, 0xAA);
  float fs;
  BOX_HALF_IN_FLOAT(fs, 0xbb81);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U16(4, v2, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0x352e, 0xb9c5, 0xb828, 0xb733,
            0x344a, 0xbb81, 0x380c, 0xbb81, 0x351e, 0xbb81, 0xb925, 0xbb81);
}

// Same data as TEST_CASE1 at e32, in its own function so a failure at one SEW
// does not hide the others. vd is pre-filled with a sentinel: every element
// (active or not) must be overwritten with either the scalar or vs2.
void TEST_CASE3(void) {
  VSET(16, e32, m2);
  VLOAD_32(v4, 0x3f5d8a7f, 0xbf0a0c89, 0xbef14c9d, 0x3f7e1eaa, 0x3ea47f0b,
           0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbe877ff1,
           0x3f46050b, 0xbe5e78a0, 0xbe80fb14, 0xbf07ff20, 0x3f4dd62f,
           0x3f5183c2);
  VLOAD_32(v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef);
  VLOAD_8(v0, 0x0F, 0xAA);
  float fs;
  BOX_FLOAT_IN_FLOAT(fs, 0xbf75e762);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U32(5, v2, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0x3ea47f0b,
            0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbf75e762,
            0x3f46050b, 0xbf75e762, 0xbe80fb14, 0xbf75e762, 0x3f4dd62f,
            0xbf75e762);
}

// Same data as TEST_CASE1 at e64, in its own function so a failure at one SEW
// does not hide the others. vd is pre-filled with a sentinel: every element
// (active or not) must be overwritten with either the scalar or vs2.
void TEST_CASE4(void) {
#if ELEN == 64
  VSET(16, e64, m2);
  VLOAD_64(v4, 0xbfd6543dea86cb60, 0xbfdccf40105d6e5c, 0x3febf098bf37400c,
           0x3fd4d756ceb279f4, 0x3fe4ceec35a6a266, 0xbfd448ad61fd7c88,
           0xbfe144f8f7861540, 0xbfe1564491a616b8, 0xbfe9825047ca1cd6,
           0xbfc696e097352100, 0x3fd6cfb5ac55edec, 0xbfd8074a7158dd78,
           0x3fe50f1ca5268668, 0xbfd54dffe23d0eec, 0x3fc68b25c63dcaf0,
           0x3fc24d1575fbd080);
  VLOAD_64(v2, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef);
  VLOAD_8(v0, 0x0F, 0xAA);
  double fs;
  BOX_DOUBLE_IN_DOUBLE(fs, 0x3fed25da5d7296fe);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U64(6, v2, 0x3fed25da5d7296fe, 0x3fed25da5d7296fe, 0x3fed25da5d7296fe,
            0x3fed25da5d7296fe, 0x3fe4ceec35a6a266, 0xbfd448ad61fd7c88,
            0xbfe144f8f7861540, 0xbfe1564491a616b8, 0xbfe9825047ca1cd6,
            0x3fed25da5d7296fe, 0x3fd6cfb5ac55edec, 0x3fed25da5d7296fe,
            0x3fe50f1ca5268668, 0x3fed25da5d7296fe, 0x3fc68b25c63dcaf0,
            0x3fed25da5d7296fe);
#endif
}

// Mask all ones -> every element is the scalar (vfmv.v.f behaviour).
// Mask all zeros -> vd is a plain copy of vs2.
void TEST_CASE5(void) {
  VSET(16, e32, m2);
  VLOAD_32(v4, 0x3f5d8a7f, 0xbf0a0c89, 0xbef14c9d, 0x3f7e1eaa, 0x3ea47f0b,
           0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbe877ff1,
           0x3f46050b, 0xbe5e78a0, 0xbe80fb14, 0xbf07ff20, 0x3f4dd62f,
           0x3f5183c2);
  float fs;
  BOX_FLOAT_IN_FLOAT(fs, 0xbf75e762);
  VLOAD_32(v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef);
  VLOAD_8(v0, 0xFF, 0xFF);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U32(7, v2, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762,
            0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762,
            0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762, 0xbf75e762,
            0xbf75e762);

  VLOAD_32(v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef);
  VLOAD_8(v0, 0x00, 0x00);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U32(8, v2, 0x3f5d8a7f, 0xbf0a0c89, 0xbef14c9d, 0x3f7e1eaa, 0x3ea47f0b,
            0xbef25bbc, 0xbe99bf6c, 0x3ee34c20, 0xbf39b46b, 0xbe877ff1,
            0x3f46050b, 0xbe5e78a0, 0xbe80fb14, 0xbf07ff20, 0x3f4dd62f,
            0x3f5183c2);
}

// Special values are moved bit-exact: no NaN canonicalisation, no quieting of
// signalling NaNs, NaN payloads, -0 and subnormals preserved, on both the
// scalar (sNaN with payload) and the vs2 side. Mask 0xA5 -> elements 0,2,5,7 active.
void TEST_CASE6(void) {
  VSET(8, e16, m2);
  VLOAD_16(v4, sNaNh, qNaNh, mZeroh, pInfh, mInfh, 0x0001, 0x83ff, 0x7e55);
  VLOAD_8(v0, 0xA5);
  float fs16;
  BOX_HALF_IN_FLOAT(fs16, 0x7d23);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs16));
  VCMP_U16(9, v2, 0x7d23, qNaNh, 0x7d23, pInfh, mInfh, 0x7d23, 0x83ff, 0x7d23);
  VSET(8, e32, m2);
  VLOAD_32(v4, sNaNf, qNaNf, mZerof, pInff, mInff, 0x00000001, 0x807fffff, 0x7fc12345);
  VLOAD_8(v0, 0xA5);
  float fs32;
  BOX_FLOAT_IN_FLOAT(fs32, 0x7f812345);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs32));
  VCMP_U32(10, v2, 0x7f812345, qNaNf, 0x7f812345, pInff, mInff, 0x7f812345, 0x807fffff, 0x7f812345);
}

// vd == vs2: merge in place.
void TEST_CASE7(void) {
  VSET(16, e16, m2);
  VLOAD_16(v4, 0xb0bd, 0xb1c0, 0xb85d, 0x352a, 0x352e, 0xb9c5, 0xb828, 0xb733,
           0x344a, 0x39d7, 0x380c, 0x38aa, 0x351e, 0xb17b, 0xb925, 0x37bf);
  VLOAD_8(v0, 0x0F, 0xAA);
  float fs;
  BOX_HALF_IN_FLOAT(fs, 0xbb81);
  asm volatile("vfmerge.vfm v4, v4, %[A], v0" ::[A] "f"(fs));
  VCMP_U16(11, v4, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0x352e, 0xb9c5, 0xb828, 0xb733,
            0x344a, 0xbb81, 0x380c, 0xbb81, 0x351e, 0xbb81, 0xb925, 0xbb81);
}

// vl = 40 at e32/m8: the mask is consumed across several VRF words and five
// mask bytes, the last word is only partially active, and the 8 tail elements
// (40..47) must keep the sentinel. vs2[i] = i + 1.0, scalar = -1.0.
void TEST_CASE8(void) {
  VSET(48, e32, m8);
  VLOAD_32(v8, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef);
  VSET(40, e32, m8);
  VLOAD_32(v16, 0x3f800000, 0x40000000, 0x40400000, 0x40800000, 0x40a00000,
           0x40c00000, 0x40e00000, 0x41000000, 0x41100000, 0x41200000,
           0x41300000, 0x41400000, 0x41500000, 0x41600000, 0x41700000,
           0x41800000, 0x41880000, 0x41900000, 0x41980000, 0x41a00000,
           0x41a80000, 0x41b00000, 0x41b80000, 0x41c00000, 0x41c80000,
           0x41d00000, 0x41d80000, 0x41e00000, 0x41e80000, 0x41f00000,
           0x41f80000, 0x42000000, 0x42040000, 0x42080000, 0x420c0000,
           0x42100000, 0x42140000, 0x42180000, 0x421c0000, 0x42200000);
  VLOAD_8(v0, 0x0F, 0xAA, 0x55, 0xF0, 0x3C);
  float fs;
  BOX_FLOAT_IN_FLOAT(fs, 0xbf800000);
  asm volatile("vfmerge.vfm v8, v16, %[A], v0" ::[A] "f"(fs));
  VSET(48, e32, m8);
  VCMP_U32(12, v8, 0xbf800000, 0xbf800000, 0xbf800000, 0xbf800000, 0x40a00000,
            0x40c00000, 0x40e00000, 0x41000000, 0x41100000, 0xbf800000,
            0x41300000, 0xbf800000, 0x41500000, 0xbf800000, 0x41700000,
            0xbf800000, 0xbf800000, 0x41900000, 0xbf800000, 0x41a00000,
            0xbf800000, 0x41b00000, 0xbf800000, 0x41c00000, 0x41c80000,
            0x41d00000, 0x41d80000, 0x41e00000, 0xbf800000, 0xbf800000,
            0xbf800000, 0xbf800000, 0x42040000, 0x42080000, 0xbf800000,
            0xbf800000, 0xbf800000, 0xbf800000, 0x421c0000, 0x42200000,
            0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
            0xdeadbeef, 0xdeadbeef, 0xdeadbeef);
}

// vl = 0: the instruction must not write vd at all. The AVL goes through a
// volatile so the compiler cannot use x0 (which would mean vl = VLMAX).
void TEST_CASE9(void) {
  VSET(16, e32, m2);
  VLOAD_32(v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
           0xdeadbeef);
  VLOAD_8(v0, 0xFF, 0xFF);
  float fs;
  BOX_FLOAT_IN_FLOAT(fs, 0xbf75e762);
  volatile int avl = 0;
  int avl0 = avl;
  VSET(avl0, e32, m2);
  unsigned int vl;
  asm volatile("csrr %0, vl" : "=r"(vl));
  XCMP(13, vl, 0);
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs));
  VSET(16, e32, m2);
  VCMP_U32(14, v2, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
            0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
            0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef,
            0xdeadbeef);
}

// RVV: a scalar FP operand that is not properly NaN-boxed must be read as the
// canonical NaN. Here the upper bits of f[rs1] are zero instead of all ones.
// This checks the scalar path (FPU sequencer / VFU), not the merge itself;
// mask 0x0F -> elements 0..3 take the (canonical-NaN) scalar.
void TEST_CASE10(void) {
  VSET(8, e16, m2);
  VLOAD_16(v4, 0xb0bd, 0xb1c0, 0xb85d, 0x352a, 0x352e, 0xb9c5, 0xb828, 0xb733);
  VLOAD_8(v0, 0x0F);
  float fs16;
  BOX_FLOAT_IN_FLOAT(fs16, 0x00003c00);  // 1.0 in FP16, but upper half not all ones
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs16));
  VCMP_U16(15, v2, qNaNh, qNaNh, qNaNh, qNaNh, 0x352e, 0xb9c5, 0xb828, 0xb733);
#if ELEN == 64
  VSET(8, e32, m2);
  VLOAD_32(v4, 0x3f5d8a7f, 0xbf0a0c89, 0xbef14c9d, 0x3f7e1eaa, 0x3ea47f0b,
           0xbef25bbc, 0xbe99bf6c, 0x3ee34c20);
  VLOAD_8(v0, 0x0F);
  double fs32;
  BOX_DOUBLE_IN_DOUBLE(fs32, 0x000000003f800000);  // 1.0f, upper word not all ones
  asm volatile("vfmerge.vfm v2, v4, %[A], v0" ::[A] "f"(fs32));
  VCMP_U32(16, v2, qNaNf, qNaNf, qNaNf, qNaNf, 0x3ea47f0b, 0xbef25bbc, 0xbe99bf6c, 0x3ee34c20);
#endif
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
  TEST_CASE6();
  TEST_CASE7();
  TEST_CASE8();
  TEST_CASE9();
  //TEST_CASE10();

  EXIT_CHECK();
}
