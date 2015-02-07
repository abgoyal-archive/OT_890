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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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

#ifndef CachedResourceHandle_h
#define CachedResourceHandle_h

#include "CachedResource.h"

namespace WebCore {

    class CachedResourceHandleBase {
    public:
        ~CachedResourceHandleBase() { if (m_resource) m_resource->unregisterHandle(this); }
        CachedResource* get() const { return m_resource; }
        
        bool operator!() const { return !m_resource; }
        
        // This conversion operator allows implicit conversion to bool but not to other integer types.
        typedef CachedResource* CachedResourceHandleBase::*UnspecifiedBoolType;
        operator UnspecifiedBoolType() const { return m_resource ? &CachedResourceHandleBase::m_resource : 0; }

    protected:
        CachedResourceHandleBase() : m_resource(0) {}
        CachedResourceHandleBase(CachedResource* res) { m_resource = res; if (m_resource) m_resource->registerHandle(this); }
        CachedResourceHandleBase(const CachedResourceHandleBase& o) : m_resource(o.m_resource) { if (m_resource) m_resource->registerHandle(this); }

        void setResource(CachedResource*);
        
    private:
        CachedResourceHandleBase& operator=(const CachedResourceHandleBase&) { return *this; } 
        
        friend class CachedResource;

        CachedResource* m_resource;
    };
        
    template <class R> class CachedResourceHandle : public CachedResourceHandleBase {
    public: 
        CachedResourceHandle() { }
        CachedResourceHandle(R* res) : CachedResourceHandleBase(res) { }
        CachedResourceHandle(const CachedResourceHandle<R>& o) : CachedResourceHandleBase(o) { }

        R* get() const { return reinterpret_cast<R*>(CachedResourceHandleBase::get()); }
        R* operator->() const { return get(); }
               
        CachedResourceHandle& operator=(R* res) { setResource(res); return *this; } 
        CachedResourceHandle& operator=(const CachedResourceHandle& o) { setResource(o.get()); return *this; }
        bool operator==(const CachedResourceHandleBase& o) const { return get() == o.get(); }
        bool operator!=(const CachedResourceHandleBase& o) const { return get() != o.get(); }
    };
    
    template <class R, class RR> bool operator==(const CachedResourceHandle<R>& h, const RR* res) 
    { 
        return h.get() == res; 
    }
    template <class R, class RR> bool operator==(const RR* res, const CachedResourceHandle<R>& h) 
    { 
        return h.get() == res; 
    }
    template <class R, class RR> bool operator!=(const CachedResourceHandle<R>& h, const RR* res) 
    { 
        return h.get() != res; 
    }
    template <class R, class RR> bool operator!=(const RR* res, const CachedResourceHandle<R>& h) 
    { 
        return h.get() != res; 
    }
}

#endif
