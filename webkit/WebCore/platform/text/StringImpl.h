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

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef StringImpl_h
#define StringImpl_h

#include <limits.h>
#include <wtf/ASCIICType.h>
#include <wtf/CrossThreadRefCounted.h>
#include <wtf/OwnFastMallocPtr.h>
#include <wtf/PassRefPtr.h>
#include <wtf/PtrAndFlags.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>
#include <wtf/unicode/Unicode.h>

#if USE(JSC)
#include <runtime/UString.h>
#endif

#if PLATFORM(CF) || (PLATFORM(QT) && PLATFORM(DARWIN))
typedef const struct __CFString * CFStringRef;
#endif

#ifdef __OBJC__
@class NSString;
#endif

namespace WebCore {

class AtomicString;
class StringBuffer;

struct CStringTranslator;
struct HashAndCharactersTranslator;
struct StringHash;
struct UCharBufferTranslator;

enum TextCaseSensitivity { TextCaseSensitive, TextCaseInsensitive };

typedef bool (*CharacterMatchFunctionPtr)(UChar);

class StringImpl : public RefCounted<StringImpl> {
    friend class AtomicString;
    friend struct CStringTranslator;
    friend struct HashAndCharactersTranslator;
    friend struct UCharBufferTranslator;
private:
    friend class ThreadGlobalData;
    StringImpl();
    StringImpl(const UChar*, unsigned length);
    StringImpl(const char*, unsigned length);

    struct AdoptBuffer { };
    StringImpl(UChar*, unsigned length, AdoptBuffer);

    struct WithTerminatingNullCharacter { };
    StringImpl(const StringImpl&, WithTerminatingNullCharacter);

    // For AtomicString.
    StringImpl(const UChar*, unsigned length, unsigned hash);
    StringImpl(const char*, unsigned length, unsigned hash);

    typedef CrossThreadRefCounted<OwnFastMallocPtr<UChar> > SharedUChar;

public:
    ~StringImpl();

    static PassRefPtr<StringImpl> create(const UChar*, unsigned length);
    static PassRefPtr<StringImpl> create(const char*, unsigned length);
    static PassRefPtr<StringImpl> create(const char*);
    static PassRefPtr<StringImpl> createUninitialized(unsigned length, UChar*& data);

    static PassRefPtr<StringImpl> createWithTerminatingNullCharacter(const StringImpl&);

    static PassRefPtr<StringImpl> createStrippingNullCharacters(const UChar*, unsigned length);
    static PassRefPtr<StringImpl> adopt(StringBuffer&);
    static PassRefPtr<StringImpl> adopt(Vector<UChar>&);
#if USE(JSC)
    static PassRefPtr<StringImpl> create(const JSC::UString&);
    JSC::UString ustring();
#endif

    SharedUChar* sharedBuffer();
    const UChar* characters() { return m_data; }
    unsigned length() { return m_length; }

    bool hasTerminatingNullCharacter() const { return m_sharedBufferAndFlags.isFlagSet(HasTerminatingNullCharacter); }

    bool inTable() const { return m_sharedBufferAndFlags.isFlagSet(InTable); }
    void setInTable() { return m_sharedBufferAndFlags.setFlag(InTable); }

    unsigned hash() { if (m_hash == 0) m_hash = computeHash(m_data, m_length); return m_hash; }
    unsigned existingHash() const { ASSERT(m_hash); return m_hash; }
    static unsigned computeHash(const UChar*, unsigned len);
    static unsigned computeHash(const char*);
    
    // Makes a deep copy. Helpful only if you need to use a String on another thread.
    // Since StringImpl objects are immutable, there's no other reason to make a copy.
    PassRefPtr<StringImpl> copy();

    // Makes a deep copy like copy() but only for a substring.
    // (This ensures that you always get something suitable for a thread while subtring
    // may not.  For example, in the empty string case, substring returns empty() which
    // is not safe for another thread.)
    PassRefPtr<StringImpl> substringCopy(unsigned pos, unsigned len  = UINT_MAX);

    PassRefPtr<StringImpl> substring(unsigned pos, unsigned len = UINT_MAX);

    UChar operator[](unsigned i) { ASSERT(i < m_length); return m_data[i]; }
    UChar32 characterStartingAt(unsigned);

    bool containsOnlyWhitespace();

    int toIntStrict(bool* ok = 0, int base = 10);
    unsigned toUIntStrict(bool* ok = 0, int base = 10);
    int64_t toInt64Strict(bool* ok = 0, int base = 10);
    uint64_t toUInt64Strict(bool* ok = 0, int base = 10);
    intptr_t toIntPtrStrict(bool* ok = 0, int base = 10);

    int toInt(bool* ok = 0); // ignores trailing garbage
    unsigned toUInt(bool* ok = 0); // ignores trailing garbage
    int64_t toInt64(bool* ok = 0); // ignores trailing garbage
    uint64_t toUInt64(bool* ok = 0); // ignores trailing garbage
    intptr_t toIntPtr(bool* ok = 0); // ignores trailing garbage

    double toDouble(bool* ok = 0);
    float toFloat(bool* ok = 0);

    bool isLower();
    PassRefPtr<StringImpl> lower();
    PassRefPtr<StringImpl> upper();
    PassRefPtr<StringImpl> secure(UChar aChar);
    PassRefPtr<StringImpl> capitalize(UChar previousCharacter);
    PassRefPtr<StringImpl> foldCase();

    PassRefPtr<StringImpl> stripWhiteSpace();
    PassRefPtr<StringImpl> simplifyWhiteSpace();

    PassRefPtr<StringImpl> removeCharacters(CharacterMatchFunctionPtr);

    int find(const char*, int index = 0, bool caseSensitive = true);
    int find(UChar, int index = 0);
    int find(CharacterMatchFunctionPtr, int index = 0);
    int find(StringImpl*, int index, bool caseSensitive = true);

    int reverseFind(UChar, int index);
    int reverseFind(StringImpl*, int index, bool caseSensitive = true);
    
    bool startsWith(StringImpl* m_data, bool caseSensitive = true) { return reverseFind(m_data, 0, caseSensitive) == 0; }
    bool endsWith(StringImpl*, bool caseSensitive = true);

    PassRefPtr<StringImpl> replace(UChar, UChar);
    PassRefPtr<StringImpl> replace(UChar, StringImpl*);
    PassRefPtr<StringImpl> replace(StringImpl*, StringImpl*);
    PassRefPtr<StringImpl> replace(unsigned index, unsigned len, StringImpl*);

    static StringImpl* empty();

    Vector<char> ascii();

    WTF::Unicode::Direction defaultWritingDirection();

#if PLATFORM(CF) || (PLATFORM(QT) && PLATFORM(DARWIN))
    CFStringRef createCFString();
#endif
#ifdef __OBJC__
    operator NSString*();
#endif

    void operator delete(void*);

private:
    // Allocation from a custom buffer is only allowed internally to avoid
    // mismatched allocators. Callers should use create().
    void* operator new(size_t size);
    void* operator new(size_t size, void* address);

    static PassRefPtr<StringImpl> createStrippingNullCharactersSlowCase(const UChar*, unsigned length);

    enum StringImplFlags {
        HasTerminatingNullCharacter,
        InTable,
    };

    unsigned m_length;
    const UChar* m_data;
    mutable unsigned m_hash;
    PtrAndFlags<SharedUChar, StringImplFlags> m_sharedBufferAndFlags;

    // In some cases, we allocate the StringImpl struct and its data
    // within a single heap buffer. In this case, the m_data pointer
    // is an "internal buffer", and does not need to be deallocated.
    bool m_bufferIsInternal;
};

bool equal(StringImpl*, StringImpl*);
bool equal(StringImpl*, const char*);
inline bool equal(const char* a, StringImpl* b) { return equal(b, a); }

bool equalIgnoringCase(StringImpl*, StringImpl*);
bool equalIgnoringCase(StringImpl*, const char*);
inline bool equalIgnoringCase(const char* a, StringImpl* b) { return equalIgnoringCase(b, a); }
bool equalIgnoringCase(const UChar* a, const char* b, unsigned length);
inline bool equalIgnoringCase(const char* a, const UChar* b, unsigned length) { return equalIgnoringCase(b, a, length); }

bool equalIgnoringNullity(StringImpl*, StringImpl*);

// Golden ratio - arbitrary start value to avoid mapping all 0's to all 0's
// or anything like that.
const unsigned phi = 0x9e3779b9U;

// Paul Hsieh's SuperFastHash
// http://www.azillionmonkeys.com/qed/hash.html
inline unsigned StringImpl::computeHash(const UChar* data, unsigned length)
{
    unsigned hash = phi;
    
    // Main loop.
    for (unsigned pairCount = length >> 1; pairCount; pairCount--) {
        hash += data[0];
        unsigned tmp = (data[1] << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2;
        hash += hash >> 11;
    }
    
    // Handle end case.
    if (length & 1) {
        hash += data[0];
        hash ^= hash << 11;
        hash += hash >> 17;
    }

    // Force "avalanching" of final 127 bits.
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 2;
    hash += hash >> 15;
    hash ^= hash << 10;

    // This avoids ever returning a hash code of 0, since that is used to
    // signal "hash not computed yet", using a value that is likely to be
    // effectively the same as 0 when the low bits are masked.
    hash |= !hash << 31;
    
    return hash;
}

// Paul Hsieh's SuperFastHash
// http://www.azillionmonkeys.com/qed/hash.html
inline unsigned StringImpl::computeHash(const char* data)
{
    // This hash is designed to work on 16-bit chunks at a time. But since the normal case
    // (above) is to hash UTF-16 characters, we just treat the 8-bit chars as if they
    // were 16-bit chunks, which should give matching results

    unsigned hash = phi;
    
    // Main loop
    for (;;) {
        unsigned char b0 = data[0];
        if (!b0)
            break;
        unsigned char b1 = data[1];
        if (!b1) {
            hash += b0;
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        }
        hash += b0;
        unsigned tmp = (b1 << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2;
        hash += hash >> 11;
    }
    
    // Force "avalanching" of final 127 bits.
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 2;
    hash += hash >> 15;
    hash ^= hash << 10;

    // This avoids ever returning a hash code of 0, since that is used to
    // signal "hash not computed yet", using a value that is likely to be
    // effectively the same as 0 when the low bits are masked.
    hash |= !hash << 31;
    
    return hash;
}

static inline bool isSpaceOrNewline(UChar c)
{
    // Use isASCIISpace() for basic Latin-1.
    // This will include newlines, which aren't included in Unicode DirWS.
    return c <= 0x7F ? WTF::isASCIISpace(c) : WTF::Unicode::direction(c) == WTF::Unicode::WhiteSpaceNeutral;
}

// This is a hot function because it's used when parsing HTML.
inline PassRefPtr<StringImpl> StringImpl::createStrippingNullCharacters(const UChar* characters, unsigned length)
{
    ASSERT(characters);
    ASSERT(length);

    // Optimize for the case where there are no Null characters by quickly
    // searching for nulls, and then using StringImpl::create, which will
    // memcpy the whole buffer.  This is faster than assigning character by
    // character during the loop. 

    // Fast case.
    int foundNull = 0;
    for (unsigned i = 0; !foundNull && i < length; i++) {
        int c = characters[i]; // more efficient than using UChar here (at least on Intel Mac OS)
        foundNull |= !c;
    }
    if (!foundNull)
        return StringImpl::create(characters, length);

    return StringImpl::createStrippingNullCharactersSlowCase(characters, length);
}

}

namespace WTF {

    // WebCore::StringHash is the default hash for StringImpl* and RefPtr<StringImpl>
    template<typename T> struct DefaultHash;
    template<> struct DefaultHash<WebCore::StringImpl*> {
        typedef WebCore::StringHash Hash;
    };
    template<> struct DefaultHash<RefPtr<WebCore::StringImpl> > {
        typedef WebCore::StringHash Hash;
    };

}

#endif
