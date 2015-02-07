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
 *  Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_OwnPtr_h
#define WTF_OwnPtr_h

#include "Assertions.h"
#include "Noncopyable.h"
#include "OwnPtrCommon.h"
#include "TypeTraits.h"
#include <algorithm>
#include <memory>

namespace WTF {

    // Unlike most of our smart pointers, OwnPtr can take either the pointer type or the pointed-to type.

    template <typename T> class PassOwnPtr;

    template <typename T> class OwnPtr : public Noncopyable {
    public:
        typedef typename RemovePointer<T>::Type ValueType;
        typedef ValueType* PtrType;

        explicit OwnPtr(PtrType ptr = 0) : m_ptr(ptr) { }
        OwnPtr(std::auto_ptr<ValueType> autoPtr) : m_ptr(autoPtr.release()) { }
        // See comment in PassOwnPtr.h for why this takes a const reference.
        template <typename U> OwnPtr(const PassOwnPtr<U>& o);

        // This copy constructor is used implicitly by gcc when it generates
        // transients for assigning a PassOwnPtr<T> object to a stack-allocated
        // OwnPtr<T> object.  It should never be called explicitly and gcc
        // should optimize away the constructor when generating code.
        OwnPtr(const OwnPtr<ValueType>& o);

        ~OwnPtr() { deleteOwnedPtr(m_ptr); }

        PtrType get() const { return m_ptr; }
        PtrType release() { PtrType ptr = m_ptr; m_ptr = 0; return ptr; }

        // FIXME: This should be renamed to adopt. 
        void set(PtrType ptr) { ASSERT(!ptr || m_ptr != ptr); deleteOwnedPtr(m_ptr); m_ptr = ptr; }

        void adopt(std::auto_ptr<ValueType> autoPtr) { ASSERT(!autoPtr.get() || m_ptr != autoPtr.get()); deleteOwnedPtr(m_ptr); m_ptr = autoPtr.release(); }

        void clear() { deleteOwnedPtr(m_ptr); m_ptr = 0; }

        ValueType& operator*() const { ASSERT(m_ptr); return *m_ptr; }
        PtrType operator->() const { ASSERT(m_ptr); return m_ptr; }

        bool operator!() const { return !m_ptr; }

        // This conversion operator allows implicit conversion to bool but not to other integer types.
        typedef PtrType OwnPtr::*UnspecifiedBoolType;
        operator UnspecifiedBoolType() const { return m_ptr ? &OwnPtr::m_ptr : 0; }

        OwnPtr& operator=(const PassOwnPtr<T>&);
        template <typename U> OwnPtr& operator=(const PassOwnPtr<U>&);

        void swap(OwnPtr& o) { std::swap(m_ptr, o.m_ptr); }

    private:
        PtrType m_ptr;
    };

    template <typename T> template <typename U> inline OwnPtr<T>::OwnPtr(const PassOwnPtr<U>& o)
        : m_ptr(o.release())
    {
    }

    template <typename T> inline OwnPtr<T>& OwnPtr<T>::operator=(const PassOwnPtr<T>& o)
    {
        T* ptr = m_ptr;
        m_ptr = o.release();
        ASSERT(!ptr || m_ptr != ptr);
        if (ptr)
            deleteOwnedPtr(ptr);
        return *this;
    }

    template <typename T> template <typename U> inline OwnPtr<T>& OwnPtr<T>::operator=(const PassOwnPtr<U>& o)
    {
        T* ptr = m_ptr;
        m_ptr = o.release();
        ASSERT(!ptr || m_ptr != ptr);
        if (ptr)
            deleteOwnedPtr(ptr);
        return *this;
    }

    template <typename T> inline void swap(OwnPtr<T>& a, OwnPtr<T>& b)
    {
        a.swap(b);
    }

    template <typename T, typename U> inline bool operator==(const OwnPtr<T>& a, U* b)
    {
        return a.get() == b; 
    }

    template <typename T, typename U> inline bool operator==(T* a, const OwnPtr<U>& b) 
    {
        return a == b.get(); 
    }

    template <typename T, typename U> inline bool operator!=(const OwnPtr<T>& a, U* b)
    {
        return a.get() != b; 
    }

    template <typename T, typename U> inline bool operator!=(T* a, const OwnPtr<U>& b)
    {
        return a != b.get(); 
    }

    template <typename T> inline typename OwnPtr<T>::PtrType getPtr(const OwnPtr<T>& p)
    {
        return p.get();
    }

} // namespace WTF

using WTF::OwnPtr;

#endif // WTF_OwnPtr_h
