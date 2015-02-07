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

// Copyright 2008 the V8 project authors. All rights reserved.
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

#ifndef V8_HASHMAP_H_
#define V8_HASHMAP_H_

namespace v8 {
namespace internal {


// Allocator defines the memory allocator interface
// used by HashMap and implements a default allocator.
class Allocator BASE_EMBEDDED {
 public:
  virtual ~Allocator()  {}
  virtual void* New(size_t size)  { return Malloced::New(size); }
  virtual void Delete(void* p)  { Malloced::Delete(p); }
};


class HashMap {
 public:
  static Allocator DefaultAllocator;

  typedef bool (*MatchFun) (void* key1, void* key2);

  // Dummy constructor.  This constructor doesn't set up the hash
  // map properly so don't use it unless you have good reason.
  HashMap();

  // initial_capacity is the size of the initial hash map;
  // it must be a power of 2 (and thus must not be 0).
  HashMap(MatchFun match,
          Allocator* allocator = &DefaultAllocator,
          uint32_t initial_capacity = 8);

  ~HashMap();

  // HashMap entries are (key, value, hash) triplets.
  // Some clients may not need to use the value slot
  // (e.g. implementers of sets, where the key is the value).
  struct Entry {
    void* key;
    void* value;
    uint32_t hash;  // the full hash value for key
  };

  // If an entry with matching key is found, Lookup()
  // returns that entry. If no matching entry is found,
  // but insert is set, a new entry is inserted with
  // corresponding key, key hash, and NULL value.
  // Otherwise, NULL is returned.
  Entry* Lookup(void* key, uint32_t hash, bool insert);

  // Removes the entry with matching key.
  void Remove(void* key, uint32_t hash);

  // Empties the hash map (occupancy() == 0).
  void Clear();

  // The number of (non-empty) entries in the table.
  uint32_t occupancy() const  { return occupancy_; }

  // The capacity of the table. The implementation
  // makes sure that occupancy is at most 80% of
  // the table capacity.
  uint32_t capacity() const  { return capacity_; }

  // Iteration
  //
  // for (Entry* p = map.Start(); p != NULL; p = map.Next(p)) {
  //   ...
  // }
  //
  // If entries are inserted during iteration, the effect of
  // calling Next() is undefined.
  Entry* Start() const;
  Entry* Next(Entry* p) const;

 private:
  Allocator* allocator_;
  MatchFun match_;
  Entry* map_;
  uint32_t capacity_;
  uint32_t occupancy_;

  Entry* map_end() const  { return map_ + capacity_; }
  Entry* Probe(void* key, uint32_t hash);
  void Initialize(uint32_t capacity);
  void Resize();
};


} }  // namespace v8::internal

#endif  // V8_HASHMAP_H_
