/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

// Copyright 2006-2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_CONVERSIONS_H_
#define V8_CONVERSIONS_H_

namespace v8 {
namespace internal {

// The fast double-to-int conversion routine does not guarantee
// rounding towards zero.
// The result is unspecified if x is infinite or NaN, or if the rounded
// integer value is outside the range of type int.
static inline int FastD2I(double x);


static inline double FastI2D(int x) {
  // There is no rounding involved in converting an integer to a
  // double, so this code should compile to a few instructions without
  // any FPU pipeline stalls.
  return static_cast<double>(x);
}


static inline double FastUI2D(unsigned x) {
  // There is no rounding involved in converting an unsigned integer to a
  // double, so this code should compile to a few instructions without
  // any FPU pipeline stalls.
  return static_cast<double>(x);
}


// This function should match the exact semantics of ECMA-262 9.4.
static inline double DoubleToInteger(double x);


// This function should match the exact semantics of ECMA-262 9.5.
static inline int32_t DoubleToInt32(double x);


// This function should match the exact semantics of ECMA-262 9.6.
static inline uint32_t DoubleToUint32(double x) {
  return static_cast<uint32_t>(DoubleToInt32(x));
}


// Returns the value (0 .. 15) of a hexadecimal character c.
// If c is not a legal hexadecimal character, returns a value < 0.
int HexValue(uc32 c);


// Enumeration for allowing octals and ignoring junk when converting
// strings to numbers.
enum ConversionFlags {
  NO_FLAGS = 0,
  ALLOW_HEX = 1,
  ALLOW_OCTALS = 2,
  ALLOW_TRAILING_JUNK = 4
};


// Convert from Number object to C integer.
static inline int32_t NumberToInt32(Object* number);
static inline uint32_t NumberToUint32(Object* number);


// Converts a string into a double value according to ECMA-262 9.3.1
double StringToDouble(const char* str, int flags, double empty_string_val = 0);
double StringToDouble(String* str, int flags, double empty_string_val = 0);

// Converts a string into an integer.
int StringToInt(String* str, int index, int radix, double* value);
int StringToInt(const char* str, int index, int radix, double* value);

// Converts a double to a string value according to ECMA-262 9.8.1.
// The buffer should be large enough for any floating point number.
// 100 characters is enough.
const char* DoubleToCString(double value, Vector<char> buffer);

// Convert an int to a null-terminated string. The returned string is
// located inside the buffer, but not necessarily at the start.
const char* IntToCString(int n, Vector<char> buffer);

// Additional number to string conversions for the number type.
// The caller is responsible for calling free on the returned pointer.
char* DoubleToFixedCString(double value, int f);
char* DoubleToExponentialCString(double value, int f);
char* DoubleToPrecisionCString(double value, int f);
char* DoubleToRadixCString(double value, int radix);

} }  // namespace v8::internal

#endif  // V8_CONVERSIONS_H_
