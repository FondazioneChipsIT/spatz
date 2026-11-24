// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matheus Cavalcante <matheusd@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "vector_macros.h"

void TEST_CASE1(void) {
  VSET(16, e8, m1);
  VLOAD_8(v2, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0,
          0x0f, 0xff, 0x00, 0xf0, 0x0f);
  VLOAD_8(v3, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0,
          0x0f, 0xf2, 0x01, 0xf0, 0x0f);
  VCLEAR(v1);
  asm volatile("vmseq.vv v1, v2, v3");
  VSET(2, e8, m1);
  VCMP_U8(1, v1, 0xcc, 0xcc);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f,
           0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f);
  VLOAD_16(v3, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f,
           0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f);
  VCLEAR(v1);
  asm volatile("vmseq.vv v1, v2, v3");
  VSET(2, e8, m1);
  VCMP_U8(2, v1, 0xcc, 0xcc);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xffffffff, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_32(v6, 0xfff2ffff, 0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff,
           0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VCLEAR(v2);
  asm volatile("vmseq.vv v2, v4, v6");
  VSET(2, e8, m1);
  VCMP_U8(3, v1, 0xcc, 0xcc);

  #if ELEN == 64
    VSET(16, e64, m4);
    VLOAD_64(v4, 0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0000000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff,
             0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_64(v8, 0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff, 0x0100000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff,
             0x0100000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VCLEAR(v0);
    asm volatile("vmseq.vv v0, v4, v8");
    VSET(2, e8, m1);
    VCMP_U8(4, v0, 0xcc, 0xcc);
  #endif
};

void TEST_CASE2(void) {
  VSET(16, e8, m1);
  VLOAD_8(v2, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0,
          0x0f, 0xff, 0x00, 0xf0, 0x0f);
  VLOAD_8(v3, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0,
          0x0f, 0xf2, 0x01, 0xf0, 0x0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VCLEAR(v1);
  asm volatile("vmseq.vv v1, v2, v3, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(5, v1, 0xdd, 0xdd);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f,
           0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f);
  VLOAD_16(v3, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f,
           0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VCLEAR(v1);
  asm volatile("vmseq.vv v1, v2, v3, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(6, v1, 0xdd, 0xdd);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xffffffff, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_32(v6, 0xfff2ffff, 0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff,
           0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VCLEAR(v1);
  asm volatile("vmseq.vv v1, v4, v6, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(7, v1, 0xdd, 0xdd);

  #if ELEN == 64
    VSET(16, e64, m4);
    VLOAD_64(v4, 0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0000000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff,
             0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_64(v8, 0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff, 0x0100000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff,
             0x0100000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_8(v0, 0xaa, 0xaa);
    VCLEAR(v12);
    asm volatile("vmseq.vv v12, v4, v8, v0.t");
    VSET(2, e8, m1);
    VCMP_U8(8, v12, 0xdd, 0xdd);
  #endif
};

void TEST_CASE3(void) {
  const uint64_t scalar = 0x00000000ffffffff;

  VSET(16, e8, m1);
  VLOAD_8(v2, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0,
          0x0f, 0xff, 0x00, 0xf0, 0x0f);
  VCLEAR(v1);
  asm volatile("vmseq.vx v1, v2, %[A]" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(9, v1, 0x11, 0x11);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f,
           0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f);
  VCLEAR(v1);
  asm volatile("vmseq.vx v1, v2, %[A]" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(10, v1, 0x11, 0x11);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xffffffff, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VCLEAR(v2);
  asm volatile("vmseq.vx v2, v4, %[A]" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(11, v2, 0x11, 0x11);

//   64bit scalars are not passed correctly by the integer core
//   VSET(16, e64, m2);
//   VLOAD_64(v2, 0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
//            0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0000000000000000,
//            0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff,
//            0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
//            0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
//            0x0f0f0f0f0f0f0f0f);
//   VCLEAR(v0);
//   asm volatile("vmseq.vx v0, v2, %[A]" ::[A] "r"(scalar));
//   VSET(2, e8, m1);
//   VCMP_U8(12, v0, 0x00, 0x00);
};

void TEST_CASE4(void) {
  const uint64_t scalar = 0x00000000ffffffff;

  VSET(16, e8, m1);
  VLOAD_8(v2, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0,
          0x0f, 0xff, 0x00, 0xf0, 0x0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v1);
  asm volatile("vmseq.vx v1, v2, %[A], v0.t" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(13, v1, 0xff, 0xff);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f,
           0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v1);
  asm volatile("vmseq.vx v1, v2, %[A], v0.t" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(14, v1, 0xff, 0xff);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xffffffff, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v2);
  asm volatile("vmseq.vx v2, v4, %[A], v0.t" ::[A] "r"(scalar));
  VSET(2, e8, m1);
  VCMP_U8(15, v2, 0xff, 0xff);

//   64bit scalars are not passed correctly by the integer core
//   VSET(16, e64, m2);
//   VLOAD_64(v2, 0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
//            0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0000000000000000,
//            0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff,
//            0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
//            0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
//            0x0f0f0f0f0f0f0f0f);
//   VLOAD_8(v0, 0x10, 0x10);
//   VCLEAR(v4);
//   asm volatile("vmseq.vx v4, v2, %[A], v0.t" ::[A] "r"(scalar));
//   VSET(2, e8, m1);
//   VCMP_U8(16, v4, 0x00, 0x00);
};

void TEST_CASE5(void) {
  VSET(16, e8, m1);
  VLOAD_8(v2, 0x0f, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0xf0,
          0x0f, 0x0f, 0x00, 0xf0, 0x0f);
  VCLEAR(v1);
  asm volatile("vmseq.vi v1, v2, 15");
  VSET(2, e8, m1);
  VCMP_U8(17, v1, 0x99, 0x99);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0x000f, 0x0000, 0xf0f0, 0x0f0f, 0x000f, 0x0000, 0xf0f0, 0x0f0f,
           0x000f, 0x0000, 0xf0f0, 0x0f0f, 0x000f, 0x0000, 0xf0f0, 0x0f0f);
  VCLEAR(v1);
  asm volatile("vmseq.vi v1, v2, 15");
  VSET(2, e8, m1);
  VCMP_U8(18, v1, 0x11, 0x11);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0x0000000f, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VCLEAR(v2);
  asm volatile("vmseq.vi v2, v4, 15");
  VSET(2, e8, m1);
  VCMP_U8(19, v2, 0x11, 0x11);

  #if ELEN == 64
    VSET(16, e64, m4);
    VLOAD_64(v8, 0x000000000000000f, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0x000000000000000f, 0x0000000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0x000000000000000f,
             0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0x000000000000000f, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VCLEAR(v4);
    asm volatile("vmseq.vi v4, v8, 15");
    VSET(2, e8, m1);
    VCMP_U8(20, v4, 0x11, 0x11);
  #endif
};

void TEST_CASE6(void) {
  VSET(16, e8, m1);
  VLOAD_8(v2, 0x0f, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0xf0,
          0x0f, 0x0f, 0x00, 0xf0, 0x0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v1);
  asm volatile("vmseq.vi v1, v2, 15, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(21, v1, 0xff, 0xff);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0x000f, 0x0000, 0xf0f0, 0x0f0f, 0x000f, 0x0000, 0xf0f0, 0x0f0f,
           0x000f, 0x0000, 0xf0f0, 0x0f0f, 0x000f, 0x0000, 0xf0f0, 0x0f0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v1);
  asm volatile("vmseq.vi v1, v2, 15, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(22, v1, 0xff, 0xff);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0x0000000f, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0x0000000f, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_8(v0, 0x10, 0x10);
  VCLEAR(v2);
  asm volatile("vmseq.vi v2, v4, 15, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(23, v2, 0xff, 0xff);

  #if ELEN == 64
    VSET(16, e64, m4);
    VLOAD_64(v4, 0x000000000000000f, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0x000000000000000f, 0x0000000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0x000000000000000f,
             0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0x000000000000000f, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_8(v0, 0x10, 0x10);
    VCLEAR(v8);
    asm volatile("vmseq.vi v8, v4, 15, v0.t");
    VSET(2, e8, m1);
    VCMP_U8(24, v8, 0xff, 0xff);
  #endif
};

void TEST_CASE7(void) {
  VSET(16, e8, m1);
  VLOAD_8(v2, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0,
          0x0f, 0xff, 0x00, 0xf0, 0x0f);
  VLOAD_8(v3, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0, 0x0f, 0xf2, 0x01, 0xf0,
          0x0f, 0xf2, 0x01, 0xf0, 0x0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VSET_TO_ONES(v1);
  asm volatile("vmseq.vv v1, v2, v3, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(25, v1, 0xdd, 0xdd);

  VSET(16, e16, m1);
  VLOAD_16(v2, 0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f,
           0xffff, 0x0000, 0xf0f0, 0x0f0f, 0xffff, 0x0000, 0xf0f0, 0x0f0f);
  VLOAD_16(v3, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f,
           0xf2ff, 0x0100, 0xf0f0, 0x0f0f, 0xf2ff, 0x0100, 0xf0f0, 0x0f0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VSET_TO_ONES(v1);
  asm volatile("vmseq.vv v1, v2, v3, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(26, v1, 0xdd, 0xdd);

  VSET(16, e32, m2);
  VLOAD_32(v4, 0xffffffff, 0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff,
           0x00000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xffffffff, 0x00000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_32(v6, 0xfff2ffff, 0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff,
           0x01000000, 0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000,
           0xf0f0f0f0, 0x0f0f0f0f, 0xfff2ffff, 0x01000000, 0xf0f0f0f0,
           0x0f0f0f0f);
  VLOAD_8(v0, 0xaa, 0xaa);
  VSET_TO_ONES(v2);
  asm volatile("vmseq.vv v2, v4, v6, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(27, v1, 0xdd, 0xdd);

  #if ELEN == 64
    VSET(16, e64, m4);
    VLOAD_64(v4, 0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0000000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff,
             0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xffffffffffffffff, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_64(v8, 0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff, 0x0100000000000000,
             0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f, 0xfff2ffffffffffff,
             0x0100000000000000, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f,
             0xfff2ffffffffffff, 0x0100000000000000, 0xf0f0f0f0f0f0f0f0,
             0x0f0f0f0f0f0f0f0f);
    VLOAD_8(v0, 0xaa, 0xaa);
    VSET_TO_ONES(v12);
    asm volatile("vmseq.vv v12, v4, v8, v0.t");
    VSET(2, e8, m1);
    VCMP_U8(28, v12, 0xdd, 0xdd);
  #endif
};

// Mask-register tests at full LMUL=8 width. All sizes derive from VLENB
// (read at runtime), so they run on every VLEN:
//   mask_bytes = VLENB        (one full vector register = the mask of VLMAX
//                              e8/m8 elements)
//   VLMAX e8/m8 = 8 * VLENB elements
// On VLEN=512 with 256-bit VRF words the mask spans two VRF words, and the
// tests below cover the word-1 cases. With 256-bit VRF words, VLEN=256 has
// just one word per register, and the same checks still have to pass.
static unsigned int mask_bytes;
static uint8_t *mask_buf;
static uint8_t *mask_gold;

// Fill v1 (the mask destination) with `fill` across the whole register
#define PREFILL_V1(fill)                                                       \
  do {                                                                         \
    VSET(mask_bytes, e8, m1);                                                  \
    asm volatile("vmv.v.x v1, %0" ::"r"((uint64_t)(fill)));                    \
  } while (0)

// Store v1 (whole register) into mask_buf
#define DUMP_V1()                                                              \
  do {                                                                         \
    VSET(mask_bytes, e8, m1);                                                  \
    asm volatile("vse8.v v1, (%0)" ::"r"(mask_buf) : "memory");                \
  } while (0)

// Fill the first `nbytes` of mask_gold with `val`
static void fill_gold(uint8_t val, unsigned int nbytes) {
  for (unsigned int i = 0; i < nbytes; i++)
    mask_gold[i] = val;
}

// Control: vl = VLMAX/2 (256 on VLEN=512) fits in the lower half of v1
void TEST_CASE8(void) {
  const unsigned int vl = mask_bytes * 4;
  PREFILL_V1(0x00);
  VSET(vl, e8, m8);
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  asm volatile("vmseq.vv v1, v8, v16");
  DUMP_V1();
  fill_gold(0xff, vl / 8);
  VMCMP(uint8_t, %hhu, 29, mask_buf, mask_gold, vl / 8);
}

// Smallest crossing of the half-register boundary: vl = VLMAX/2 + 8
// (264 on VLEN=512 -> 33 mask bytes, byte 32 lives in VRF word 1)
void TEST_CASE9(void) {
  const unsigned int vl = mask_bytes * 4 + 8;
  PREFILL_V1(0x00);
  VSET(vl, e8, m8);
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  asm volatile("vmseq.vv v1, v8, v16");
  DUMP_V1();
  fill_gold(0xff, vl / 8);
  VMCMP(uint8_t, %hhu, 30, mask_buf, mask_gold, vl / 8);
}

// Full register: vl = VLMAX, all equal -> mask_bytes of 0xff
void TEST_CASE10(void) {
  const unsigned int vl = mask_bytes * 8;
  PREFILL_V1(0x00);
  VSET(vl, e8, m8);
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  asm volatile("vmseq.vv v1, v8, v16");
  DUMP_V1();
  fill_gold(0xff, mask_bytes);
  VMCMP(uint8_t, %hhu, 31, mask_buf, mask_gold, mask_bytes);
}

// Masked, mask-undisturbed (vma=0): inactive bits must keep the old vd value
// in every VRF word, and the upper v0 bits must come from v0 (not v1).
// v0 = 0x55.., old vd = 0xf0.., all compares true:
//   (0x55 & 0xff) | (0xf0 & ~0x55) = 0xf5
void TEST_CASE11(void) {
  PREFILL_V1(0xf0);
  VSET(mask_bytes, e8, m1);
  asm volatile("vmv.v.x v0, %0" ::"r"((uint64_t)0x55));
  {
    unsigned int vl;
    asm volatile("vsetvli %0, %1, e8, m8, ta, mu"
                 : "=r"(vl)
                 : "r"(mask_bytes * 8));
  }
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  asm volatile("vmseq.vv v1, v8, v16, v0.t");
  DUMP_V1();
  fill_gold(0xf5, mask_bytes);
  VMCMP(uint8_t, %hhu, 32, mask_buf, mask_gold, mask_bytes);
}

// Non-uniform data, vl = VLMAX: checks bit placement in the upper word, not
// just that "something" got written. Element i is equal iff i % 5 == 0; that
// pattern is not periodic in 256, so a wrong word offset gives wrong bytes.
// Mask bytes repeat every 5: 0x21, 0x84, 0x10, 0x42, 0x08.
void TEST_CASE12(void) {
  const unsigned int vl = mask_bytes * 8;
  uint8_t *a = (uint8_t *)snrt_l1alloc(vl);
  uint8_t *b = (uint8_t *)snrt_l1alloc(vl);
  for (unsigned int i = 0; i < vl; i++) {
    a[i] = (uint8_t)i;
    b[i] = (i % 5 == 0) ? a[i] : (uint8_t)(a[i] ^ 0x1);
  }
  static const uint8_t pattern[5] = {0x21, 0x84, 0x10, 0x42, 0x08};
  for (unsigned int i = 0; i < mask_bytes; i++)
    mask_gold[i] = pattern[i % 5];

  PREFILL_V1(0x00);
  VSET(vl, e8, m8);
  asm volatile("vle8.v v8,  (%0)" ::"r"(a) : "memory");
  asm volatile("vle8.v v16, (%0)" ::"r"(b) : "memory");
  asm volatile("vmseq.vv v1, v8, v16");
  DUMP_V1();
  VMCMP(uint8_t, %hhu, 33, mask_buf, mask_gold, mask_bytes);
}

// Control at SEW=16/LMUL=8: vl = VLMAX e16/m8 = 4 * VLENB
// (256 on VLEN=512 -> 32 mask bytes, fits in one word)
void TEST_CASE13(void) {
  const unsigned int vl = mask_bytes * 4;
  PREFILL_V1(0x00);
  VSET(vl, e16, m8);
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  asm volatile("vmseq.vv v1, v8, v16");
  DUMP_V1();
  fill_gold(0xff, vl / 8);
  VMCMP(uint8_t, %hhu, 34, mask_buf, mask_gold, vl / 8);
}

// Back-to-back masked compares, mask-undisturbed (vma=0), different vd.
// Each compare's inactive mask bits must come from its OWN old vd. The VFU
// keeps the old vd of a masked compare in one register (cmp_mask_dst_q); if
// the second compare reads its vd before the first one has written back, the
// first one merges the second one's old vd.
// v0 = 0x55 (even elements active), all compares true:
//   v1: old 0xf0 -> 0x55 | (0xf0 & 0xaa) = 0xf5
//   v2: old 0x0f -> 0x55 | (0x0f & 0xaa) = 0x5f
// Hazard: v1 = 0x5f (got v2's old vd).
void TEST_CASE14(void) {
  VSET(2, e8, m1);
  asm volatile("vmv.v.x v0, %0" ::"r"(0x55));
  asm volatile("vmv.v.x v1, %0" ::"r"(0xf0));
  asm volatile("vmv.v.x v2, %0" ::"r"(0x0f));
  {
    unsigned int vl;
    asm volatile("vsetvli %0, %1, e32, m2, ta, mu" : "=r"(vl) : "r"(16));
  }
  asm volatile("vmv.v.i v8, 3");
  asm volatile("vmv.v.i v16, 3");
  // Single asm block: the two compares must issue back to back
  asm volatile("vmseq.vv v1, v8, v16, v0.t\n\t"
               "vmseq.vv v2, v8, v16, v0.t");
  VSET(2, e8, m1);
  VCMP_U8(35, v1, 0xf5, 0xf5);
  VCMP_U8(36, v2, 0x5f, 0x5f);
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

  asm volatile("csrr %0, vlenb" : "=r"(mask_bytes));
  mask_buf = (uint8_t *)snrt_l1alloc(mask_bytes);
  mask_gold = (uint8_t *)snrt_l1alloc(mask_bytes);
  TEST_CASE8();
  TEST_CASE9();
  TEST_CASE10();
  TEST_CASE11();
  TEST_CASE12();
  TEST_CASE13();
  TEST_CASE14();

  EXIT_CHECK();
}