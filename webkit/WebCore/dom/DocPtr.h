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
 * This file is part of the DOM implementation for KDE.
 * Copyright (C) 2005, 2006 Apple Computer, Inc.
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

#ifndef DocPtr_h
#define DocPtr_h

namespace WebCore {

template <class T> class DocPtr {
public:
    DocPtr() : m_ptr(0) {}
    DocPtr(T *ptr) : m_ptr(ptr) { if (ptr) ptr->selfOnlyRef(); }
    DocPtr(const DocPtr &o) : m_ptr(o.m_ptr) { if (T *ptr = m_ptr) ptr->selfOnlyRef(); }
    ~DocPtr() { if (T *ptr = m_ptr) ptr->selfOnlyDeref(); }
    
    template <class U> DocPtr(const DocPtr<U> &o) : m_ptr(o.get()) { if (T *ptr = m_ptr) ptr->selfOnlyRef(); }
    
    void resetSkippingRef(T *o) { m_ptr = o; }
    
    T *get() const { return m_ptr; }
    
    T &operator*() const { return *m_ptr; }
    T *operator->() const { return m_ptr; }
    
    bool operator!() const { return !m_ptr; }

    // this type conversion operator allows implicit conversion to
    // bool but not to other integer types
    
    typedef T * (DocPtr::*UnspecifiedBoolType)() const;
    operator UnspecifiedBoolType() const
    {
        return m_ptr ? &DocPtr::get : 0;
    }
    
    DocPtr &operator=(const DocPtr &);
    DocPtr &operator=(T *);
    
 private:
    T *m_ptr;
};

template <class T> DocPtr<T> &DocPtr<T>::operator=(const DocPtr<T> &o) 
{
    T *optr = o.m_ptr;
    if (optr)
        optr->selfOnlyRef();
    if (T *ptr = m_ptr)
        ptr->selfOnlyDeref();
    m_ptr = optr;
        return *this;
}

template <class T> inline DocPtr<T> &DocPtr<T>::operator=(T *optr)
{
    if (optr)
        optr->selfOnlyRef();
    if (T *ptr = m_ptr)
        ptr->selfOnlyDeref();
    m_ptr = optr;
    return *this;
}

template <class T> inline bool operator==(const DocPtr<T> &a, const DocPtr<T> &b) 
{ 
    return a.get() == b.get(); 
}

template <class T> inline bool operator==(const DocPtr<T> &a, const T *b) 
{ 
    return a.get() == b; 
}

template <class T> inline bool operator==(const T *a, const DocPtr<T> &b) 
{
    return a == b.get(); 
}

template <class T> inline bool operator!=(const DocPtr<T> &a, const DocPtr<T> &b) 
{ 
    return a.get() != b.get(); 
}

template <class T> inline bool operator!=(const DocPtr<T> &a, const T *b)
{
    return a.get() != b; 
}

template <class T> inline bool operator!=(const T *a, const DocPtr<T> &b) 
{ 
    return a != b.get(); 
}

} // namespace WebCore

#endif // DocPtr_h
