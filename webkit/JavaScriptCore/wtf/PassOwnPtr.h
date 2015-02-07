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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WTF_PassOwnPtr_h
#define WTF_PassOwnPtr_h

#include "Assertions.h"
#include "OwnPtrCommon.h"
#include "TypeTraits.h"

namespace WTF {

    // Unlike most of our smart pointers, PassOwnPtr can take either the pointer type or the pointed-to type.

    template <typename T> class OwnPtr;

    template <typename T> class PassOwnPtr {
    public:
        typedef typename RemovePointer<T>::Type ValueType;
        typedef ValueType* PtrType;

        PassOwnPtr(PtrType ptr = 0) : m_ptr(ptr) { }
        // It somewhat breaks the type system to allow transfer of ownership out of
        // a const PassOwnPtr. However, it makes it much easier to work with PassOwnPtr
        // temporaries, and we don't really have a need to use real const PassOwnPtrs 
        // anyway.
        PassOwnPtr(const PassOwnPtr& o) : m_ptr(o.release()) { }
        template <typename U> PassOwnPtr(const PassOwnPtr<U>& o) : m_ptr(o.release()) { }

        ~PassOwnPtr() { deleteOwnedPtr(m_ptr); }

        PtrType get() const { return m_ptr; }

        void clear() { m_ptr = 0; }
        PtrType release() const { PtrType ptr = m_ptr; m_ptr = 0; return ptr; }

        ValueType& operator*() const { ASSERT(m_ptr); return *m_ptr; }
        PtrType operator->() const { ASSERT(m_ptr); return m_ptr; }

        bool operator!() const { return !m_ptr; }

        // This conversion operator allows implicit conversion to bool but not to other integer types.
        typedef PtrType PassOwnPtr::*UnspecifiedBoolType;
        operator UnspecifiedBoolType() const { return m_ptr ? &PassOwnPtr::m_ptr : 0; }

        PassOwnPtr& operator=(T*);
        PassOwnPtr& operator=(const PassOwnPtr<T>&);
        template <typename U> PassOwnPtr& operator=(const PassOwnPtr<U>&);

    private:
        mutable PtrType m_ptr;
    };

    template <typename T> inline PassOwnPtr<T>& PassOwnPtr<T>::operator=(T* optr)
    {
        T* ptr = m_ptr;
        m_ptr = optr;
        ASSERT(!ptr || m_ptr != ptr);
        if (ptr)
            deleteOwnedPtr(ptr);
        return *this;
    }

    template <typename T> inline PassOwnPtr<T>& PassOwnPtr<T>::operator=(const PassOwnPtr<T>& optr)
    {
        T* ptr = m_ptr;
        m_ptr = optr.release();
        ASSERT(!ptr || m_ptr != ptr);
        if (ptr)
            deleteOwnedPtr(ptr);
        return *this;
    }

    template <typename T> template <typename U> inline PassOwnPtr<T>& PassOwnPtr<T>::operator=(const PassOwnPtr<U>& optr)
    {
        T* ptr = m_ptr;
        m_ptr = optr.release();
        ASSERT(!ptr || m_ptr != ptr);
        if (ptr)
            deleteOwnedPtr(ptr);
        return *this;
    }

    template <typename T, typename U> inline bool operator==(const PassOwnPtr<T>& a, const PassOwnPtr<U>& b) 
    {
        return a.get() == b.get(); 
    }

    template <typename T, typename U> inline bool operator==(const PassOwnPtr<T>& a, const OwnPtr<U>& b) 
    {
        return a.get() == b.get(); 
    }
    
    template <typename T, typename U> inline bool operator==(const OwnPtr<T>& a, const PassOwnPtr<U>& b) 
    {
        return a.get() == b.get(); 
    }
    
    template <typename T, typename U> inline bool operator==(const PassOwnPtr<T>& a, U* b) 
    {
        return a.get() == b; 
    }
    
    template <typename T, typename U> inline bool operator==(T* a, const PassOwnPtr<U>& b) 
    {
        return a == b.get(); 
    }
    
    template <typename T, typename U> inline bool operator!=(const PassOwnPtr<T>& a, const PassOwnPtr<U>& b) 
    {
        return a.get() != b.get(); 
    }
    
    template <typename T, typename U> inline bool operator!=(const PassOwnPtr<T>& a, const OwnPtr<U>& b) 
    {
        return a.get() != b.get(); 
    }
    
    template <typename T, typename U> inline bool operator!=(const OwnPtr<T>& a, const PassOwnPtr<U>& b) 
    {
        return a.get() != b.get(); 
    }
    
    template <typename T, typename U> inline bool operator!=(const PassOwnPtr<T>& a, U* b)
    {
        return a.get() != b; 
    }
    
    template <typename T, typename U> inline bool operator!=(T* a, const PassOwnPtr<U>& b) 
    {
        return a != b.get(); 
    }

    template <typename T, typename U> inline PassOwnPtr<T> static_pointer_cast(const PassOwnPtr<U>& p) 
    {
        return PassOwnPtr<T>(static_cast<T*>(p.release())); 
    }

    template <typename T, typename U> inline PassOwnPtr<T> const_pointer_cast(const PassOwnPtr<U>& p) 
    {
        return PassOwnPtr<T>(const_cast<T*>(p.release())); 
    }

    template <typename T> inline T* getPtr(const PassOwnPtr<T>& p)
    {
        return p.get();
    }

} // namespace WTF

using WTF::PassOwnPtr;
using WTF::const_pointer_cast;
using WTF::static_pointer_cast;

#endif // WTF_PassOwnPtr_h
