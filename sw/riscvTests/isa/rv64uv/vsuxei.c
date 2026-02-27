// Copyright 2026 ETH Zurich, University of Bologna and Fondazione Chips-IT.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Author: Riccardo Giunti, Fondazione Chips-IT

#include "vector_macros.h"

//#define AXI_DWIDTH 128

#define INIT 98

// Naive test: EEW Destination == EEW indexes
void TEST_CASE1(void) {
  volatile uint8_t BUFFER_O8[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};

  VSET(12, e8, m1);
  VLOAD_8(v1, 0xd3, 0x40, 0xd1, 0x84, 0x48, 0x88, 0x88, 0xae, 0x91, 0x02, 0x59,
          0x89);
  VLOAD_8(v2, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13, 15);
  asm volatile("vsuxei8.v v1, (%0), v2" ::"r"(&BUFFER_O8[0]));
  VVCMP_U8(1, BUFFER_O8, INIT, 0xd3, 0x40, 0xd1, 0x84, 0x48, INIT, 0x88, 0x88,
           0xae, INIT, 0x91, 0x02, 0x59, INIT, 0x89);


  volatile uint16_t BUFFER_O16[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};

  VSET(12, e16, m1);
  VLOAD_16(v1, 0xbbd3, 0x3840, 0x8cd1, 0x9384, 0x7548, 0x9388, 0x8188, 0x11ae,
           0x4891, 0x4902, 0x8759, 0x1989);
  VLOAD_16(v2, 2, 4, 6, 8, 10, 14, 16, 18, 22, 24, 26, 30);
  asm volatile("vsuxei16.v v1, (%0), v2" ::"r"(&BUFFER_O16[0]));
  VVCMP_U16(2, BUFFER_O16, INIT, 0xbbd3, 0x3840, 0x8cd1, 0x9384, 0x7548, INIT,
            0x9388, 0x8188, 0x11ae, INIT, 0x4891, 0x4902, 0x8759, INIT, 0x1989);


  volatile uint32_t BUFFER_O32[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};

  VSET(8, e32, m1);
  VLOAD_32(v1, 0xf9aa71f0, 0xa11a9384, 0x99991348, 0x9fa831c7, 0x38197598,
           0x81937598, 0x18747547, 0x3eeeeeee);
  VLOAD_32(v2, 4, 8, 12, 16, 20, 28, 32, 36);
  asm volatile("vsuxei32.v v1, (%0), v2" ::"r"(&BUFFER_O32[0]));
  VVCMP_U32(3, BUFFER_O32, INIT, 0xf9aa71f0, 0xa11a9384, 0x99991348, 0x9fa831c7,
            0x38197598, INIT, 0x81937598, 0x18747547, 0x3eeeeeee, INIT,
            INIT, INIT, INIT, INIT, INIT);
}

// EEW Destination > EEW indexes
void TEST_CASE3(void) {

  volatile uint16_t BUFFER_O16[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};
  
  VSET(2, e16, m1);
  VLOAD_16(v1, 0xbbd3, 0x1989);
  VLOAD_8(v2, 2, 30);
  asm volatile("vsuxei8.v v1, (%0), v2" ::"r"(&BUFFER_O16[0]));
  VVCMP_U16(4, BUFFER_O16, INIT, 0xbbd3, INIT, INIT, INIT, INIT, INIT, INIT,
            INIT, INIT, INIT, INIT, INIT, INIT, INIT, 0x1989);
  
  
  volatile uint32_t BUFFER_O32[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};
  
  VSET(2, e32, m1);
  VLOAD_32(v1, 0xf9aa71f0, 0x89139848);
  VLOAD_16(v2, 4, 60);
  asm volatile("vsuxei16.v v1, (%0), v2" ::"r"(&BUFFER_O32[0]));
  VVCMP_U32(5, BUFFER_O32, INIT, 0xf9aa71f0, INIT, INIT, INIT, INIT, INIT, INIT,
            INIT, INIT, INIT, INIT, INIT, INIT, INIT, 0x89139848);
  
  volatile uint64_t BUFFER_O64[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};
  
  VSET(2, e64, m1);
  VLOAD_64(v1, 0xf9aa71f0c394bbd3, 0x8913984898951989);
  VLOAD_32(v2, 8, 120);
  asm volatile("vsuxei32.v v1, (%0), v2" ::"r"(&BUFFER_O64[0]));
  VVCMP_U64(6, BUFFER_O64, INIT, 0xf9aa71f0c394bbd3, INIT, INIT, INIT, INIT, 
            INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT, 0x8913984898951989);
}

// EEW Destination < EEW indexes
void TEST_CASE4(void) {
  
  volatile uint8_t BUFFER_O8[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};
  
  VSET(2, e8, m1);
  VLOAD_8(v1, 0xd3, 0x89);
  VLOAD_16(v2, 1, 15);
  asm volatile("vsuxei16.v v1, (%0), v2" ::"r"(&BUFFER_O8[0]));
  VVCMP_U8(7, BUFFER_O8, INIT, 0xd3, INIT, INIT, INIT, INIT, INIT, INIT,
           INIT, INIT, INIT, INIT, INIT, INIT, INIT, 0x89);
  
  volatile uint16_t BUFFER_O16[] = {
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT,
    INIT, INIT, INIT, INIT, INIT, INIT, INIT, INIT};
  
  VSET(2, e16, m1);
  VLOAD_16(v1, 0xbbd3, 0x1989);
  VLOAD_32(v2, 2, 30);
  asm volatile("vsuxei32.v v1, (%0), v2" ::"r"(&BUFFER_O16[0]));
  VVCMP_U16(8, BUFFER_O16, INIT, 0xbbd3, INIT, INIT, INIT, INIT, INIT, INIT,
            INIT, INIT, INIT, INIT, INIT, INIT, INIT, 0x1989);
}

int main(void) {
  INIT_CHECK();
  enable_vec();

  TEST_CASE1();
  //TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();

  EXIT_CHECK();
}