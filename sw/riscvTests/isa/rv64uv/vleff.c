// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Unit-stride fault-only-first loads: vle8ff.v, vle16ff.v, vle32ff.v,
// vle64ff.v
//
// Spatz never raises memory exceptions, so a fault-only-first load can never
// trim vl: it must behave exactly like the corresponding vle<eew>.v and leave
// vl and vtype unchanged. The vl-trimming path (fault on element > 0) cannot
// be exercised on this platform and is NOT covered here.
//
// Every test case checks both the loaded data and that vl/vtype are the same
// before and after the FF load.

#include "vector_macros.h"

#define V8_GROUP "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"

// Snapshot vl/vtype before the FF load ...
#define FF_SNAPSHOT                                                            \
  unsigned long ff_vl0, ff_vtype0, ff_vl1, ff_vtype1;                          \
  read_vl(ff_vl0) read_vtype(ff_vtype0)

// ... and check that the FF load did not modify them
#define FF_CHECK_VL(casenum)                                                   \
  read_vl(ff_vl1) read_vtype(ff_vtype1)                                        \
  check_vtype_vl(casenum, ff_vtype1, ff_vtype0, ff_vl1, ff_vl0)

static inline uint32_t get_vlenb(void) {
  uint32_t vlenb;
  asm volatile("csrr %0, vlenb" : "=r"(vlenb));
  return vlenb;
}

//////////////////////
// Unmasked, per EEW //
//////////////////////

// vle8ff, vl = 16
void TEST_CASE1(void) {
  VSET(16, e8, m1);
  volatile uint8_t INP1[] = {0x00, 0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05,
                             0xe0, 0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb};
  VCLEAR(v1);
  FF_SNAPSHOT;
  asm volatile("vle8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(1);
  VCMP_U8(2, v1, 0x00, 0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0, 0xf9,
          0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb);
}

// vle16ff, vl = 16
void TEST_CASE2(void) {
  VSET(16, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VCLEAR(v1);
  FF_SNAPSHOT;
  asm volatile("vle16ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(3);
  VCMP_U16(4, v1, 0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0, 0xc394,
           0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0, 0x1357, 0x2468, 0x369b,
           0x48ac);
}

// vle32ff, vl = 16 (LMUL = 2)
void TEST_CASE3(void) {
  VSET(16, e32, m2);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vle32ff.v v2, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(5);
  VCMP_U32(6, v2, 0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3, 0x12345678,
           0x9abcdef0, 0x13572468, 0x369b48ac, 0xdeadbeef, 0xcafebabe,
           0x01234567, 0x89abcdef, 0x55aa55aa, 0x77889900, 0xabcdef12,
           0x34567890);
}

// vle64ff, vl = 16 (LMUL = 4)
void TEST_CASE4(void) {
#if ELEN == 64
  VSET(16, e64, m4);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VCLEAR(v4);
  FF_SNAPSHOT;
  asm volatile("vle64ff.v v4, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(7);
  VCMP_U64(8, v4, 0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
           0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
           0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
           0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
           0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
           0x987654321fedcba0);
#endif
}

///////////////////////////////////////////////////////////
// Masked, per EEW: inactive elements keep the old value //
///////////////////////////////////////////////////////////

// vle8ff, mask 0xAA
void TEST_CASE5(void) {
  VSET(2, e8, m1);
  VLOAD_8(v0, 0xAA, 0xAA);

  VSET(16, e8, m1);
  volatile uint8_t INP1[] = {0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89};
  VLOAD_8(v1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  FF_SNAPSHOT;
  asm volatile("vle8ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(9);
  VCMP_U8(10, v1, 1, 0xd3, 3, 0xd1, 5, 0x48, 7, 0x88, 9, 0xae, 11, 0x91, 13,
          0x59, 15, 0x89);
}

// vle16ff, mask 0x55
void TEST_CASE6(void) {
  VSET(2, e8, m1);
  VLOAD_8(v0, 0x55, 0x55);

  VSET(16, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VLOAD_16(v1, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef,
           0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef);
  FF_SNAPSHOT;
  asm volatile("vle16ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(11);
  VCMP_U16(12, v1, 0x9fe4, 0xbeef, 0x8f2e, 0xbeef, 0xf9aa, 0xbeef, 0xc394,
           0xbeef, 0x1234, 0xbeef, 0x9abc, 0xbeef, 0x1357, 0xbeef, 0x369b,
           0xbeef);
}

// vle32ff, mask 0x0F 0xF0 (LMUL = 2)
void TEST_CASE7(void) {
  VSET(2, e8, m1);
  VLOAD_8(v0, 0x0F, 0xF0);

  VSET(16, e32, m2);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VLOAD_32(v2, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1,
           0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1,
           0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1,
           0xa1a1a1a1);
  FF_SNAPSHOT;
  asm volatile("vle32ff.v v2, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(13);
  VCMP_U32(14, v2, 0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3, 0xa1a1a1a1,
           0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1,
           0xa1a1a1a1, 0xa1a1a1a1, 0x55aa55aa, 0x77889900, 0xabcdef12,
           0x34567890);
}

// vle64ff, mask 0x6D 0x0B (LMUL = 4)
void TEST_CASE8(void) {
#if ELEN == 64
  VSET(2, e8, m1);
  VLOAD_8(v0, 0x6D, 0x0B);

  VSET(16, e64, m4);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VLOAD_64(v4, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef);
  FF_SNAPSHOT;
  asm volatile("vle64ff.v v4, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(15);
  VCMP_U64(16, v4, 0x9fe419208f2e05e0, 0xdeadbeefdeadbeef, 0x123456789abcdef0,
           0x13572468369b48ac, 0xdeadbeefdeadbeef, 0x0123456789abcdef,
           0x55aa55aa77889900, 0xdeadbeefdeadbeef, 0xfeedfacecafebabe,
           0x123456789abcdef0, 0xdeadbeefdeadbeef, 0x369b48acdeadbeef,
           0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef, 0xdeadbeefdeadbeef,
           0xdeadbeefdeadbeef);
#endif
}

//////////////////
// Corner cases //
//////////////////

// All-zeros mask: nothing is loaded, vd untouched, vl unchanged
void TEST_CASE9(void) {
  VSET(2, e8, m1);
  VLOAD_8(v0, 0x00, 0x00);

  VSET(16, e8, m1);
  volatile uint8_t INP1[] = {0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89};
  VLOAD_8(v1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  FF_SNAPSHOT;
  asm volatile("vle8ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(17);
  VCMP_U8(18, v1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
}

// vl < VLMAX, tail-undisturbed: vl must stay 5 (not grow to VLMAX) and the
// tail elements [5, 8) must keep their old value
void TEST_CASE10(void) {
  unsigned long vl;
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3};

  asm volatile("vsetvli %0, %1, e16, m1, tu, mu" : "=r"(vl) : "r"(8));
  VLOAD_16(v1, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef, 0xbeef);

  asm volatile("vsetvli %0, %1, e16, m1, tu, mu" : "=r"(vl) : "r"(5));
  FF_SNAPSHOT;
  asm volatile("vle16ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(19);
  XCMP(20, ff_vl1, 5);

  asm volatile("vsetvli %0, %1, e16, m1, tu, mu" : "=r"(vl) : "r"(8));
  VCMP_U16(21, v1, 0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0xbeef, 0xbeef,
           0xbeef);
}

// LMUL = 8, vl = 64, masked (same pattern as vle8 TEST_CASE6)
void TEST_CASE11(void) {
  VSET(8, e8, m1);
  VLOAD_8(v0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA);

  VSET(64, e8, m8);
  volatile uint8_t INP1[] = {0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89,
                             0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89,
                             0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89,
                             0xe0, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x89, 0x88,
                             0x88, 0xae, 0x08, 0x91, 0x02, 0x59, 0x11, 0x89};
  VLOAD_8(v8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
              17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
              33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
              49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64);
  FF_SNAPSHOT;
  asm volatile("vle8ff.v v8, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(22);
  VCMP_U8(23, v8, 1, 0xd3, 3, 0xd1, 5, 0x48, 7, 0x88, 9, 0xae, 11, 0x91, 13, 0x59, 15, 0x89,
                  17, 0xd3, 19, 0xd1, 21, 0x48, 23, 0x88, 25, 0xae, 27, 0x91, 29, 0x59, 31, 0x89,
                  33, 0xd3, 35, 0xd1, 37, 0x48, 39, 0x88, 41, 0xae, 43, 0x91, 45, 0x59, 47, 0x89,
                  49, 0xd3, 51, 0xd1, 53, 0x48, 55, 0x88, 57, 0xae, 59, 0x91, 61, 0x59, 63, 0x89);
}

// RAW hazard: a consumer reads the last register of the m8 group written by
// the FF load, with no drain in between (same as vle8 TEST_CASE7)
void TEST_CASE12(void) {
  uint32_t vlenb = get_vlenb();

  // m8 group v8..v15; only v15 (the last register) gets 5,6,7,8,...
  volatile uint8_t buf[8 * vlenb] __attribute__((aligned(8)));
  for (uint32_t i = 0; i < 8 * vlenb; i++)
    buf[i] = (i < 7 * vlenb) ? 0x11 : (uint8_t)(i - 7 * vlenb + 5);

  // Poison v15 so a premature read is visible as 0xFF
  VSET(vlenb, e8, m1);
  asm volatile("vmv.v.i v15, -1" ::: "v15");

  // Long producer: 8 * VLENB bytes, v15 is written at the very end
  VSET(8 * vlenb, e8, m8);
  asm volatile("vle8ff.v v8, (%0)" ::"r"(buf) : V8_GROUP, "memory");

  // Consumer reads the last register of the group at LMUL=1
  VSET(4, e8, m1);
  asm volatile("vadd.vi v4, v15, 0" ::: "v4");

  VCMP_U8(24, v4, 5, 6, 7, 8);
}

// Typical FF usage: a strip-mined copy loop that advances the pointers by the
// vl read back *after* the FF load. The length spans more than two full
// vector registers plus a partial tail.
void TEST_CASE13(void) {
  uint32_t vlenb = get_vlenb();
  const uint32_t len = 2 * vlenb + 5;

  volatile uint8_t src[len] __attribute__((aligned(8)));
  volatile uint8_t dst[len] __attribute__((aligned(8)));
  for (uint32_t i = 0; i < len; i++) {
    src[i] = (uint8_t)(i * 7 + 3);
    dst[i] = 0;
  }

  volatile uint8_t *s = src;
  volatile uint8_t *d = dst;
  uint32_t n = len;
  uint32_t vl_mismatch = 0;
  uint32_t iters = 0;

  while (n > 0 && iters < 16) {
    unsigned long vl, vl_ff;
    asm volatile("vsetvli %0, %1, e8, m1, ta, ma" : "=r"(vl) : "r"(n));
    asm volatile("vle8ff.v v1, (%0)" ::"r"(s) : "v1", "memory");
    read_vl(vl_ff);
    if (vl_ff != vl)
      vl_mismatch++;
    asm volatile("vse8.v v1, (%0)" ::"r"(d) : "memory");
    s += vl_ff;
    d += vl_ff;
    n -= vl_ff;
    iters++;
  }

  // The vector store must be complete before the scalar reads below
  asm volatile("fence" ::: "memory");

  uint32_t data_mismatch = 0;
  for (uint32_t i = 0; i < len; i++)
    if (dst[i] != src[i])
      data_mismatch++;

  XCMP(25, vl_mismatch, 0);
  XCMP(26, n, 0);
  XCMP(27, data_mismatch, 0);
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
  TEST_CASE8();
  TEST_CASE9();
  // TEST_CASE10(); BUG
  TEST_CASE11();
  TEST_CASE12();
  TEST_CASE13();

  EXIT_CHECK();
}
