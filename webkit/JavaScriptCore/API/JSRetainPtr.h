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
 * Copyright (C) 2005, 2006, 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JSRetainPtr_h
#define JSRetainPtr_h

#include <JavaScriptCore/JSStringRef.h>
#include <algorithm>

inline void JSRetain(JSStringRef string) { JSStringRetain(string); }
inline void JSRelease(JSStringRef string) { JSStringRelease(string); }

enum AdoptTag { Adopt };

template <typename T> class JSRetainPtr {
public:
    JSRetainPtr() : m_ptr(0) {}
    JSRetainPtr(T ptr) : m_ptr(ptr) { if (ptr) JSRetain(ptr); }

    JSRetainPtr(AdoptTag, T ptr) : m_ptr(ptr) { }
    
    JSRetainPtr(const JSRetainPtr& o) : m_ptr(o.m_ptr) { if (T ptr = m_ptr) JSRetain(ptr); }

    ~JSRetainPtr() { if (T ptr = m_ptr) JSRelease(ptr); }
    
    template <typename U> JSRetainPtr(const JSRetainPtr<U>& o) : m_ptr(o.get()) { if (T ptr = m_ptr) JSRetain(ptr); }
    
    T get() const { return m_ptr; }
    
    T releaseRef() { T tmp = m_ptr; m_ptr = 0; return tmp; }
    
    T operator->() const { return m_ptr; }
    
    bool operator!() const { return !m_ptr; }

    // This conversion operator allows implicit conversion to bool but not to other integer types.
    typedef T JSRetainPtr::*UnspecifiedBoolType;
    operator UnspecifiedBoolType() const { return m_ptr ? &JSRetainPtr::m_ptr : 0; }
    
    JSRetainPtr& operator=(const JSRetainPtr&);
    template <typename U> JSRetainPtr& operator=(const JSRetainPtr<U>&);
    JSRetainPtr& operator=(T);
    template <typename U> JSRetainPtr& operator=(U*);

    void adopt(T);
    
    void swap(JSRetainPtr&);

private:
    T m_ptr;
};

template <typename T> inline JSRetainPtr<T>& JSRetainPtr<T>::operator=(const JSRetainPtr<T>& o)
{
    T optr = o.get();
    if (optr)
        JSRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        JSRelease(ptr);
    return *this;
}

template <typename T> template <typename U> inline JSRetainPtr<T>& JSRetainPtr<T>::operator=(const JSRetainPtr<U>& o)
{
    T optr = o.get();
    if (optr)
        JSRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        JSRelease(ptr);
    return *this;
}

template <typename T> inline JSRetainPtr<T>& JSRetainPtr<T>::operator=(T optr)
{
    if (optr)
        JSRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        JSRelease(ptr);
    return *this;
}

template <typename T> inline void JSRetainPtr<T>::adopt(T optr)
{
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        JSRelease(ptr);
}

template <typename T> template <typename U> inline JSRetainPtr<T>& JSRetainPtr<T>::operator=(U* optr)
{
    if (optr)
        JSRetain(optr);
    T ptr = m_ptr;
    m_ptr = optr;
    if (ptr)
        JSRelease(ptr);
    return *this;
}

template <class T> inline void JSRetainPtr<T>::swap(JSRetainPtr<T>& o)
{
    std::swap(m_ptr, o.m_ptr);
}

template <class T> inline void swap(JSRetainPtr<T>& a, JSRetainPtr<T>& b)
{
    a.swap(b);
}

template <typename T, typename U> inline bool operator==(const JSRetainPtr<T>& a, const JSRetainPtr<U>& b)
{ 
    return a.get() == b.get(); 
}

template <typename T, typename U> inline bool operator==(const JSRetainPtr<T>& a, U* b)
{ 
    return a.get() == b; 
}

template <typename T, typename U> inline bool operator==(T* a, const JSRetainPtr<U>& b) 
{
    return a == b.get(); 
}

template <typename T, typename U> inline bool operator!=(const JSRetainPtr<T>& a, const JSRetainPtr<U>& b)
{ 
    return a.get() != b.get(); 
}

template <typename T, typename U> inline bool operator!=(const JSRetainPtr<T>& a, U* b)
{
    return a.get() != b; 
}

template <typename T, typename U> inline bool operator!=(T* a, const JSRetainPtr<U>& b)
{ 
    return a != b.get(); 
}


#endif // JSRetainPtr_h
