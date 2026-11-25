// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Unit-stride segment fault-only-first loads: vlseg<nf>e<eew>ff.v
//
// Spatz never raises memory exceptions, so a fault-only-first segment load can
// never trim vl: it must behave exactly like the corresponding
// vlseg<nf>e<eew>.v and leave vl and vtype unchanged. The vl-trimming path
// (fault on segment > 0) cannot be exercised on this platform and is NOT
// covered here.
//
// Input data and golden values are the same as in vlseg.c. Every test case
// additionally checks that vl/vtype are the same before and after the load.

#include "vector_macros.h"

// Snapshot vl/vtype before the FF load ...
#define FF_SNAPSHOT                                                            \
  unsigned long ff_vl0, ff_vtype0, ff_vl1, ff_vtype1;                          \
  read_vl(ff_vl0) read_vtype(ff_vtype0)

// ... and check that the FF load did not modify them
#define FF_CHECK_VL(casenum)                                                   \
  read_vl(ff_vl1) read_vtype(ff_vtype1)                                        \
  check_vtype_vl(casenum, ff_vtype1, ff_vtype0, ff_vl1, ff_vl0)

////////////////////////////
// Unmasked, per NF / EEW //
////////////////////////////

// Segment-2 for 8-bit, vl = 4 --> 8 bytes
void TEST_CASE2_8_vl4(void) {
  VSET(4, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VCLEAR(v1);
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(1);
  VCMP_U8(2, v1, 0x9f, 0x19, 0x8f, 0x05);
  VCMP_U8(3, v2, 0xe4, 0x20, 0x2e, 0xe0);
}

// Segment-3 for 8-bit, vl = 4 --> 12 bytes
void TEST_CASE3_8_vl4(void) {
  VSET(4, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  FF_SNAPSHOT;
  asm volatile("vlseg3e8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(4);
  VCMP_U8(5, v1, 0x9f, 0x20, 0x05, 0xaa);
  VCMP_U8(6, v2, 0xe4, 0x8f, 0xe0, 0x71);
  VCMP_U8(7, v3, 0x19, 0x2e, 0xf9, 0xf0);
}

// Segment-4 for 8-bit, vl = 4 --> 16 bytes
void TEST_CASE4_8_vl4(void) {
  VSET(4, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  FF_SNAPSHOT;
  asm volatile("vlseg4e8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(8);
  VCMP_U8(9, v1, 0x9f, 0x8f, 0xf9, 0xc3);
  VCMP_U8(10, v2, 0xe4, 0x2e, 0xaa, 0x94);
  VCMP_U8(11, v3, 0x19, 0x05, 0x71, 0xbb);
  VCMP_U8(12, v4, 0x20, 0xe0, 0xf0, 0xd3);
}

// Segment-8 for 8-bit, vl = 2 --> 16 bytes
void TEST_CASE8_8_vl2(void) {
  VSET(2, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  VCLEAR(v5);
  VCLEAR(v6);
  VCLEAR(v7);
  VCLEAR(v8);
  FF_SNAPSHOT;
  asm volatile("vlseg8e8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(13);
  VCMP_U8(14, v1, 0x9f, 0xf9);
  VCMP_U8(15, v2, 0xe4, 0xaa);
  VCMP_U8(16, v3, 0x19, 0x71);
  VCMP_U8(17, v4, 0x20, 0xf0);
  VCMP_U8(18, v5, 0x8f, 0xc3);
  VCMP_U8(19, v6, 0x2e, 0x94);
  VCMP_U8(20, v7, 0x05, 0xbb);
  VCMP_U8(21, v8, 0xe0, 0xd3);
}

// Segment-2 for 16-bit, vl = 4 --> 16 bytes
void TEST_CASE2_16_vl4(void) {
  VSET(4, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VCLEAR(v1);
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e16ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(22);
  VCMP_U16(23, v1, 0x9fe4, 0x8f2e, 0xf9aa, 0xc394);
  VCMP_U16(24, v2, 0x1920, 0x05e0, 0x71f0, 0xbbd3);
}

// Segment-4 for 16-bit, vl = 4 --> 32 bytes
void TEST_CASE4_16_vl4(void) {
  VSET(4, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  FF_SNAPSHOT;
  asm volatile("vlseg4e16ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(25);
  VCMP_U16(26, v1, 0x9fe4, 0xf9aa, 0x1234, 0x1357);
  VCMP_U16(27, v2, 0x1920, 0x71f0, 0x5678, 0x2468);
  VCMP_U16(28, v3, 0x8f2e, 0xc394, 0x9abc, 0x369b);
  VCMP_U16(29, v4, 0x05e0, 0xbbd3, 0xdef0, 0x48ac);
}

// Segment-8 for 16-bit, vl = 2 --> 32 bytes
void TEST_CASE8_16_vl2(void) {
  VSET(2, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  VCLEAR(v5);
  VCLEAR(v6);
  VCLEAR(v7);
  VCLEAR(v8);
  FF_SNAPSHOT;
  asm volatile("vlseg8e16ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(30);
  VCMP_U16(31, v1, 0x9fe4, 0x1234);
  VCMP_U16(32, v2, 0x1920, 0x5678);
  VCMP_U16(33, v3, 0x8f2e, 0x9abc);
  VCMP_U16(34, v4, 0x05e0, 0xdef0);
  VCMP_U16(35, v5, 0xf9aa, 0x1357);
  VCMP_U16(36, v6, 0x71f0, 0x2468);
  VCMP_U16(37, v7, 0xc394, 0x369b);
  VCMP_U16(38, v8, 0xbbd3, 0x48ac);
}

// Segment-2 for 32-bit, vl = 4 --> 32 bytes
void TEST_CASE2_32_vl4(void) {
  VSET(4, e32, m1);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VCLEAR(v1);
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e32ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(39);
  VCMP_U32(40, v1, 0x9fe41920, 0xf9aa71f0, 0x12345678, 0x13572468);
  VCMP_U32(41, v2, 0x8f2e05e0, 0xc394bbd3, 0x9abcdef0, 0x369b48ac);
}

// Segment-3 for 32-bit, vl = 4 --> 48 bytes
void TEST_CASE3_32_vl4(void) {
  VSET(4, e32, m1);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  FF_SNAPSHOT;
  asm volatile("vlseg3e32ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(42);
  VCMP_U32(43, v1, 0x9fe41920, 0xc394bbd3, 0x13572468, 0xcafebabe);
  VCMP_U32(44, v2, 0x8f2e05e0, 0x12345678, 0x369b48ac, 0x01234567);
  VCMP_U32(45, v3, 0xf9aa71f0, 0x9abcdef0, 0xdeadbeef, 0x89abcdef);
}

// Segment-8 for 32-bit, vl = 2 --> 64 bytes
void TEST_CASE8_32_vl2(void) {
  VSET(2, e32, m1);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  VCLEAR(v5);
  VCLEAR(v6);
  VCLEAR(v7);
  VCLEAR(v8);
  FF_SNAPSHOT;
  asm volatile("vlseg8e32ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(46);
  VCMP_U32(47, v1, 0x9fe41920, 0xdeadbeef);
  VCMP_U32(48, v2, 0x8f2e05e0, 0xcafebabe);
  VCMP_U32(49, v3, 0xf9aa71f0, 0x01234567);
  VCMP_U32(50, v4, 0xc394bbd3, 0x89abcdef);
  VCMP_U32(51, v5, 0x12345678, 0x55aa55aa);
  VCMP_U32(52, v6, 0x9abcdef0, 0x77889900);
  VCMP_U32(53, v7, 0x13572468, 0xabcdef12);
  VCMP_U32(54, v8, 0x369b48ac, 0x34567890);
}

// Segment-2 for 64-bit, vl = 4 --> 64 bytes
void TEST_CASE2_64_vl4(void) {
#if ELEN == 64
  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VCLEAR(v1);
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e64ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(55);
  VCMP_U64(56, v1, 0x9fe419208f2e05e0, 0x123456789abcdef0, 0xdeadbeefcafebabe, 0x55aa55aa77889900);
  VCMP_U64(57, v2, 0xf9aa71f0c394bbd3, 0x13572468369b48ac, 0x0123456789abcdef, 0xabcdef1234567890);
#endif
}

// Segment-4 for 64-bit, vl = 4 --> 128 bytes
void TEST_CASE4_64_vl4(void) {
#if ELEN == 64
  VSET(4, e64, m1);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  FF_SNAPSHOT;
  asm volatile("vlseg4e64ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(58);
  VCMP_U64(59, v1, 0x9fe419208f2e05e0, 0xdeadbeefcafebabe, 0xfeedfacecafebabe, 0xcafebabe12345678);
  VCMP_U64(60, v2, 0xf9aa71f0c394bbd3, 0x0123456789abcdef, 0x123456789abcdef0, 0xabcdef0987654321);
  VCMP_U64(61, v3, 0x123456789abcdef0, 0x55aa55aa77889900, 0x1357246855aa55aa, 0x012345670abcdef1);
  VCMP_U64(62, v4, 0x13572468369b48ac, 0xabcdef1234567890, 0x369b48acdeadbeef, 0x987654321fedcba0);
#endif
}

// Segment-8 for 64-bit, vl = 2 --> 128 bytes
void TEST_CASE8_64_vl2(void) {
#if ELEN == 64
  VSET(2, e64, m1);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VCLEAR(v1);
  VCLEAR(v2);
  VCLEAR(v3);
  VCLEAR(v4);
  VCLEAR(v5);
  VCLEAR(v6);
  VCLEAR(v7);
  VCLEAR(v8);
  FF_SNAPSHOT;
  asm volatile("vlseg8e64ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(63);
  VCMP_U64(64, v1, 0x9fe419208f2e05e0, 0xfeedfacecafebabe);
  VCMP_U64(65, v2, 0xf9aa71f0c394bbd3, 0x123456789abcdef0);
  VCMP_U64(66, v3, 0x123456789abcdef0, 0x1357246855aa55aa);
  VCMP_U64(67, v4, 0x13572468369b48ac, 0x369b48acdeadbeef);
  VCMP_U64(68, v5, 0xdeadbeefcafebabe, 0xcafebabe12345678);
  VCMP_U64(69, v6, 0x0123456789abcdef, 0xabcdef0987654321);
  VCMP_U64(70, v7, 0x55aa55aa77889900, 0x012345670abcdef1);
  VCMP_U64(71, v8, 0xabcdef1234567890, 0x987654321fedcba0);
#endif
}

///////////////////////////////////////////////////////////////////////
// Masked: non-zero background, inactive elements keep the old value //
///////////////////////////////////////////////////////////////////////

// Segment-2 for 8-bit, vl = 4, mask 0b1010
void TEST_CASE2_8_vl4_m(void) {
  VSET(4, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VLOAD_8(v0, 0x0A, 0x00, 0x00, 0x00);
  VLOAD_8(v1, 0xa1, 0xa1, 0xa1, 0xa1);
  VLOAD_8(v2, 0xb2, 0xb2, 0xb2, 0xb2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e8ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(72);
  VCMP_U8(73, v1, 0xa1, 0x19, 0xa1, 0x05);
  VCMP_U8(74, v2, 0xb2, 0x20, 0xb2, 0xe0);
}

// Segment-4 for 16-bit, vl = 4, mask 0b0101
void TEST_CASE4_16_vl4_m(void) {
  VSET(4, e16, m1);
  volatile uint16_t INP1[] = {0x9fe4, 0x1920, 0x8f2e, 0x05e0, 0xf9aa, 0x71f0,
                              0xc394, 0xbbd3, 0x1234, 0x5678, 0x9abc, 0xdef0,
                              0x1357, 0x2468, 0x369b, 0x48ac};
  VLOAD_8(v0, 0x05, 0x00, 0x00, 0x00);
  VLOAD_16(v1, 0xa1a1, 0xa1a1, 0xa1a1, 0xa1a1);
  VLOAD_16(v2, 0xb2b2, 0xb2b2, 0xb2b2, 0xb2b2);
  VLOAD_16(v3, 0xc3c3, 0xc3c3, 0xc3c3, 0xc3c3);
  VLOAD_16(v4, 0xd4d4, 0xd4d4, 0xd4d4, 0xd4d4);
  FF_SNAPSHOT;
  asm volatile("vlseg4e16ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(75);
  VCMP_U16(76, v1, 0x9fe4, 0xa1a1, 0x1234, 0xa1a1);
  VCMP_U16(77, v2, 0x1920, 0xb2b2, 0x5678, 0xb2b2);
  VCMP_U16(78, v3, 0x8f2e, 0xc3c3, 0x9abc, 0xc3c3);
  VCMP_U16(79, v4, 0x05e0, 0xd4d4, 0xdef0, 0xd4d4);
}

// Segment-3 for 32-bit, vl = 4, mask 0b0110
void TEST_CASE3_32_vl4_m(void) {
  VSET(4, e32, m1);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  VLOAD_8(v0, 0x06, 0x00, 0x00, 0x00);
  VLOAD_32(v1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1);
  VLOAD_32(v2, 0xb2b2b2b2, 0xb2b2b2b2, 0xb2b2b2b2, 0xb2b2b2b2);
  VLOAD_32(v3, 0xc3c3c3c3, 0xc3c3c3c3, 0xc3c3c3c3, 0xc3c3c3c3);
  FF_SNAPSHOT;
  asm volatile("vlseg3e32ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(80);
  VCMP_U32(81, v1, 0xa1a1a1a1, 0xc394bbd3, 0x13572468, 0xa1a1a1a1);
  VCMP_U32(82, v2, 0xb2b2b2b2, 0x12345678, 0x369b48ac, 0xb2b2b2b2);
  VCMP_U32(83, v3, 0xc3c3c3c3, 0x9abcdef0, 0xdeadbeef, 0xc3c3c3c3);
}

// Segment-8 for 64-bit, vl = 2, mask 0b10
void TEST_CASE8_64_vl2_m(void) {
#if ELEN == 64
  VSET(2, e64, m1);
  volatile uint64_t INP1[] = {
      0x9fe419208f2e05e0, 0xf9aa71f0c394bbd3, 0x123456789abcdef0,
      0x13572468369b48ac, 0xdeadbeefcafebabe, 0x0123456789abcdef,
      0x55aa55aa77889900, 0xabcdef1234567890, 0xfeedfacecafebabe,
      0x123456789abcdef0, 0x1357246855aa55aa, 0x369b48acdeadbeef,
      0xcafebabe12345678, 0xabcdef0987654321, 0x012345670abcdef1,
      0x987654321fedcba0};
  VLOAD_8(v0, 0x02, 0x00);
  VLOAD_64(v1, 0x1111111111111111, 0x1111111111111111);
  VLOAD_64(v2, 0x2222222222222222, 0x2222222222222222);
  VLOAD_64(v3, 0x3333333333333333, 0x3333333333333333);
  VLOAD_64(v4, 0x4444444444444444, 0x4444444444444444);
  VLOAD_64(v5, 0x5555555555555555, 0x5555555555555555);
  VLOAD_64(v6, 0x6666666666666666, 0x6666666666666666);
  VLOAD_64(v7, 0x7777777777777777, 0x7777777777777777);
  VLOAD_64(v8, 0x8888888888888888, 0x8888888888888888);
  FF_SNAPSHOT;
  asm volatile("vlseg8e64ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(84);
  VCMP_U64(85, v1, 0x1111111111111111, 0xfeedfacecafebabe);
  VCMP_U64(86, v2, 0x2222222222222222, 0x123456789abcdef0);
  VCMP_U64(87, v3, 0x3333333333333333, 0x1357246855aa55aa);
  VCMP_U64(88, v4, 0x4444444444444444, 0x369b48acdeadbeef);
  VCMP_U64(89, v5, 0x5555555555555555, 0xcafebabe12345678);
  VCMP_U64(90, v6, 0x6666666666666666, 0xabcdef0987654321);
  VCMP_U64(91, v7, 0x7777777777777777, 0x012345670abcdef1);
  VCMP_U64(92, v8, 0x8888888888888888, 0x987654321fedcba0);
#endif
}

//////////////////
// Corner cases //
//////////////////

// All-zeros mask: no segment is loaded, destinations untouched, vl unchanged
void TEST_CASE3_8_vl4_m_none(void) {
  VSET(4, e8, m1);
  volatile uint8_t INP1[] = {0x9f, 0xe4, 0x19, 0x20, 0x8f, 0x2e, 0x05, 0xe0,
                             0xf9, 0xaa, 0x71, 0xf0, 0xc3, 0x94, 0xbb, 0xd3};
  VLOAD_8(v0, 0x00, 0x00, 0x00, 0x00);
  VLOAD_8(v1, 0xa1, 0xa1, 0xa1, 0xa1);
  VLOAD_8(v2, 0xb2, 0xb2, 0xb2, 0xb2);
  VLOAD_8(v3, 0xc3, 0xc3, 0xc3, 0xc3);
  FF_SNAPSHOT;
  asm volatile("vlseg3e8ff.v v1, (%0), v0.t" ::"r"(INP1) : "memory");
  FF_CHECK_VL(93);
  VCMP_U8(94, v1, 0xa1, 0xa1, 0xa1, 0xa1);
  VCMP_U8(95, v2, 0xb2, 0xb2, 0xb2, 0xb2);
  VCMP_U8(96, v3, 0xc3, 0xc3, 0xc3, 0xc3);
}

// Segment-2 for 8-bit, vl = 32 --> 64 bytes
void TEST_CASE2_8_vl32(void) {
  VSET(32, e8, m1);
  volatile uint8_t INP1[] = {0x31, 0xda, 0x32, 0x6f, 0xb5, 0x1a, 0xe6, 0xc7,
                             0xc1, 0x73, 0x67, 0x1e, 0xd0, 0xab, 0xbb, 0x46,
                             0x74, 0x2b, 0xa7, 0xf6, 0x50, 0xc3, 0xab, 0xee,
                             0x64, 0x73, 0xd7, 0xbe, 0x00, 0x6b, 0xab, 0xc6,
                             0x04, 0x4b, 0xb7, 0x36, 0x40, 0x43, 0x7b, 0x0e,
                             0x74, 0xb3, 0xc7, 0xde, 0xf0, 0x6b, 0x5b, 0xc6,
                             0x54, 0x2b, 0x07, 0xf6, 0x30, 0x03, 0x8b, 0xee,
                             0x04, 0xb3, 0xb7, 0xbe, 0xa0, 0xab, 0x4b, 0x46};
  VCLEAR(v1);
  VCLEAR(v2);
  FF_SNAPSHOT;
  asm volatile("vlseg2e8ff.v v1, (%0)" ::"r"(INP1) : "memory");
  FF_CHECK_VL(97);
  VCMP_U8(98, v1, 0x31, 0x32, 0xb5, 0xe6, 0xc1, 0x67, 0xd0, 0xbb,
                  0x74, 0xa7, 0x50, 0xab, 0x64, 0xd7, 0x00, 0xab,
                  0x04, 0xb7, 0x40, 0x7b, 0x74, 0xc7, 0xf0, 0x5b,
                  0x54, 0x07, 0x30, 0x8b, 0x04, 0xb7, 0xa0, 0x4b);
  VCMP_U8(99, v2, 0xda, 0x6f, 0x1a, 0xc7, 0x73, 0x1e, 0xab, 0x46,
                  0x2b, 0xf6, 0xc3, 0xee, 0x73, 0xbe, 0x6b, 0xc6,
                  0x4b, 0x36, 0x43, 0x0e, 0xb3, 0xde, 0x6b, 0xc6,
                  0x2b, 0xf6, 0x03, 0xee, 0xb3, 0xbe, 0xab, 0x46);
}

// EMUL = 2: field 0 goes to v2-v3, field 1 to v4-v5 (same as vlseg D26)
void TEST_CASE2_32_m2(void) {
  VSET(4, e32, m2);
  volatile uint32_t MEM[] = {0x0a, 0x14, 0x1e, 0x28, 0x32, 0x3c, 0x46, 0x50};

  // Pre-fill v2..v5 with distinct patterns to detect clobbering
  asm volatile("vmv.v.i v2, 2\n\t"
               "vmv.v.i v4, 4\n\t");

  FF_SNAPSHOT;
  asm volatile("vlseg2e32ff.v v2, (%0)" ::"r"(MEM) : "memory");
  FF_CHECK_VL(100);
  VCMP_U32(101, v2, 0x0a, 0x1e, 0x32, 0x46);
  VCMP_U32(102, v4, 0x14, 0x28, 0x3c, 0x50);
}

// RAW hazard: a consumer reads the last field register right after the FF
// segment load, with no drain in between
void TEST_CASE4_32_vl4_raw(void) {
  VSET(4, e32, m1);
  volatile uint32_t INP1[] = {0x9fe41920, 0x8f2e05e0, 0xf9aa71f0, 0xc394bbd3,
                              0x12345678, 0x9abcdef0, 0x13572468, 0x369b48ac,
                              0xdeadbeef, 0xcafebabe, 0x01234567, 0x89abcdef,
                              0x55aa55aa, 0x77889900, 0xabcdef12, 0x34567890};
  // Poison the last field so a premature read is visible
  asm volatile("vmv.v.i v4, -1" ::: "v4");

  asm volatile("vlseg4e32ff.v v1, (%0)" ::"r"(INP1)
               : "v1", "v2", "v3", "v4", "memory");
  asm volatile("vadd.vi v12, v4, 1" ::: "v12");

  VCMP_U32(103, v12, 0xc394bbd4, 0x369b48ad, 0x89abcdf0, 0x34567891);
}

int main(void) {
  INIT_CHECK();
  enable_vec();

  TEST_CASE2_8_vl4();
  TEST_CASE3_8_vl4();
  TEST_CASE4_8_vl4();
  TEST_CASE8_8_vl2();

  TEST_CASE2_16_vl4();
  TEST_CASE4_16_vl4();
  TEST_CASE8_16_vl2();

  TEST_CASE2_32_vl4();
  TEST_CASE3_32_vl4();
  TEST_CASE8_32_vl2();

  TEST_CASE2_64_vl4();
  TEST_CASE4_64_vl4();
  TEST_CASE8_64_vl2();

  TEST_CASE2_8_vl4_m();
  TEST_CASE4_16_vl4_m();
  TEST_CASE3_32_vl4_m();
  TEST_CASE8_64_vl2_m();

  TEST_CASE3_8_vl4_m_none();
  TEST_CASE2_8_vl32();
  TEST_CASE2_32_m2();
  TEST_CASE4_32_vl4_raw();

  EXIT_CHECK();
}
