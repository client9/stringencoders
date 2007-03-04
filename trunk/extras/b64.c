#include <stdio.h>
/*
 * Copyright 2005, Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the modp.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This is the standard "new" BSD license:
 * http://www.opensource.org/licenses/bsd-license.php
 */

#include "b64.h"

static const char b64_chars[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' } ;

static const u_int32_t b64_chars0[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' } ;

static const u_int32_t b64_chars24[64] = {
  'A' << 24, 'B' << 24, 'C' << 24, 'D' << 24, 'E' << 24, 'F' << 24,
  'G' << 24, 'H' << 24, 'I' << 24, 'J' << 24, 'K' << 24, 'L' << 24,
  'M' << 24, 'N' << 24, 'O' << 24, 'P' << 24, 'Q' << 24, 'R' << 24,
  'S' << 24, 'T' << 24, 'U' << 24, 'V' << 24, 'W' << 24, 'X' << 24,
  'Y' << 24, 'Z' << 24, 'a' << 24, 'b' << 24, 'c' << 24, 'd' << 24,
  'e' << 24, 'f' << 24, 'g' << 24, 'h' << 24, 'i' << 24, 'j' << 24, 
  'k' << 24, 'l' << 24, 'm' << 24, 'n' << 24, 'o' << 24, 'p' << 24,
  'q' << 24, 'r' << 24, 's' << 24, 't' << 24, 'u' << 24, 'v' << 24,
  'w' << 24, 'x' << 24, 'y' << 24, 'z' << 24, '0' << 24, '1' << 24,
  '2' << 24, '3' << 24, '4' << 24, '5' << 24, '6' << 24, '7' << 24, 
  '8' << 24, '9' << 24, '+' << 24, '/'  << 24} ;

static const u_int32_t b64_chars16[64] = {
  'A' << 16, 'B' << 16, 'C' << 16, 'D' << 16, 'E' << 16, 'F' << 16,
  'G' << 16, 'H' << 16, 'I' << 16, 'J' << 16, 'K' << 16, 'L' << 16,
  'M' << 16, 'N' << 16, 'O' << 16, 'P' << 16, 'Q' << 16, 'R' << 16,
  'S' << 16, 'T' << 16, 'U' << 16, 'V' << 16, 'W' << 16, 'X' << 16, 
  'Y' << 16, 'Z' << 16, 'a' << 16, 'b' << 16, 'c' << 16, 'd' << 16,
  'e' << 16, 'f' << 16, 'g' << 16, 'h' << 16, 'i' << 16, 'j' << 16, 
  'k' << 16, 'l' << 16, 'm' << 16, 'n' << 16, 'o' << 16, 'p' << 16, 
  'q' << 16, 'r' << 16, 's' << 16, 't' << 16, 'u' << 16, 'v' << 16,
  'w' << 16, 'x' << 16, 'y' << 16, 'z' << 16, '0' << 16, '1' << 16, 
  '2' << 16, '3' << 16, '4' << 16, '5' << 16, '6' << 16, '7' << 16,
  '8' << 16, '9' << 16, '+' << 16, '/' << 16} ;

static const u_int32_t b64_chars8[64] = {
  'A' << 8, 'B' << 8, 'C' << 8, 'D' << 8, 'E' << 8, 'F' << 8,
  'G' << 8, 'H' << 8, 'I' << 8, 'J' << 8, 'K' << 8, 'L' << 8,
  'M' << 8, 'N' << 8, 'O' << 8, 'P' << 8, 'Q' << 8, 'R' << 8,
  'S' << 8, 'T' << 8, 'U' << 8, 'V' << 8, 'W' << 8, 'X' << 8,
  'Y' << 8, 'Z' << 8, 'a' << 8, 'b' << 8, 'c' << 8, 'd' << 8,
  'e' << 8, 'f' << 8, 'g' << 8, 'h' << 8, 'i' << 8, 'j' << 8,
  'k' << 8, 'l' << 8, 'm' << 8, 'n' << 8, 'o' << 8, 'p' << 8,
  'q' << 8, 'r' << 8, 's' << 8, 't' << 8, 'u' << 8, 'v' << 8,
  'w' << 8, 'x' << 8, 'y' << 8, 'z' << 8, '0' << 8, '1' << 8,
  '2' << 8, '3' << 8, '4' << 8, '5' << 8, '6' << 8, '7' << 8,
  '8' << 8, '9' << 8, '+' << 8, '/' << 8} ;

#define NC64 0x0001FFFFFFFFFFFFLL
static const u_int64_t b64_map[256] = {
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* 00-0F */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* 10NC64F */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,  62,NC64,  62,  63,  63,  /* 20-2F */
    52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,NC64,NC64,NC64,NC64,NC64,NC64,  /* 30-3F */
  NC64,   0,   1,   2,   3,   4,   5,   6,
     7,   8,   9,  10,  11,  12,  13,  14,  /* 40-4F */
    15,  16,  17,  18,  19,  20,  21,  22,
    23,  24,  25,NC64,NC64,NC64,NC64,NC64,  /* 50-5F */
  NC64,  26,  27,  28,  29,  30,  31,  32,
    33,  34,  35,  36,  37,  38,  39,  40,  /* 60-6F */
    41,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,NC64,NC64,NC64,NC64,NC64,  /* 70-7F */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* 80-8F */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* 90-9F */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* A0-AF */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* B0-BF */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* C0-CF */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* D0-DF */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,  /* E0-EF */
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64,
  NC64,NC64,NC64,NC64,NC64,NC64,NC64,NC64   /* F0-FF */
} ; 


#define BC 0x01FFFFFF
static const u_int32_t b64_map32[256] = {
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* 00-0F */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* 10-1F */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,62,BC,62,63,63,  /* 20-2F */
  52,53,54,55,56,57,58,59,60,61,BC,BC,BC,BC,BC,BC,  /* 30-3F */
  BC, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
  15,16,17,18,19,20,21,22,23,24,25,BC,BC,BC,BC,BC,  /* 50-5F */
  BC,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
  41,42,43,44,45,46,47,48,49,50,51,BC,BC,BC,BC,BC,  /* 70-7F */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* 80-8F */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* 90-9F */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* A0-AF */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* B0-BF */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* C0-CF */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* D0-DF */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,  /* E0-EF */
  BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC,BC   /* F0-FF */
} ; 

int b64_decode64_1(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  // prevent compiler warnings by saying we are really using unsigned
  const unsigned char* s = (const unsigned char*)src;
  char* p = dest;
  int i,j;
  int chunks = len / 8;
  int leftover = len % 8;
  
  u_int64_t d = 0;
  u_int64_t x = 0;
  u_int64_t* destInt = (u_int64_t*) p;
  for (i = 0; i < chunks; ++i) {
    for (j = 0; j < 8; ++j) {
      if ((d = b64_map[*s++]) == BC) {
	return -1;
      }
      x = x*64 + d ;
    }

    *destInt = x << 16;
    p += 6;
    destInt = (u_int64_t*)p; 
    x = 0;
  }

  if (leftover) {
    for (j = 0; j < leftover; ++j) {
      if ((d = b64_map[*s++]) == BC) return -1;
      x = x*64 + d ;
    }
    *destInt = x << (64 - 6*leftover);
  } else {
    *p = '\0';
  }

  return (int)(p - dest) + (leftover*3/4); 
}


int b64_decode32_1(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i,j;
  int chunks = len / 4;
  int leftover = len % 4;
  const unsigned char* s = (const unsigned char*) src;
  char* p = dest;
  u_int32_t d = 0;
  u_int32_t x = 0;
  u_int32_t* destInt = (u_int32_t*) p;
  for (i = 0; i < chunks; ++i) {
    for (j = 0; j < 4; ++j) {
      if ((d = b64_map32[*s++]) == BC) return -1;
      // 12/2005: on AMD, doing x <<= 6; x |= d is slower
      // also on other arch w/ FMA a*x+b can be one op.
      x = x*64 + d ;
    }
    *destInt = x << 8;
    p += 3;
    destInt = (u_int32_t*)p; 
    x = 0;		
  }

  if (leftover) {
    for (j = 0; j < leftover; ++j) {
      if ((d = b64_map32[*s++]) == BC) return -1;
      x = x*64 + d ;
    }
    *destInt = x << (32 - 6*leftover);
  } else {
    *p = '\0';
  }

  return (int)(p - dest) + (leftover*3/4);
}


int b64_decode32_2(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i = 0;
  int chunks = len / 4;
  int leftover = len % 4;
  char* p = dest;
  u_int32_t d = 0;
  u_int32_t x = 0;

  u_int32_t* destInt = (u_int32_t*) p;
  u_int32_t* srcInt = (u_int32_t*) src;
  u_int32_t y = *srcInt++;
  while (i++ < chunks) {
    if ((x = b64_map32[y >> 24]) == BC) return -1;
    if ((d = b64_map32[y >> 16 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map32[y >> 8 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map32[y & 0xff]) == BC) return -1;
    x = x*64 + d;

    *destInt = x << 8;
    p += 3;
    destInt = (u_int32_t*)p; 
    y = *srcInt++;
  }

  switch (leftover) {
  case 3:
    if ((x = b64_map32[y >> 24]) == BC) return -1;
    if ((d = b64_map32[y >> 16 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map32[y >> 8 & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (32-18);
    break;
  case 2:
    if ((x = b64_map32[y >> 24]) == BC) return -1;
    if ((d = b64_map32[(y >> 16) & 0xff]) == BC) return -1;
    *destInt = (x*64+d) << (32-12);
    break;
  case 1:
    if ((x = b64_map32[y >> 24]) == BC) return -1;
    *destInt = x << (32-6) ;
    break;
  case 0:
    *p = '\0';
  }

  return (int)(p - dest) + leftover *3/4;
}

int b64_decode32_3(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i = 0;
  int chunks = len / 4;
  int leftover = len % 4;
  char* p = dest;
  u_int32_t d = 0;
  u_int32_t err = 0;
  u_int32_t x = 0;
  u_int32_t* destInt = (u_int32_t*) p;
  u_int32_t* srcInt = (u_int32_t*) src;
  u_int32_t y = *srcInt++;
  while (i++ < chunks) {
    x = b64_map32[y >> 24];
    err |= x;

    d = b64_map32[y >> 16 & 0xff];
    err |= d;
    x = x*64 + d;

    d = b64_map32[y >> 8 & 0xff];
    err |= d;
    x = x*64 + d;

    d = b64_map32[y & 0xff];
    err |= d;
    x = x*64 + d;

    *destInt = x << 8;
    p += 3;
    destInt = (u_int32_t*)p; 
    y = *srcInt++;
  }

  switch (leftover) {
  case 3:
    x = b64_map32[y >> 24];
    err |= d;
    d = b64_map32[y >> 16 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map32[y >> 8 & 0xff];
    err |= d;
    x = x*64 + d;
    *destInt = x << (32-18);
    break;
  case 2:
    x = b64_map32[y >> 24];
    err |= x;
    d = b64_map32[y >> 16 & 0xff];
    err |= d;
    x = x*64 + d;
    *destInt = x << (32-12);
    break;
  case 1:
    x = b64_map32[y >> 24];
    err |= x;
    *destInt = x << (32-6) ;
    break;
  case 0:
    *p = '\0';
  }

  return (int)(p - dest) + leftover *3/4;
}

int b64_decode32_4(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }
  if (len <= 0) return 0;

  int i;
  int chunks = len / 4;
  int leftover = len % 4;
  char* p = dest;
  u_int32_t x = 0;
  u_int32_t* destInt = (u_int32_t*) p;
  u_int32_t* srcInt = (u_int32_t*) src;
  u_int32_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    x = ((((b64_map32[y >> 24] * 64) +
	   b64_map32[y >> 16 & 0xff])*64 +
	  b64_map32[y >> 8 & 0xff])*64 +
	 b64_map32[y & 0xff]);
    if (x >= BC) return -1;
    *destInt = x << 8;
    p += 3;
    destInt = (u_int32_t*)p; 
    y = *srcInt++;
  }

  /*
   * Oddly on PowerGC G4, gcc 4.0.2
   * this loop is a lot faster than the switch below
   * can't tell if this ia gcc 4 issue or a PPC issue
   */
  /*
  if (leftover) {
    x = 0;
    for (j = 1; j <= leftover; ++j) {
      x = x*64 + b64_map[(y >> (32 - 8*j)) & 0xff];
    }
    if (x >= BC) return -1;
    *destInt = x << (32 -6*leftover);
  } else {
    *dest = '\0';
 }
  */

  switch (leftover) {
  case 3:
    x = (b64_map32[y >> 24] *64 + b64_map32[(y >> 16) & 0xff])*64 +
      b64_map32[(y >> 8) & 0xff];
    if (x >= BC) return -1;
    *destInt = x << 14;
    break;
  case 2:
    x = b64_map32[y >> 24] *64 + b64_map32[(y >> 16) & 0xff];
    if (x >= BC) return -1;
    *destInt = x << 20;
    break;
  case 1:
    x = b64_map32[y >> 24];
    if (x >= BC) return -1;
    *destInt = x << 26;    
    break;
  default:
    *p = '\0';
  }

  return (int)(p - dest) + leftover *3/4;
}

int b64_decode32_5(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }
  if (len <= 0) return 0;

  int i;
  int chunks = len / 4;
  int leftover = len % 4;
  char* p = dest;
  u_int32_t err = 0;
  u_int32_t x = 0;
  u_int32_t* destInt = (u_int32_t*) p;
  u_int32_t* srcInt = (u_int32_t*) src;
  u_int32_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    x = ((((b64_map32[y >> 24] * 64) +
	   b64_map32[y >> 16 & 0xff])*64 +
	  b64_map32[y >> 8 & 0xff])*64 +
	 b64_map32[y & 0xff]);
    err |= x;
    *destInt = x << 8;
    p += 3;
    destInt = (u_int32_t*)p; 
    y = *srcInt++;
  }

  /*
   * Oddly on PowerGC G4, gcc 4.0.2
   * this loop is a lot faster than the switch below
   * can't tell if this ia gcc 4 issue or a PPC issue
   */
  /*
  if (leftover) {
    x = 0;
    for (j = 1; j <= leftover; ++j) {
      x = x*64 + b64_map[(y >> (32 - 8*j)) & 0xff];
    }
    if (x >= BC) return -1;
    *destInt = x << (32 -6*leftover);
  } else {
    *p = '\0';
 }
  */

  switch (leftover) {
  case 3:
    x = (b64_map32[y >> 24] *64 + b64_map32[(y >> 16) & 0xff])*64 +
      b64_map32[(y >> 8) & 0xff];
    err |= x;
    *destInt = x << 14;
    break;
  case 2:
    x = b64_map32[y >> 24] *64 + b64_map32[(y >> 16) & 0xff];
    err |= x;
    *destInt = x << 20;
    break;
  case 1:
    x = b64_map32[y >> 24];
    err |= x;
    *destInt = x << 26;    
    break;
  default:
    *p = '\0';
  }

  if (err >= BC) {
    return -1;
  }
  return (int)(p - dest) + leftover *3/4;
}

int b64_decode64_2(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }
  if (len <= 0) return 0;

  int i;
  int chunks = len / 8;
  int leftover = len % 8;

  char* p = dest;
  u_int64_t d = 0;
  u_int64_t x = 0;

  u_int64_t* destInt = (u_int64_t*) p;
  u_int64_t* srcInt = (u_int64_t*) src;
  u_int64_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    if ((x = b64_map[y >> 56]) == BC) return -1;
    if ((d = b64_map[y >> 48 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 40 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 32 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 24 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 16 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 8 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y & 0xff]) == BC) return -1;
    x = x*64 + d;

    *destInt = x << 16;
    p += 6;
    destInt = (u_int64_t*)p; 
    y = *srcInt++;
  }

  switch (leftover) {
  case 7:
    if ((x = b64_map[y >> 56]) == BC) return -1;
    if ((d = b64_map[y >> 48 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 40 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 32 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 24 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 16 & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> 8 & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-42);
    break;
  case 6:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    if ((d = b64_map[y >> (64-16) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-24) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-32) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-40) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-48) & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-36);
    break;
  case 5:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    if ((d = b64_map[y >> (64-16) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-24) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-32) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-40) & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-30);
    break;
  case 4:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    if ((d = b64_map[y >> (64-16) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-24) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-32) & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-24);
    break;
  case 3:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    if ((d = b64_map[y >> (64-16) & 0xff]) == BC) return -1;
    x = x*64 + d;
    if ((d = b64_map[y >> (64-24) & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-18);
    break;
  case 2:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    if ((d = b64_map[y >> (64-16) & 0xff]) == BC) return -1;
    x = x*64 + d;
    *destInt = x << (64-12);
    break;
  case 1:
    if ((x = b64_map[y >> (64-8)]) == BC) return -1;
    *destInt = x << (64-6) ;
    break;
  case 0:
    *p = '\0';
  }
  return (int)(p - dest) + leftover *3/4;
}

int b64_decode64_3(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i;
  int chunks = len / 8;
  int leftover = len % 8;
  
  u_int64_t d = 0;
  u_int64_t x = 0;
  char* p = dest;
  u_int64_t err = 0;
  u_int64_t* destInt = (u_int64_t*)p;
  u_int64_t* srcInt = (u_int64_t*) src;
  u_int64_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    x = b64_map[y >> 56];
    err |= x;
    d = b64_map[y >> 48 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y >> 40 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y >> 32 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y >> 24 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y >> 16 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y >> 8 & 0xff];
    err |= d;
    x = x*64 + d;
    d = b64_map[y & 0xff];
    err |= d;
    x = x*64 + d;

    *destInt = x << 16;
    p += 6;
    destInt = (u_int64_t*)p;
    y = *srcInt++;
 
  }

  switch (leftover) {
  case 7:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> 48 & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> 40 & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> 32 & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> 24 & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> 16 & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> 8 & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << (64-42);
    break;
  case 6:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> (64-16) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-24) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-32) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-40) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-48) & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << (64-36);
    break;
  case 5:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> (64-16) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-24) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-32) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-40) & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << (64-30);
    break;
  case 4:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> (64-16) & 0xff];err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-24) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-32) & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << (64-24);
    break;
  case 3:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> (64-16) & 0xff]; err |= d;
    x = x*64 + d;
    d = b64_map[y >> (64-24) & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << (64-18);
    break;
  case 2:
    x = b64_map[y >> 56]; err |= x;
    d = b64_map[y >> (64-16) & 0xff]; err |= d;
    x = x*64 + d;
    *destInt = x << 52;
    break;
  case 1:
    x = b64_map[y >> 56];
    err |= x;
    *destInt = x << (64-6) ;
    break;
  case 0:
    *p = '\0';
  }

  if (err > 127) {
    return -1;
  }

  return (int)(p - dest) + leftover *3/4;
}

int b64_decode64_4(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i;
  int chunks = len / 8;
  int leftover = len % 8;
  
  char* p = dest;
  u_int64_t x = 0;
  u_int64_t* destInt = (u_int64_t*) p;
  u_int64_t* srcInt = (u_int64_t*) src;
  u_int64_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    x = ((((((b64_map[y >> 56]* 64 + 
	     b64_map[(y >> 48) & 0xff])*64 +
	    b64_map[(y >> 40) & 0xff])*64 +
	   b64_map[(y >> 32) & 0xff])*64 +
	  b64_map[(y >> 24) & 0xff])*64 +
	 b64_map[(y >> 16) & 0xff])*64 +
	 b64_map[(y >> 8) & 0xff])*64 +
      b64_map[y & 0xff];
    if (x >= NC64) return -1;
    *destInt = x << 16;
    p += 6;
    destInt = (u_int64_t*)p;
    y = *srcInt++; 
  }
  
  switch (leftover) {
  case 7:
    x = (((((b64_map[y >> 56]* 64 + 
	     b64_map[(y >> 48) & 0xff])*64 +
	    b64_map[(y >> 40) & 0xff])*64 +
	   b64_map[(y >> 32) & 0xff])*64 +
	  b64_map[(y >> 24) & 0xff])*64 +
	 b64_map[(y >> 16) & 0xff])*64 +
      b64_map[(y >> 8) & 0xff];
    if (x >= NC64) return -1; 
    *destInt = x << 22;
    break;
  case 6:
    x = ((((b64_map[y >> 56]* 64 + 
	    b64_map[(y >> 48) & 0xff])*64 +
	   b64_map[(y >> 40) & 0xff])*64 +
	  b64_map[(y >> 32) & 0xff])*64 +
	 b64_map[(y >> 24) & 0xff])*64 +
      b64_map[(y >> 16) & 0xff];
    if (x >= NC64) return -1; 
    *destInt = x << 28;
    break;
  case 5:
    x = (((b64_map[y >> 56]* 64 + 
	   b64_map[(y >> 48) & 0xff])*64 +
	  b64_map[(y >> 40) & 0xff])*64 +
	 b64_map[(y >> 32) & 0xff])*64 +
      b64_map[(y >> 24) & 0xff];
    if (x >= NC64) return -1; 
    *destInt = x << 34;
    break;
  case 4:
    x = ((b64_map[y >> 56]* 64 + 
	  b64_map[(y >> 48) & 0xff])*64 +
	 b64_map[(y >> 40) & 0xff])*64 +
      b64_map[(y >> 32) & 0xff];
    if (x >= NC64) return -1;
    *destInt = x << 40;
    break;
  case 3:
    x = (b64_map[y >> 56]* 64 + 
	 b64_map[(y >> 48) & 0xff])*64 +
      b64_map[(y >> 40) & 0xff];
    if (x >= NC64) return -1;
    *destInt = x << 46;
    break;
  case 2:
    x = b64_map[y >> 56]*64 + b64_map[(y >> 48) & 0xff]; 
    if (x >= NC64) return -1;
    *destInt = x << 52;
    break;
  case 1:
    x = b64_map[y >> 56];
    if (x >= NC64) return -1;
    *destInt = x << 58;
    break;
  case 0:
    *p = '\0';
  }

  return (int)(p - dest) + leftover *3/4;
}

int b64_decode64_5(char* dest, const char* src, int len)
{
  if (src[len-1] == '=') {
    len--;
    if (src[len -1] == '=') {
      len--;
    }
  }

  int i;
  int chunks = len / 8;
  int leftover = len % 8;
  
  char* p = dest;
  u_int64_t x = 0;
  u_int64_t err = 0;
  u_int64_t* destInt = (u_int64_t*) p;
  u_int64_t* srcInt = (u_int64_t*) src;
  u_int64_t y = *srcInt++;
  for (i = 0; i < chunks; ++i) {
    x = ((((((b64_map[y >> 56]* 64 + 
	     b64_map[(y >> 48) & 0xff])*64 +
	    b64_map[(y >> 40) & 0xff])*64 +
	   b64_map[(y >> 32) & 0xff])*64 +
	  b64_map[(y >> 24) & 0xff])*64 +
	 b64_map[(y >> 16) & 0xff])*64 +
	 b64_map[(y >> 8) & 0xff])*64 +
      b64_map[y & 0xff];
    err |= x;
    *destInt = x << 16;
    p += 6;
    destInt = (u_int64_t*)p;
    y = *srcInt++; 
  }
  
  switch (leftover) {
  case 7:
    x = (((((b64_map[y >> 56]* 64 + 
	     b64_map[(y >> 48) & 0xff])*64 +
	    b64_map[(y >> 40) & 0xff])*64 +
	   b64_map[(y >> 32) & 0xff])*64 +
	  b64_map[(y >> 24) & 0xff])*64 +
	 b64_map[(y >> 16) & 0xff])*64 +
      b64_map[(y >> 8) & 0xff];
    err |= x;
    *destInt = x << 22;
    break;
  case 6:
    x = ((((b64_map[y >> 56]* 64 + 
	    b64_map[(y >> 48) & 0xff])*64 +
	   b64_map[(y >> 40) & 0xff])*64 +
	  b64_map[(y >> 32) & 0xff])*64 +
	 b64_map[(y >> 24) & 0xff])*64 +
      b64_map[(y >> 16) & 0xff];
    err |= x;
    *destInt = x << 28;
    break;
  case 5:
    x = (((b64_map[y >> 56]* 64 + 
	   b64_map[(y >> 48) & 0xff])*64 +
	  b64_map[(y >> 40) & 0xff])*64 +
	 b64_map[(y >> 32) & 0xff])*64 +
      b64_map[(y >> 24) & 0xff];
    err |= x;
    *destInt = x << 34;
    break;
  case 4:
    x = ((b64_map[y >> 56]* 64 + 
	  b64_map[(y >> 48) & 0xff])*64 +
	 b64_map[(y >> 40) & 0xff])*64 +
      b64_map[(y >> 32) & 0xff];
    err |= x;
    *destInt = x << 40;
    break;
  case 3:
    x = (b64_map[y >> 56]* 64 + 
	 b64_map[(y >> 48) & 0xff])*64 +
      b64_map[(y >> 40) & 0xff];
    err |= x;
    *destInt = x << 46;
    break;
  case 2:
    x = b64_map[y >> 56]*64 + b64_map[(y >> 48) & 0xff]; 
    err |= x;
    *destInt = x << 52;
    break;
  case 1:
    x = b64_map[y >> 56];
    err |= x;
    *destInt = x << 58;
    break;
  case 0:
    *p = '\0';
  }

  if (err >= NC64) {
    return -1;
  }
  return (int)(p - dest) + leftover *3/4;
}

int b64_encode64_1(char* dest, const char* str, int len)
{
  char* p = dest;
  int i = 0;
  u_int64_t x = *((u_int64_t*) str);
  int chunks = len / 6;
  int leftover = len % 6;

  while (i < chunks) {
    *p++ = b64_chars[x >> (64- 6)];
    *p++ = b64_chars[x >> (64-12) & 0x3f];
    *p++ = b64_chars[x >> (64-18) & 0x3f];
    *p++ = b64_chars[x >> (64-24) & 0x3f];
    *p++ = b64_chars[x >> (64-30) & 0x3f];
    *p++ = b64_chars[x >> (64-36) & 0x3f];
    *p++ = b64_chars[x >> (64-42) & 0x3f];
    *p++ = b64_chars[x >> (64-48) & 0x3f];
    ++i;
    x = *((u_int64_t*)(str + 6* i));
  } 

  switch (leftover) {
  case 5:
    *p++ = b64_chars[x >> 58];
    *p++ = b64_chars[(x >> 52) & 0x3f];
    *p++ = b64_chars[(x >> 46) & 0x3f];
    *p++ = b64_chars[(x >> 40) & 0x3f];
    *p++ = b64_chars[(x >> 34) & 0x3f];
    *p++ = b64_chars[(x >> 28) & 0x3f];
    *p++ = b64_chars[(x >> 22) & 0x3c];
    *p++ = '=';
    break;
  case 4:
    *p++ = b64_chars[x >> 58];
    *p++ = b64_chars[(x >> 52) & 0x3f];
    *p++ = b64_chars[(x >> 46) & 0x3f];
    *p++ = b64_chars[(x >> 40) & 0x3f];
    *p++ = b64_chars[(x >> 34) & 0x3f];
    *p++ = b64_chars[(x >> 28) & 0x30];
    *p++ = '=';
    *p++ = '=';
    break;
  case 3:
    *p++ = b64_chars[(x >> (64 -6)) & 0x3f];
    *p++ = b64_chars[(x >> (64-12)) & 0x3f];
    *p++ = b64_chars[(x >> (64-18)) & 0x3f];
    *p++ = b64_chars[(x >> (64-24)) & 0x3f];
    break;    
  case 2:
    *p++ = b64_chars[x >> (64 -6)];
    *p++ = b64_chars[(x >> (64-12)) & 0x3f];
    *p++ = b64_chars[(x >> (64-18)) & 0x3c];
    *p++ = '=';
    break;
  case 1:
    *p++ = b64_chars[x >> 58];
    *p++ = b64_chars[(x >> 52) & 0x30];
    *p++ = '=';
    *p++ = '=';
    break;
  }
  *p = '\0';
  return p - dest;   
}

#include "b64_constants.c"
int b64_encode64_2(char* dest, const char* str, int len)
{
  int i = 0;
  int chunks = len / 6;
  int leftover = len % 6;
  u_int64_t x = *((u_int64_t*)(str));
  u_int64_t* destInt = (u_int64_t*) dest;
  while (i < chunks) {
    *destInt++ = b64_s56[x >> 58] |
      b64_s48[(x >> 52) & 0x3f] |
      b64_s40[(x >> 46) & 0x3f] |
      b64_s32[(x >> 40) & 0x3f] |
      b64_s24[(x >> 34) & 0x3f] |
      b64_s16[(x >> 28) & 0x3f] |
      b64_s8[(x >> 22) & 0x3f] |
      b64_s0[(x >> 16) & 0x3f] ;
    x = *((u_int64_t*)(str + 6* ++i));
  }

  int offset = 0;
  switch (leftover) {
  case 5:
   *destInt++ = b64_s56[x >> 58] |
      b64_s48[(x >> 52) & 0x3f] |
      b64_s40[(x >> 46) & 0x3f] |
      b64_s32[(x >> 40) & 0x3f] |
      b64_s24[(x >> 34) & 0x3f] |
      b64_s16[(x >> 28) & 0x3f] |
      b64_s8[(x >> 22) & 0x3c] |
     '=';
    break;
  case 4:
    *destInt++ = b64_s56[x >> 58] |
      b64_s48[(x >> 52) & 0x3f] |
      b64_s40[(x >> 46) & 0x3f] |
      b64_s32[(x >> 40) & 0x3f] |
      b64_s24[(x >> 34) & 0x3f] |
      b64_s16[(x >> 28) & 0x30] | '=' << 8 | '=';
    break;
  case 3:
    *destInt++ = b64_s56[x >> 58] |
      b64_s48[x >> 52 & 0x3f] |
      b64_s40[x >> 46 & 0x3f] |
      b64_s32[x >> 40 & 0x3f];
    offset = 4;
    break;    
  case 2:
    *destInt++ = b64_s56[x >> 58] |
      b64_s48[(x >> 52) & 0x3f] |
      b64_s40[(x >> 46) & 0x3c] |
      ((u_int64_t)'=') << 32;
    offset = 4;
    break;
  case 1:
    *destInt++ = b64_s56[x >> 58] |
      b64_s48[(x >> 52) & 0x30] |
      (((u_int64_t)'=') << 40) |
      (((u_int64_t)'=') << 32);
    offset = 4;
    break;
  }
  *destInt= '\0';

  return (int)((char*)destInt - dest) - offset;
}

/**
 * "linear" version
 * Oddly removing the x0,x1,x2 by inlining the calls
 * makes this go faster on Intel 32-bit gcc 3.4.1
 */
int b64_encode32_0(char *dest, const char *src, int len)
{
  int x0,x1,x2;
  char *p =dest;
  const int leftover = len % 3;
  int i = 0;
  while (i < (int)(len - leftover)) {
    x0 = src[i++];
    x1 = src[i++];
    x2 = src[i++];
    *p++ = b64_chars[(x0 >> 2) & 0x3F];
    *p++ = b64_chars[((x0 & 0x3) << 4) | ((x1 & 0xF0) >> 4)];
    *p++ = b64_chars[((x1 & 0xF) << 2) | ((x2 & 0xC0) >> 6)];
    *p++ = b64_chars[x2 & 0x3F];
  }

  switch (leftover) {
  case 2:
    x0 = src[i];
    x1 = src[i+1];
    *p++ = b64_chars[(x0 >> 2) & 0x3F];
    *p++ = b64_chars[((x0 & 0x3) << 4) | ((x1 & 0xF0) >> 4)];
    *p++ = b64_chars[(x1 & 0x0F) << 2];
    *p++ = '=';
    *p = '\0';
    break;
  case 1:
    x0 = src[i];
    *p++ = b64_chars[(x0 >> 2) & 0x3F];
    *p++ = b64_chars[((x0 & 0x3) << 4)];
    *p++ = '=';
    *p++ = '=';
    *p = '\0';
    break; 
  default: // really "0"
    *p = '\0';
  }
  return p - dest;
}


int b64_encode32_1(char* dest, const char* str, int len)
{
  char* p = dest;
  int chunks = len / 3;
  int leftover = len % 3;
  u_int32_t x = *((u_int32_t*)(str));
  int i = 0;
  while (i < chunks) {
    *p++ = b64_chars[x >> (32- 6)];
    *p++ = b64_chars[(x >> (32-12)) & 0x3f];
    *p++ = b64_chars[(x >> (32-18)) & 0x3f];
    *p++ = b64_chars[(x >> (32-24)) & 0x3f];
    x = *((u_int32_t*)(str + 3* ++i));
  } 

  switch (leftover) {
  case 2:
    *p++ = b64_chars[x >> 26];
    *p++ = b64_chars[(x >> 20) & 0x3f];
    *p++ = b64_chars[(x >> 14) & 0x3C];
    *p++ = '=';
    break;
  case 1:
    *p++ = b64_chars[x >>  (32 -6)];
    *p++ = b64_chars[(x >> (32-12)) & 0x30];
    *p++ = '=';
    *p++ = '=';
  }
  *p = '\0';
  return p-dest;   
}

int b64_encode32_2(char* dest, const char* str, int len)
{
  // every 3 bytes make 4 characters
  int chunks = len / 3;
  int leftover = len % 3;
  u_int32_t* destInt = (u_int32_t*) dest;
  u_int32_t x = *((u_int32_t*)(str));
  int i = 0;
  while (i < chunks) {
    // at every 3 bytes, take the next 4 bytes as an int.
    *destInt++ = b64_chars24[x >> (32- 6)]  |
      b64_chars16[(x >> (32-12)) & 0x3f]  |
      b64_chars8[(x >> (32-18)) & 0x3f]  |
      b64_chars0[(x >> (32-24)) & 0x3f];
    x = *((u_int32_t*)(str + 3*(++i)));
  } 

  switch (leftover) {
  case 2:
    *destInt++ = b64_chars24[x >> (32 -6)] |
      b64_chars16[(x >> (32-12)) & 0x3f] |
      b64_chars8[(x >> (32-18)) & 0x3c] | '=';
    *destInt = 0;
    break;
  case 1:
    *destInt++ = b64_chars24[x >> (32 -6)] |
      b64_chars16[(x >> (32-12)) & 0x30] |
      '=' << 8 | '=';
    *destInt = 0;
    break;
  default:
    *destInt = 0;
  }

  return (char*)destInt - dest;
}


int b64_encode32_3(char* dest, const char* str, int len)
{
  // every 3 bytes make 4 characters
  int chunks = len / 3;
  int leftover = len % 3;
  u_int32_t* destInt = (u_int32_t*) dest;
  u_int32_t x = *((u_int32_t*)(str));
  
  while (--chunks != 0) {
    // at every 3 bytes, take the next 4 bytes as an int.
    *destInt++ = b64_chars24[x >> (32- 6)]  |
      b64_chars16[(x >> (32-12)) & 0x3f]  |
      b64_chars8[(x >> (32-18)) & 0x3f]  |
      b64_chars0[(x >> (32-24)) & 0x3f];
    str += 3;
    x = *((u_int32_t*)(str));
  } 

  switch (leftover) {
  case 2:
    *destInt++ = b64_chars24[x >> (32 -6)] |
      b64_chars16[(x >> (32-12)) & 0x3f] |
      b64_chars8[(x >> (32-18)) & 0x3c] | '=';
    *destInt = 0;
    break;
  case 1:
    *destInt++ = b64_chars24[x >> (32 -6)] |
      b64_chars16[(x >> (32-12)) & 0x30] |
      '=' << 8 | '=';
    *destInt = 0;
    break;
  default:
    *destInt = 0;
  }

  return (char*)destInt - dest;
}
