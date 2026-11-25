// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51

#include "vector_macros.h"

//**********Checking functionality of vle64 ********//
void TEST_CASE1(void) {
  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                              0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019};

  asm volatile("vle64.v v1, (%0)" ::"r"(INP1) : "memory");
  VCMP_U64(1, v1, 0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                  0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019);
}

//*******Checking functionality of vle64 with different values of masking register******//
void TEST_CASE2(void) {
  VSET(1, e8, m1);
  VCLEAR(v0);
  VLOAD_8(v0, 0x0F);

  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE00e0, 0xDEADBEEFCAFE00d3,
                              0xDEADBEEFCAFE0040, 0xDEADBEEFCAFE00d1};
  VCLEAR(v1);

  asm volatile("vle64.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  VCMP_U64(2, v1, 0xDEADBEEFCAFE00e0, 0xDEADBEEFCAFE00d3,
                  0xDEADBEEFCAFE0040, 0xDEADBEEFCAFE00d1);
}

void TEST_CASE3(void) {
  VSET(1, e8, m1);
  VCLEAR(v0);
  VLOAD_8(v0, 0x00);

  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE00e0, 0xDEADBEEFCAFE00d3,
                              0xDEADBEEFCAFE0040, 0xDEADBEEFCAFE00d1};
  VCLEAR(v1);
  VLOAD_64(v1, 1, 2, 3, 4);
  asm volatile("vle64.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  VCMP_U64(3, v1, 1, 2, 3, 4);
}

void TEST_CASE4(void) {
  VSET(1, e8, m1);
  VCLEAR(v0);
  VLOAD_8(v0, 0x0A);

  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE00e0, 0xDEADBEEFCAFE00d3,
                              0xDEADBEEFCAFE0040, 0xDEADBEEFCAFE00d1};
  VCLEAR(v1);
  VLOAD_64(v1, 1, 2, 3, 4);
  VSET(4, e64, m1);
  asm volatile("vle64.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  VCMP_U64(4, v1, 1, 0xDEADBEEFCAFE00d3, 3, 0xDEADBEEFCAFE00d1);
}

void TEST_CASE5(void) {
  VSET(1, e8, m1);
  VCLEAR(v0);
  VLOAD_8(v0, 0x0A);

  VSET(4, e64, m8);
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE00e0, 0xDEADBEEFCAFE00d3,
                              0xDEADBEEFCAFE0040, 0xDEADBEEFCAFE00d1};
  VCLEAR(v8);
  VLOAD_64(v8, 1, 2, 3, 4);
  VSET(4, e64, m8);
  asm volatile("vle64.v v8, (%0), v0.t" ::"r"(INP1) : "memory");
  VCMP_U64(5, v8, 1, 0xDEADBEEFCAFE00d3, 3, 0xDEADBEEFCAFE00d1);
}

//*******Checking functionality of vle64 with large number of elements******//
void TEST_CASE6(void) {
  VSET(16, e64, m8);
  volatile uint64_t INP1[] = {
      0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003,
      0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 0x000000000000000A, 0x000000000000000B,
      0x000000000000000C, 0x000000000000000D, 0x000000000000000E, 0x000000000000000F};

  asm volatile("vle64.v v8, (%0)" ::"r"(INP1) : "memory");

  VCMP_U64(6, v8,
      0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003,
      0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 0x000000000000000A, 0x000000000000000B,
      0x000000000000000C, 0x000000000000000D, 0x000000000000000E, 0x000000000000000F);
}

void TEST_CASE7(void) {
  VSET(32, e64, m8);
  volatile uint64_t INP1[] = {
      0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003,
      0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 0x000000000000000A, 0x000000000000000B,
      0x000000000000000C, 0x000000000000000D, 0x000000000000000E, 0x000000000000000F,

      0x0000000000000010, 0x0000000000000011, 0x0000000000000012, 0x0000000000000013,
      0x0000000000000014, 0x0000000000000015, 0x0000000000000016, 0x0000000000000017,
      0x0000000000000018, 0x0000000000000019, 0x000000000000001A, 0x000000000000001B,
      0x000000000000001C, 0x000000000000001D, 0x000000000000001E, 0x000000000000001F};

  asm volatile("vle64.v v8, (%0)" ::"r"(INP1) : "memory");

  VCMP_U64(7, v8,
      0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003,
      0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 0x000000000000000A, 0x000000000000000B,
      0x000000000000000C, 0x000000000000000D, 0x000000000000000E, 0x000000000000000F,

      0x0000000000000010, 0x0000000000000011, 0x0000000000000012, 0x0000000000000013,
      0x0000000000000014, 0x0000000000000015, 0x0000000000000016, 0x0000000000000017,
      0x0000000000000018, 0x0000000000000019, 0x000000000000001A, 0x000000000000001B,
      0x000000000000001C, 0x000000000000001D, 0x000000000000001E, 0x000000000000001F);
}

void TEST_CASE8(void) {
  VSET(4, e8, m1);
  VCLEAR(v0);
  VLOAD_8(v0, 0xAB, 0xAB, 0xAA, 0xAA);

  VSET(32, e64, m8);
  volatile uint64_t INP1[] = {
      0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003,
      0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 0x000000000000000A, 0x000000000000000B,
      0x000000000000000C, 0x000000000000000D, 0x000000000000000E, 0x000000000000000F,

      0x0000000000000010, 0x0000000000000011, 0x0000000000000012, 0x0000000000000013,
      0x0000000000000014, 0x0000000000000015, 0x0000000000000016, 0x0000000000000017,
      0x0000000000000018, 0x0000000000000019, 0x000000000000001A, 0x000000000000001B,
      0x000000000000001C, 0x000000000000001D, 0x000000000000001E, 0x000000000000001F};

  VCLEAR(v8);

  VLOAD_64(v8,
      99,  99,  99,  99,  99,  99,  99,  99,
      99,  99,  99,  99,  99,  99,  99,  99,
      99,  99,  99,  99,  99,  99,  99,  99,
      99,  99,  99,  99,  99,  99,  99,  99);

  VLOAD_8(v0, 0xAB, 0xAB, 0xAA, 0xAA);

  asm volatile("vle64.v v8, (%0), v0.t" ::"r"(INP1) : "memory");

  VCMP_U64(8, v8,
      0x0000000000000000, 0x0000000000000001, 99, 0x0000000000000003,
      99, 0x0000000000000005, 99, 0x0000000000000007,
      0x0000000000000008, 0x0000000000000009, 99, 0x000000000000000B,
      99, 0x000000000000000D, 99, 0x000000000000000F,

      99, 0x0000000000000011, 99, 0x0000000000000013,
      99, 0x0000000000000015, 99, 0x0000000000000017,
      99, 0x0000000000000019, 99, 0x000000000000001B,
      99, 0x000000000000001D, 99, 0x000000000000001F);
}

void TEST_CASE9(void) {

  volatile uint64_t BUF[] = {
      0xf9aa71f0c394bbd3, 0x8913984898951989, 0x99991348a9f38cd1, 0x9fa831c7a11a9384,
      0x3819759853987548, 0x1111111111111111, 0x2222222222222222, 0x3333333333333333,
      0x4444444444444444};

  VSET(9, e64, m4);
  VCLEAR(v8);
  asm volatile("vle64.v v8, (%0)" ::"r"(BUF));
  VCMP_U64(9, v8, 0xf9aa71f0c394bbd3, 0x8913984898951989, 0x99991348a9f38cd1, 0x9fa831c7a11a9384,
                  0x3819759853987548, 0x1111111111111111, 0x2222222222222222, 0x3333333333333333,
                  0x4444444444444444);
}

void TEST_CASE10(void) {
  uint32_t vlenb;
  asm volatile("csrr %0, vlenb" : "=r"(vlenb));
  volatile uint16_t MEM[128];
  for (uint32_t i = 0; i < vlenb; i++)
    MEM[i] = (i < vlenb / 2) ? 0x0000 : 0x0505;

  VSET(vlenb, e8, m1);
  asm volatile(
    "vmv.v.i v3, 0\n\t"
    "vle16.v v2, (%0)\n\t"
    "vadd.vv v6, v3, v3\n\t"
    :: "r"(MEM) : "memory");

  VSET(4, e32, m1);
  VCMP_U32(10, v6, 0x0A0A0A0A, 0x0A0A0A0A, 0x0A0A0A0A, 0x0A0A0A0A);
}

void TEST_CASE11(void) {
  uint32_t vlenb;
  asm volatile("csrr %0, vlenb" : "=r"(vlenb));
  volatile uint16_t MEM[128];
  for (uint32_t i = 0; i < vlenb; i++)
    MEM[i] = (i < vlenb / 2) ? 0x0000 : 0x0505;
  uint32_t stride = 2;

  VSET(vlenb, e8, m1);
  asm volatile(
    "vmv.v.i v3, 0\n\t"
    "vlse16.v v2, (%0), %1\n\t"
    "vadd.vv v6, v3, v3\n\t"
    :: "r"(MEM), "r"(stride) : "memory");

  VSET(4, e32, m1);
  VCMP_U32(11, v6, 0x0A0A0A0A, 0x0A0A0A0A, 0x0A0A0A0A, 0x0A0A0A0A);
}

//*******Checking that vle64 leaves the tail (elements >= vl) undisturbed******//
// The destination is filled with a sentinel (0x0123456789abcdef), then a
// different pattern (0x5a5a5a5a5a5a5a5a) is loaded into another register so
// that any stale data left inside the VLSU is recognizable. The partial load
// runs with tu/mu: elements past vl must still hold the sentinel afterwards.
// With 64-bit elements the tail always starts on a word boundary, so these
// cases check the word-level write enables (and, for LMUL = 2, a tail that
// continues into the second register of the group on a 256-bit VRF).
//   0x0123456789abcdef -> correct (tail untouched)
//   0x5a5a5a5a5a5a5a5a -> tail written with stale VLSU data
//   INP1[]             -> tail written with memory data past vl

// vl = 1 of 4
void TEST_CASE12(void) {
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                              0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019};
  VSET(4, e64, m1);
  VLOAD_64(v1, 0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef);
  VLOAD_64(v2, 0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a);

  VSET_TU(1, e64, m1);
  asm volatile("vle64.v v1, (%0)" ::"r"(INP1) : "memory");

  VSET(4, e64, m1);
  VCMP_U64(12, v1, 0xDEADBEEFCAFE0000, 0x0123456789abcdef,
                   0x0123456789abcdef, 0x0123456789abcdef);
}

// vl = 3 of 4
void TEST_CASE13(void) {
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                              0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019};
  VSET(4, e64, m1);
  VLOAD_64(v1, 0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef);
  VLOAD_64(v2, 0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a);

  VSET_TU(3, e64, m1);
  asm volatile("vle64.v v1, (%0)" ::"r"(INP1) : "memory");

  VSET(4, e64, m1);
  VCMP_U64(13, v1, 0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                   0xDEADBEEFCAFE00e4, 0x0123456789abcdef);
}

// vl = 5 of 8, LMUL = 2
void TEST_CASE14(void) {
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                              0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019,
                              0xDEADBEEFCAFE0020, 0xDEADBEEFCAFE008f,
                              0xDEADBEEFCAFE002e, 0xDEADBEEFCAFE0005};
  VSET(8, e64, m2);
  VLOAD_64(v2, 0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef);
  VLOAD_64(v4, 0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a);

  VSET_TU(5, e64, m2);
  asm volatile("vle64.v v2, (%0)" ::"r"(INP1) : "memory");

  VSET(8, e64, m2);
  VCMP_U64(14, v2, 0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                   0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019,
                   0xDEADBEEFCAFE0020, 0x0123456789abcdef,
                   0x0123456789abcdef, 0x0123456789abcdef);
}

// vl = 3 of 4, mask 0x05: masked-off elements and tail must both keep the
// sentinel
void TEST_CASE15(void) {
  volatile uint64_t INP1[] = {0xDEADBEEFCAFE0000, 0xDEADBEEFCAFE009f,
                              0xDEADBEEFCAFE00e4, 0xDEADBEEFCAFE0019};
  VSET(1, e8, m1);
  VLOAD_8(v0, 0x05);

  VSET(4, e64, m1);
  VLOAD_64(v1, 0x0123456789abcdef, 0x0123456789abcdef,
               0x0123456789abcdef, 0x0123456789abcdef);
  VLOAD_64(v2, 0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a,
               0x5a5a5a5a5a5a5a5a, 0x5a5a5a5a5a5a5a5a);

  VSET_TU(3, e64, m1);
  asm volatile("vle64.v v1, (%0), v0.t" ::"r"(INP1) : "memory");

  VSET(4, e64, m1);
  VCMP_U64(15, v1, 0xDEADBEEFCAFE0000, 0x0123456789abcdef,
                   0xDEADBEEFCAFE00e4, 0x0123456789abcdef);
}

int main(void) {
  INIT_CHECK();
  enable_vec();

#if ELEN == 64
  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();
  TEST_CASE6();
  TEST_CASE7();
  TEST_CASE8();
  TEST_CASE9();
  TEST_CASE10();
  TEST_CASE11();
  TEST_CASE12();
  TEST_CASE13();
  TEST_CASE14();
  TEST_CASE15();
#endif

  EXIT_CHECK();
}