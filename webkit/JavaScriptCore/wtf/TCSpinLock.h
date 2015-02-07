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

// Copyright (c) 2005, 2006, Google Inc.
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
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

// ---
// Author: Sanjay Ghemawat <opensource@google.com>

#ifndef TCMALLOC_INTERNAL_SPINLOCK_H__
#define TCMALLOC_INTERNAL_SPINLOCK_H__

#if (PLATFORM(X86) || PLATFORM(PPC)) && (COMPILER(GCC) || COMPILER(MSVC))

#include <time.h>       /* For nanosleep() */

#include <sched.h>      /* For sched_yield() */

#if HAVE(STDINT_H)
#include <stdint.h>
#elif HAVE(INTTYPES_H)
#include <inttypes.h>
#else
#include <sys/types.h>
#endif

#if PLATFORM(WIN_OS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

static void TCMalloc_SlowLock(volatile unsigned int* lockword);

// The following is a struct so that it can be initialized at compile time
struct TCMalloc_SpinLock {

  inline void Lock() {
    int r;
#if COMPILER(GCC)
#if PLATFORM(X86)
    __asm__ __volatile__
      ("xchgl %0, %1"
       : "=r"(r), "=m"(lockword_)
       : "0"(1), "m"(lockword_)
       : "memory");
#else
    volatile unsigned int *lockword_ptr = &lockword_;
    __asm__ __volatile__
        ("1: lwarx %0, 0, %1\n\t"
         "stwcx. %2, 0, %1\n\t"
         "bne- 1b\n\t"
         "isync"
         : "=&r" (r), "=r" (lockword_ptr)
         : "r" (1), "1" (lockword_ptr)
         : "memory");
#endif
#elif COMPILER(MSVC)
    __asm {
        mov eax, this    ; store &lockword_ (which is this+0) in eax
        mov ebx, 1       ; store 1 in ebx
        xchg [eax], ebx  ; exchange lockword_ and 1
        mov r, ebx       ; store old value of lockword_ in r
    }
#endif
    if (r) TCMalloc_SlowLock(&lockword_);
  }

  inline void Unlock() {
#if COMPILER(GCC)
#if PLATFORM(X86)
    __asm__ __volatile__
      ("movl $0, %0"
       : "=m"(lockword_)
       : "m" (lockword_)
       : "memory");
#else
    __asm__ __volatile__
      ("isync\n\t"
       "eieio\n\t"
       "stw %1, %0"
#if PLATFORM(DARWIN) || PLATFORM(PPC)
       : "=o" (lockword_)
#else
       : "=m" (lockword_) 
#endif
       : "r" (0)
       : "memory");
#endif
#elif COMPILER(MSVC)
      __asm {
          mov eax, this  ; store &lockword_ (which is this+0) in eax
          mov [eax], 0   ; set lockword_ to 0
      }
#endif
  }
    // Report if we think the lock can be held by this thread.
    // When the lock is truly held by the invoking thread
    // we will always return true.
    // Indended to be used as CHECK(lock.IsHeld());
    inline bool IsHeld() const {
        return lockword_ != 0;
    }

    inline void Init() { lockword_ = 0; }

    volatile unsigned int lockword_;
};

#define SPINLOCK_INITIALIZER { 0 }

static void TCMalloc_SlowLock(volatile unsigned int* lockword) {
  sched_yield();        // Yield immediately since fast path failed
  while (true) {
    int r;
#if COMPILER(GCC)
#if PLATFORM(X86)
    __asm__ __volatile__
      ("xchgl %0, %1"
       : "=r"(r), "=m"(*lockword)
       : "0"(1), "m"(*lockword)
       : "memory");

#else
    int tmp = 1;
    __asm__ __volatile__
        ("1: lwarx %0, 0, %1\n\t"
         "stwcx. %2, 0, %1\n\t"
         "bne- 1b\n\t"
         "isync"
         : "=&r" (r), "=r" (lockword)
         : "r" (tmp), "1" (lockword)
         : "memory");
#endif
#elif COMPILER(MSVC)
    __asm {
        mov eax, lockword     ; assign lockword into eax
        mov ebx, 1            ; assign 1 into ebx
        xchg [eax], ebx       ; exchange *lockword and 1
        mov r, ebx            ; store old value of *lockword in r
    }
#endif
    if (!r) {
      return;
    }

    // This code was adapted from the ptmalloc2 implementation of
    // spinlocks which would sched_yield() upto 50 times before
    // sleeping once for a few milliseconds.  Mike Burrows suggested
    // just doing one sched_yield() outside the loop and always
    // sleeping after that.  This change helped a great deal on the
    // performance of spinlocks under high contention.  A test program
    // with 10 threads on a dual Xeon (four virtual processors) went
    // from taking 30 seconds to 16 seconds.

    // Sleep for a few milliseconds
#if PLATFORM(WIN_OS)
    Sleep(2);
#else
    struct timespec tm;
    tm.tv_sec = 0;
    tm.tv_nsec = 2000001;
    nanosleep(&tm, NULL);
#endif
  }
}

#else

#include <pthread.h>

// Portable version
struct TCMalloc_SpinLock {
  pthread_mutex_t private_lock_;

  inline void Init() {
    if (pthread_mutex_init(&private_lock_, NULL) != 0) CRASH();
  }
  inline void Finalize() {
    if (pthread_mutex_destroy(&private_lock_) != 0) CRASH();
  }
  inline void Lock() {
    if (pthread_mutex_lock(&private_lock_) != 0) CRASH();
  }
  inline void Unlock() {
    if (pthread_mutex_unlock(&private_lock_) != 0) CRASH();
  }
};

#define SPINLOCK_INITIALIZER { PTHREAD_MUTEX_INITIALIZER }

#endif

// Corresponding locker object that arranges to acquire a spinlock for
// the duration of a C++ scope.
class TCMalloc_SpinLockHolder {
 private:
  TCMalloc_SpinLock* lock_;
 public:
  inline explicit TCMalloc_SpinLockHolder(TCMalloc_SpinLock* l)
    : lock_(l) { l->Lock(); }
  inline ~TCMalloc_SpinLockHolder() { lock_->Unlock(); }
};

// Short-hands for convenient use by tcmalloc.cc
typedef TCMalloc_SpinLock SpinLock;
typedef TCMalloc_SpinLockHolder SpinLockHolder;

#endif  // TCMALLOC_INTERNAL_SPINLOCK_H__
