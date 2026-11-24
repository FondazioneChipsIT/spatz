// Copyright 2026 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Check NaN-boxing of narrow FP loads (flw/flh/flb) in the LSU return path,
// and that integer loads are unaffected. The box bits are observed by reading
// the FPR back wider than the load wrote it.

#include "vector_macros.h"

// flw must NaN-box FPR bits [63:32] to all 1s; fsd exposes them.
// Only meaningful (and fsd only legal) on RVD configs.
#if ELEN == 64
void TEST_CASE1(void) {
  volatile uint32_t src;
  volatile uint32_t dst[2] __attribute__((aligned(8)));
  src = 0x3F800000; // 1.0 (fp32)
  dst[0] = 0;
  dst[1] = 0;
  asm volatile("fence\n"
               "flw fa0, 0(%[src])\n"
               "fsd fa0, 0(%[dst])\n"
               "fence\n"
               :
               : [src] "r"(&src), [dst] "r"(dst)
               : "fa0", "memory");
  XCMP(1, dst[0], (uint32_t)0x3F800000);
  XCMP(2, dst[1], (uint32_t)0xFFFFFFFF);
}
#endif

// flh must NaN-box FPR bits [63:16] to all 1s; fmv.x.s exposes [31:16]
void TEST_CASE2(void) {
  volatile uint32_t src = 0x00003C00; // 1.0 (fp16) in the low halfword
  register const volatile uint32_t *addr asm("a0") = &src;
  register uint32_t out asm("a1") = 0;
  asm volatile("fence\n"
               "flh fa0, 0(a0)\n"
               "fmv.x.s a1, fa0\n"
               : "+r"(out)
               : "r"(addr)
               : "fa0", "memory");
  XCMP(3, out, (uint32_t)0xFFFF3C00);
}

// flb must NaN-box FPR bits [63:8] to all 1s; fmv.x.s exposes [31:8]
void TEST_CASE3(void) {
  volatile uint32_t src = 0x00000038; // 1.0 (fp8) in the low byte
  register const volatile uint32_t *addr asm("a0") = &src;
  register uint32_t out asm("a1") = 0;
  asm volatile("fence\n"
               "flb fa0, 0(a0)\n"
               "fmv.x.s a1, fa0\n"
               : "+r"(out)
               : "r"(addr)
               : "fa0", "memory");
  XCMP(4, out, (uint32_t)0xFFFFFF38);
}

// Integer loads go through the same rewritten expression with NaNBox=0 and
// must keep sign/zero-extending as before
void TEST_CASE4(void) {
  volatile uint32_t src = 0x00008080; // sign bits set for both lh and lb
  uint32_t h, hu, b, bu;
  asm volatile("lh  %[h],  0(%[src])\n"
               "lhu %[hu], 0(%[src])\n"
               "lb  %[b],  0(%[src])\n"
               "lbu %[bu], 0(%[src])\n"
               : [h] "=r"(h), [hu] "=r"(hu), [b] "=r"(b), [bu] "=r"(bu)
               : [src] "r"(&src));
  XCMP(5, h, (uint32_t)0xFFFF8080);
  XCMP(6, hu, (uint32_t)0x00008080);
  XCMP(7, b, (uint32_t)0xFFFFFF80);
  XCMP(8, bu, (uint32_t)0x00000080);
}

// Full-width fld control: value passes through unmodified
#if ELEN == 64
void TEST_CASE5(void) {
  volatile uint32_t src[2] __attribute__((aligned(8)));
  volatile uint32_t dst[2] __attribute__((aligned(8)));
  src[0] = 0x00000000; // 1.0 (fp64) = 0x3FF0000000000000
  src[1] = 0x3FF00000;
  dst[0] = 0;
  dst[1] = 0;
  asm volatile("fence\n"
               "fld fa0, 0(%[src])\n"
               "fsd fa0, 0(%[dst])\n"
               "fence\n"
               :
               : [src] "r"(src), [dst] "r"(dst)
               : "fa0", "memory");
  XCMP(9, dst[0], (uint32_t)0x00000000);
  XCMP(10, dst[1], (uint32_t)0x3FF00000);
}
#endif

// fmv.{w,h,b}.x must NaN-box like the narrow loads: the integer value goes to
// the low bits and every FPR bit above it is set to 1.

// fmv.w.x must NaN-box FPR bits [63:32]; fsd exposes them (RVD only)
#if ELEN == 64
void TEST_CASE6(void) {
  volatile uint32_t dst[2] __attribute__((aligned(8)));
  dst[0] = 0;
  dst[1] = 0;
  asm volatile("fmv.w.x fa0, %[src]\n"
               "fsd fa0, 0(%[dst])\n"
               "fence\n"
               :
               : [src] "r"(0x3F800000), [dst] "r"(dst)
               : "fa0", "memory");
  XCMP(11, dst[0], (uint32_t)0x3F800000);
  XCMP(12, dst[1], (uint32_t)0xFFFFFFFF);
}
#endif

// fmv.h.x must take only rs1[15:0] and NaN-box [FLEN-1:16]; fmv.x.w exposes
// [31:16]. The junk in rs1[31:16] must not leak into the FPR.
void TEST_CASE7(void) {
  uint32_t out;
  asm volatile("fmv.h.x fa0, %[src]\n"
               "fmv.x.w %[out], fa0\n"
               : [out] "=r"(out)
               : [src] "r"(0x12343C00)
               : "fa0");
  XCMP(13, out, (uint32_t)0xFFFF3C00);
}

// fmv.b.x must take only rs1[7:0] and NaN-box [FLEN-1:8]; fmv.x.w exposes
// [31:8]. The junk in rs1[31:8] must not leak into the FPR.
void TEST_CASE8(void) {
  uint32_t out;
  asm volatile("fmv.b.x fa0, %[src]\n"
               "fmv.x.w %[out], fa0\n"
               : [out] "=r"(out)
               : [src] "r"(0x1234563C)
               : "fa0");
  XCMP(14, out, (uint32_t)0xFFFFFF3C);
}

// fmv.h.x upper bits [63:32] must also be boxed; fsd exposes them (RVD only)
#if ELEN == 64
void TEST_CASE9(void) {
  volatile uint32_t dst[2] __attribute__((aligned(8)));
  dst[0] = 0;
  dst[1] = 0;
  asm volatile("fmv.h.x fa0, %[src]\n"
               "fsd fa0, 0(%[dst])\n"
               "fence\n"
               :
               : [src] "r"(0x00003C00), [dst] "r"(dst)
               : "fa0", "memory");
  XCMP(15, dst[0], (uint32_t)0xFFFF3C00);
  XCMP(16, dst[1], (uint32_t)0xFFFFFFFF);
}
#endif

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();

#if ELEN == 64
  TEST_CASE1();
#endif
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
#if ELEN == 64
  TEST_CASE5();
#endif

  // fmv.{w,h,b}.x NaN-boxing
#if ELEN == 64
  TEST_CASE6();
#endif
  TEST_CASE7();
  TEST_CASE8();
#if ELEN == 64
  TEST_CASE9();
#endif

  EXIT_CHECK();
}
