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
 *  Copyright (C) 2004, 2008, 2009 Apple Inc. All rights reserved.
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


#ifndef Protect_h
#define Protect_h

#include "JSCell.h"
#include "Collector.h"

namespace JSC {

    inline void gcProtect(JSCell* val) 
    {
        Heap::heap(val)->protect(val);
    }

    inline void gcUnprotect(JSCell* val)
    {
        Heap::heap(val)->unprotect(val);
    }

    inline void gcProtectNullTolerant(JSCell* val) 
    {
        if (val) 
            gcProtect(val);
    }

    inline void gcUnprotectNullTolerant(JSCell* val) 
    {
        if (val) 
            gcUnprotect(val);
    }
    
    inline void gcProtect(JSValue value)
    {
        if (value && value.isCell())
            gcProtect(asCell(value));
    }

    inline void gcUnprotect(JSValue value)
    {
        if (value && value.isCell())
            gcUnprotect(asCell(value));
    }

    // FIXME: Share more code with RefPtr template? The only differences are the ref/deref operation
    // and the implicit conversion to raw pointer
    template <class T> class ProtectedPtr {
    public:
        ProtectedPtr() : m_ptr(0) {}
        ProtectedPtr(T* ptr);
        ProtectedPtr(const ProtectedPtr&);
        ~ProtectedPtr();

        template <class U> ProtectedPtr(const ProtectedPtr<U>&);
        
        T* get() const { return m_ptr; }
        operator T*() const { return m_ptr; }
        operator JSValue() const { return JSValue(m_ptr); }
        T* operator->() const { return m_ptr; }
        
        operator bool() const { return m_ptr; }
        bool operator!() const { return !m_ptr; }

        ProtectedPtr& operator=(const ProtectedPtr&);
        ProtectedPtr& operator=(T*);
        
    private:
        T* m_ptr;
    };

    class ProtectedJSValue {
    public:
        ProtectedJSValue() {}
        ProtectedJSValue(JSValue value);
        ProtectedJSValue(const ProtectedJSValue&);
        ~ProtectedJSValue();

        template <class U> ProtectedJSValue(const ProtectedPtr<U>&);
        
        JSValue get() const { return m_value; }
        operator JSValue() const { return m_value; }
        JSValue operator->() const { return m_value; }
        
        operator bool() const { return m_value; }
        bool operator!() const { return !m_value; }

        ProtectedJSValue& operator=(const ProtectedJSValue&);
        ProtectedJSValue& operator=(JSValue);
        
    private:
        JSValue m_value;
    };

    template <class T> inline ProtectedPtr<T>::ProtectedPtr(T* ptr)
        : m_ptr(ptr)
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>::ProtectedPtr(const ProtectedPtr& o)
        : m_ptr(o.get())
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>::~ProtectedPtr()
    {
        gcUnprotectNullTolerant(m_ptr);
    }

    template <class T> template <class U> inline ProtectedPtr<T>::ProtectedPtr(const ProtectedPtr<U>& o)
        : m_ptr(o.get())
    {
        gcProtectNullTolerant(m_ptr);
    }

    template <class T> inline ProtectedPtr<T>& ProtectedPtr<T>::operator=(const ProtectedPtr<T>& o) 
    {
        T* optr = o.m_ptr;
        gcProtectNullTolerant(optr);
        gcUnprotectNullTolerant(m_ptr);
        m_ptr = optr;
        return *this;
    }

    template <class T> inline ProtectedPtr<T>& ProtectedPtr<T>::operator=(T* optr)
    {
        gcProtectNullTolerant(optr);
        gcUnprotectNullTolerant(m_ptr);
        m_ptr = optr;
        return *this;
    }

    inline ProtectedJSValue::ProtectedJSValue(JSValue value)
        : m_value(value)
    {
        gcProtect(m_value);
    }

    inline ProtectedJSValue::ProtectedJSValue(const ProtectedJSValue& o)
        : m_value(o.get())
    {
        gcProtect(m_value);
    }

    inline ProtectedJSValue::~ProtectedJSValue()
    {
        gcUnprotect(m_value);
    }

    template <class U> ProtectedJSValue::ProtectedJSValue(const ProtectedPtr<U>& o)
        : m_value(o.get())
    {
        gcProtect(m_value);
    }

    inline ProtectedJSValue& ProtectedJSValue::operator=(const ProtectedJSValue& o) 
    {
        JSValue ovalue = o.m_value;
        gcProtect(ovalue);
        gcUnprotect(m_value);
        m_value = ovalue;
        return *this;
    }

    inline ProtectedJSValue& ProtectedJSValue::operator=(JSValue ovalue)
    {
        gcProtect(ovalue);
        gcUnprotect(m_value);
        m_value = ovalue;
        return *this;
    }

    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const ProtectedPtr<T>& b) { return a.get() == b.get(); }
    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const T* b) { return a.get() == b; }
    template <class T> inline bool operator==(const T* a, const ProtectedPtr<T>& b) { return a == b.get(); }

    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const ProtectedPtr<T>& b) { return a.get() != b.get(); }
    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const T* b) { return a.get() != b; }
    template <class T> inline bool operator!=(const T* a, const ProtectedPtr<T>& b) { return a != b.get(); }

    inline bool operator==(const ProtectedJSValue& a, const ProtectedJSValue& b) { return a.get() == b.get(); }
    inline bool operator==(const ProtectedJSValue& a, const JSValue b) { return a.get() == b; }
    template <class T> inline bool operator==(const ProtectedJSValue& a, const ProtectedPtr<T>& b) { return a.get() == JSValue(b.get()); }
    inline bool operator==(const JSValue a, const ProtectedJSValue& b) { return a == b.get(); }
    template <class T> inline bool operator==(const ProtectedPtr<T>& a, const ProtectedJSValue& b) { return JSValue(a.get()) == b.get(); }

    inline bool operator!=(const ProtectedJSValue& a, const ProtectedJSValue& b) { return a.get() != b.get(); }
    inline bool operator!=(const ProtectedJSValue& a, const JSValue b) { return a.get() != b; }
    template <class T> inline bool operator!=(const ProtectedJSValue& a, const ProtectedPtr<T>& b) { return a.get() != JSValue(b.get()); }
    inline bool operator!=(const JSValue a, const ProtectedJSValue& b) { return a != b.get(); }
    template <class T> inline bool operator!=(const ProtectedPtr<T>& a, const ProtectedJSValue& b) { return JSValue(a.get()) != b.get(); }
 
} // namespace JSC

#endif // Protect_h
