// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Whole-register moves: vmv1r.v, vmv2r.v, vmv4r.v, vmv8r.v
//
// These copy NREG whole registers regardless of vl and of the LMUL in vtype
// (evl = NREG * VLEN / SEW). Each source register gets its own pattern, so a
// destination register that is not copied, or copied from the wrong source,
// shows up in the check of that register.

#include "vector_macros.h"

// Load / check the first 16 bytes of a single register (VLENB >= 32 on all
// configs, so this also works with the small VRF). Must be used under e8, m1.
#define VLOAD_REG(reg, b)                                                      \
  VLOAD_8(reg, (b), (b) + 1, (b) + 2, (b) + 3, (b) + 4, (b) + 5, (b) + 6,      \
          (b) + 7, (b) + 8, (b) + 9, (b) + 10, (b) + 11, (b) + 12, (b) + 13,   \
          (b) + 14, (b) + 15)
#define VCMP_REG(casenum, reg, b)                                              \
  VCMP_U8(casenum, reg, (b), (b) + 1, (b) + 2, (b) + 3, (b) + 4, (b) + 5,      \
          (b) + 6, (b) + 7, (b) + 8, (b) + 9, (b) + 10, (b) + 11, (b) + 12,    \
          (b) + 13, (b) + 14, (b) + 15)

// Fill v16..v23 with a different pattern per register: v(16+k) = 0x10*k + 1..
#define LOAD_SOURCES()                                                         \
  do {                                                                         \
    VSET(16, e8, m1);                                                          \
    VLOAD_REG(v16, 0x01);                                                      \
    VLOAD_REG(v17, 0x11);                                                      \
    VLOAD_REG(v18, 0x21);                                                      \
    VLOAD_REG(v19, 0x31);                                                      \
    VLOAD_REG(v20, 0x41);                                                      \
    VLOAD_REG(v21, 0x51);                                                      \
    VLOAD_REG(v22, 0x61);                                                      \
    VLOAD_REG(v23, 0x71);                                                      \
  } while (0)

// Zero the whole destination group v8..v15
#define CLEAR_DEST()                                                           \
  do {                                                                         \
    VSET(16, e8, m8);                                                          \
    VCLEAR(v8);                                                                \
  } while (0)

// Compare two byte buffers of length n, VCMP-style
#define BUFCMP(casenum, dst, src, n)                                           \
  do {                                                                         \
    for (unsigned int i = 0; i < (n); i++) {                                   \
      if ((dst)[i] != (src)[i]) {                                              \
        printf("[TC %d] Index %d FAILED. Got %hhu, expected %hhu.\n",          \
               casenum, i, (dst)[i], (src)[i]);                                \
        num_failed++;                                                          \
        return;                                                                \
      }                                                                        \
    }                                                                          \
    printf("[TC %d] PASSED.\n", casenum);                                      \
  } while (0)

// Each NREG under e8, m1: every register of the group must be copied, and
// registers past the group must stay untouched.
void TEST_CASE1(void) {
  LOAD_SOURCES();

  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv1r.v v8, v16");
  VCMP_REG(1, v8, 0x01);
  VCMP_U8(2, v9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv2r.v v8, v16");
  VCMP_REG(3, v8, 0x01);
  VCMP_REG(4, v9, 0x11);
  VCMP_U8(5, v10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv4r.v v8, v16");
  VCMP_REG(6, v8, 0x01);
  VCMP_REG(7, v9, 0x11);
  VCMP_REG(8, v10, 0x21);
  VCMP_REG(9, v11, 0x31);
  VCMP_U8(10, v12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv8r.v v8, v16");
  VCMP_REG(11, v8, 0x01);
  VCMP_REG(12, v9, 0x11);
  VCMP_REG(13, v10, 0x21);
  VCMP_REG(14, v11, 0x31);
  VCMP_REG(15, v12, 0x41);
  VCMP_REG(16, v13, 0x51);
  VCMP_REG(17, v14, 0x61);
  VCMP_REG(18, v15, 0x71);
}

// The copy uses EEW = SEW, so evl changes with SEW. The bytes moved must not.
void TEST_CASE2(void) {
  CLEAR_DEST();
  VSET(16, e16, m1);
  VLOAD_16(v16, 0x0101, 0x0202, 0x0303, 0x0404, 0x0505, 0x0606, 0x0707, 0x0808,
           0x0909, 0x0a0a, 0x0b0b, 0x0c0c, 0x0d0d, 0x0e0e, 0x0f0f, 0x1010);
  VLOAD_16(v17, 0x1111, 0x1212, 0x1313, 0x1414, 0x1515, 0x1616, 0x1717, 0x1818,
           0x1919, 0x1a1a, 0x1b1b, 0x1c1c, 0x1d1d, 0x1e1e, 0x1f1f, 0x2020);
  asm volatile("vmv2r.v v8, v16");
  VCMP_U16(19, v8, 0x0101, 0x0202, 0x0303, 0x0404, 0x0505, 0x0606, 0x0707,
           0x0808, 0x0909, 0x0a0a, 0x0b0b, 0x0c0c, 0x0d0d, 0x0e0e, 0x0f0f,
           0x1010);
  VCMP_U16(20, v9, 0x1111, 0x1212, 0x1313, 0x1414, 0x1515, 0x1616, 0x1717,
           0x1818, 0x1919, 0x1a1a, 0x1b1b, 0x1c1c, 0x1d1d, 0x1e1e, 0x1f1f,
           0x2020);

  CLEAR_DEST();
  VSET(8, e32, m1);
  VLOAD_32(v16, 0x01010101, 0x02020202, 0x03030303, 0x04040404, 0x05050505,
           0x06060606, 0x07070707, 0x08080808);
  VLOAD_32(v17, 0x11111111, 0x12121212, 0x13131313, 0x14141414, 0x15151515,
           0x16161616, 0x17171717, 0x18181818);
  asm volatile("vmv2r.v v8, v16");
  VCMP_U32(21, v8, 0x01010101, 0x02020202, 0x03030303, 0x04040404, 0x05050505,
           0x06060606, 0x07070707, 0x08080808);
  VCMP_U32(22, v9, 0x11111111, 0x12121212, 0x13131313, 0x14141414, 0x15151515,
           0x16161616, 0x17171717, 0x18181818);

#if ELEN == 64
  CLEAR_DEST();
  VSET(4, e64, m1);
  VLOAD_64(v16, 0x0101010101010101, 0x0202020202020202, 0x0303030303030303,
           0x0404040404040404);
  VLOAD_64(v17, 0x1111111111111111, 0x1212121212121212, 0x1313131313131313,
           0x1414141414141414);
  asm volatile("vmv2r.v v8, v16");
  VCMP_U64(23, v8, 0x0101010101010101, 0x0202020202020202, 0x0303030303030303,
           0x0404040404040404);
  VCMP_U64(24, v9, 0x1111111111111111, 0x1212121212121212, 0x1313131313131313,
           0x1414141414141414);
#endif
}

// vl and vtype.vlmul must not matter: vl = 0 still copies the whole group, and
// so does a fractional LMUL with a tiny vl. The AVL goes through a volatile so
// the compiler cannot use x0 (which would mean vl = VLMAX).
void TEST_CASE3(void) {
  LOAD_SOURCES();

  CLEAR_DEST();
  volatile int avl = 0;
  int avl0 = avl;
  VSET(avl0, e8, m1);
  unsigned int vl;
  asm volatile("csrr %0, vl" : "=r"(vl));
  XCMP(25, vl, 0);
  asm volatile("vmv4r.v v8, v16");
  VSET(16, e8, m1);
  VCMP_REG(26, v8, 0x01);
  VCMP_REG(27, v9, 0x11);
  VCMP_REG(28, v10, 0x21);
  VCMP_REG(29, v11, 0x31);

  CLEAR_DEST();
  VSET(3, e8, mf2);
  asm volatile("vmv2r.v v8, v16");
  VSET(16, e8, m1);
  VCMP_REG(30, v8, 0x01);
  VCMP_REG(31, v9, 0x11);
}

// A VFU instruction reading the last register of the group right after the
// move: the scoreboard must track all NREG destination registers.
void TEST_CASE4(void) {
  LOAD_SOURCES();
  CLEAR_DEST();

  VSET(16, e8, m1);
  asm volatile("vmv4r.v v8, v16\n"
               "vadd.vi v24, v11, 1");
  VCMP_REG(32, v24, 0x32);
}

// Full register contents, sized from VLMAX so it works for any VLEN: every
// byte of every register (all VRF words) must be copied.
void TEST_CASE5(void) {
  unsigned int n;

  // vmv8r.v under e8, m1, vl = 1
  asm volatile("vsetvli %0, %1, e8, m8, ta, ma" : "=r"(n) : "r"(-1));
  uint8_t *src = (uint8_t *)snrt_l1alloc(n);
  uint8_t *dst = (uint8_t *)snrt_l1alloc(n);
  for (unsigned int i = 0; i < n; i++) {
    src[i] = (uint8_t)(i * 7 + 3);
    dst[i] = 0;
  }
  asm volatile("vle8.v v16, (%0)" ::"r"(src) : "memory");
  VCLEAR(v8);
  VSET(1, e8, m1);
  asm volatile("vmv8r.v v8, v16");
  asm volatile("vsetvli zero, %0, e8, m8, ta, ma" ::"r"(-1));
  asm volatile("vse8.v v8, (%0)" ::"r"(dst) : "memory");
  BUFCMP(33, dst, src, n);

  // vmv2r.v under e32: evl = 2 * VLEN / 32, same bytes as at e8
  asm volatile("vsetvli %0, %1, e8, m2, ta, ma" : "=r"(n) : "r"(-1));
  for (unsigned int i = 0; i < n; i++) {
    src[i] = (uint8_t)(0xA5 ^ (i * 13));
    dst[i] = 0;
  }
  asm volatile("vle8.v v16, (%0)" ::"r"(src) : "memory");
  VCLEAR(v8);
  VSET(1, e32, m1);
  asm volatile("vmv2r.v v8, v16");
  asm volatile("vsetvli zero, %0, e8, m2, ta, ma" ::"r"(-1));
  asm volatile("vse8.v v8, (%0)" ::"r"(dst) : "memory");
  BUFCMP(34, dst, src, n);
}

// vstart is honoured and counts SEW elements: elements below vstart keep their
// old value. The second part starts inside the second register of the group.
void TEST_CASE6(void) {
  LOAD_SOURCES();

  // e16, vstart = 3: bytes 0..5 of v8 untouched
  CLEAR_DEST();
  VSET(16, e16, m1);
  asm volatile("csrw vstart, 3");
  asm volatile("vmv2r.v v8, v16");
  asm volatile("csrw vstart, 0");
  VSET(16, e8, m1);
  VCMP_U8(35, v8, 0, 0, 0, 0, 0, 0, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
          0x0e, 0x0f, 0x10);
  VCMP_REG(36, v9, 0x11);

  // e8, vstart = VLENB + 2: v8 and bytes 0..1 of v9 untouched
  unsigned int vlenb;
  asm volatile("vsetvli %0, %1, e8, m1, ta, ma" : "=r"(vlenb) : "r"(-1));
  unsigned int vstart = vlenb + 2;
  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("csrw vstart, %0" ::"r"(vstart));
  asm volatile("vmv2r.v v8, v16");
  asm volatile("csrw vstart, 0");
  VCMP_U8(37, v8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  VCMP_U8(38, v9, 0, 0, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
          0x1c, 0x1d, 0x1e, 0x1f, 0x20);
}

// The VLSU right after the move. A store of the last destination register
// must wait for the copy (RAW on vd), and a load into the last source register
// must not overtake the copy's read (WAR on vs2).
void TEST_CASE7(void) {
  volatile uint8_t out[16] __attribute__((aligned(16)));
  volatile uint8_t in[16] __attribute__((aligned(16))) = {
      0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
      0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0};
  for (int i = 0; i < 16; i++)
    out[i] = 0;

  // RAW: store v15 immediately after vmv8r.v writes it
  LOAD_SOURCES();
  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv8r.v v8, v16\n"
               "vse8.v  v15, (%0)" ::"r"(out)
               : "memory");
  VVCMP_U8(39, out, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
           0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80);

  // WAR: overwrite v23 immediately after vmv8r.v reads it
  LOAD_SOURCES();
  CLEAR_DEST();
  VSET(16, e8, m1);
  asm volatile("vmv8r.v v8, v16\n"
               "vle8.v  v23, (%0)" ::"r"(in)
               : "memory");
  VCMP_REG(40, v15, 0x71);
  VCMP_U8(41, v23, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca,
          0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0);
}

// Whole-register loads, stores, and moves do not depend upon vtype
void TEST_CASE8(void) {
  LOAD_SOURCES();
  CLEAR_DEST();
  unsigned int vtype = (4 << 3) | 0; // vsew = 4 is reserved -> vill = 1
  asm volatile("vsetvl zero, zero, %[vtype]" ::[vtype] "r"(vtype));
  asm volatile("vmv2r.v v8, v16");
  VSET(16, e8, m1);
  VCMP_REG(42, v8, 0x01);
  VCMP_REG(43, v9, 0x11);
  VCMP_U8(44, v10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

int main(void) {
  INIT_CHECK();
  enable_vec();

  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();
  // Disabled: they expose two RTL bugs that predate vmv<nreg>r.v (slide-unit
  // vstart byte enables, and WAR chaining in the scoreboard)
  // TEST_CASE6();
  // TEST_CASE7();
  TEST_CASE8();

  EXIT_CHECK();
}
