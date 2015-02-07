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
 *  Copyright (C) 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef WTF_RefPtr_h
#define WTF_RefPtr_h

#include <algorithm>
#include "AlwaysInline.h"
#include "FastAllocBase.h"

namespace WTF {

    enum PlacementNewAdoptType { PlacementNewAdopt };

    template <typename T> class PassRefPtr;

    enum HashTableDeletedValueType { HashTableDeletedValue };

    template <typename T> class RefPtr : public FastAllocBase {
    public:
        RefPtr() : m_ptr(0) { }
        RefPtr(T* ptr) : m_ptr(ptr) { if (ptr) ptr->ref(); }
        RefPtr(const RefPtr& o) : m_ptr(o.m_ptr) { if (T* ptr = m_ptr) ptr->ref(); }
        // see comment in PassRefPtr.h for why this takes const reference
        template <typename U> RefPtr(const PassRefPtr<U>&);

        // Special constructor for cases where we overwrite an object in place.
        RefPtr(PlacementNewAdoptType) { }

        // Hash table deleted values, which are only constructed and never copied or destroyed.
        RefPtr(HashTableDeletedValueType) : m_ptr(hashTableDeletedValue()) { }
        bool isHashTableDeletedValue() const { return m_ptr == hashTableDeletedValue(); }

        ~RefPtr() { if (T* ptr = m_ptr) ptr->deref(); }
        
        template <typename U> RefPtr(const RefPtr<U>& o) : m_ptr(o.get()) { if (T* ptr = m_ptr) ptr->ref(); }
        
        T* get() const { return m_ptr; }
        
        void clear() { if (T* ptr = m_ptr) ptr->deref(); m_ptr = 0; }
        PassRefPtr<T> release() { PassRefPtr<T> tmp = adoptRef(m_ptr); m_ptr = 0; return tmp; }

        T& operator*() const { return *m_ptr; }
        ALWAYS_INLINE T* operator->() const { return m_ptr; }
        
        bool operator!() const { return !m_ptr; }
    
        // This conversion operator allows implicit conversion to bool but not to other integer types.
#if COMPILER(WINSCW)
        operator bool() const { return m_ptr; }
#else
        typedef T* RefPtr::*UnspecifiedBoolType;
        operator UnspecifiedBoolType() const { return m_ptr ? &RefPtr::m_ptr : 0; }
#endif
        
        RefPtr& operator=(const RefPtr&);
        RefPtr& operator=(T*);
        RefPtr& operator=(const PassRefPtr<T>&);
        template <typename U> RefPtr& operator=(const RefPtr<U>&);
        template <typename U> RefPtr& operator=(const PassRefPtr<U>&);

        void swap(RefPtr&);

    private:
        static T* hashTableDeletedValue() { return reinterpret_cast<T*>(-1); }

        T* m_ptr;
    };
    
    template <typename T> template <typename U> inline RefPtr<T>::RefPtr(const PassRefPtr<U>& o)
        : m_ptr(o.releaseRef())
    {
    }

    template <typename T> inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr<T>& o)
    {
        T* optr = o.get();
        if (optr)
            optr->ref();
        T* ptr = m_ptr;
        m_ptr = optr;
        if (ptr)
            ptr->deref();
        return *this;
    }
    
    template <typename T> template <typename U> inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr<U>& o)
    {
        T* optr = o.get();
        if (optr)
            optr->ref();
        T* ptr = m_ptr;
        m_ptr = optr;
        if (ptr)
            ptr->deref();
        return *this;
    }
    
    template <typename T> inline RefPtr<T>& RefPtr<T>::operator=(T* optr)
    {
        if (optr)
            optr->ref();
        T* ptr = m_ptr;
        m_ptr = optr;
        if (ptr)
            ptr->deref();
        return *this;
    }

    template <typename T> inline RefPtr<T>& RefPtr<T>::operator=(const PassRefPtr<T>& o)
    {
        T* ptr = m_ptr;
        m_ptr = o.releaseRef();
        if (ptr)
            ptr->deref();
        return *this;
    }

    template <typename T> template <typename U> inline RefPtr<T>& RefPtr<T>::operator=(const PassRefPtr<U>& o)
    {
        T* ptr = m_ptr;
        m_ptr = o.releaseRef();
        if (ptr)
            ptr->deref();
        return *this;
    }

    template <class T> inline void RefPtr<T>::swap(RefPtr<T>& o)
    {
        std::swap(m_ptr, o.m_ptr);
    }

    template <class T> inline void swap(RefPtr<T>& a, RefPtr<T>& b)
    {
        a.swap(b);
    }

    template <typename T, typename U> inline bool operator==(const RefPtr<T>& a, const RefPtr<U>& b)
    { 
        return a.get() == b.get(); 
    }

    template <typename T, typename U> inline bool operator==(const RefPtr<T>& a, U* b)
    { 
        return a.get() == b; 
    }
    
    template <typename T, typename U> inline bool operator==(T* a, const RefPtr<U>& b) 
    {
        return a == b.get(); 
    }
    
    template <typename T, typename U> inline bool operator!=(const RefPtr<T>& a, const RefPtr<U>& b)
    { 
        return a.get() != b.get(); 
    }

    template <typename T, typename U> inline bool operator!=(const RefPtr<T>& a, U* b)
    {
        return a.get() != b; 
    }

    template <typename T, typename U> inline bool operator!=(T* a, const RefPtr<U>& b)
    { 
        return a != b.get(); 
    }
    
    template <typename T, typename U> inline RefPtr<T> static_pointer_cast(const RefPtr<U>& p)
    { 
        return RefPtr<T>(static_cast<T*>(p.get())); 
    }

    template <typename T, typename U> inline RefPtr<T> const_pointer_cast(const RefPtr<U>& p)
    { 
        return RefPtr<T>(const_cast<T*>(p.get())); 
    }

    template <typename T> inline T* getPtr(const RefPtr<T>& p)
    {
        return p.get();
    }

} // namespace WTF

using WTF::RefPtr;
using WTF::static_pointer_cast;
using WTF::const_pointer_cast;

#endif // WTF_RefPtr_h
