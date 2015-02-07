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
 * Copyright (C) 2007, 2008 Apple Inc. All Rights Reserved.
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

#ifndef COMVariantSetter_h
#define COMVariantSetter_h

#include <WebCore/BString.h>
#include <WebCore/COMPtr.h>
#include <wtf/Assertions.h>

namespace WebCore {
    class String;
}

template<typename T> struct COMVariantSetter {};

template<typename T> struct COMVariantSetterBase
{
    static inline VARENUM variantType(const T&)
    {
        return COMVariantSetter<T>::VariantType;
    }
};

template<> struct COMVariantSetter<WebCore::String> : COMVariantSetterBase<WebCore::String>
{
    static const VARENUM VariantType = VT_BSTR;

    static void setVariant(VARIANT* variant, const WebCore::String& value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        V_VT(variant) = VariantType;
        V_BSTR(variant) = WebCore::BString(value).release();
    }
};

template<> struct COMVariantSetter<unsigned long long> : COMVariantSetterBase<unsigned long long>
{
    static const VARENUM VariantType = VT_UI8;

    static void setVariant(VARIANT* variant, unsigned long long value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        V_VT(variant) = VariantType;
        V_UI8(variant) = value;
    }
};

template<> struct COMVariantSetter<int> : COMVariantSetterBase<int>
{
    static const VARENUM VariantType = VT_I4;

    static void setVariant(VARIANT* variant, int value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        V_VT(variant) = VariantType;
        V_I4(variant) = value;
    }
};

template<typename T> struct COMVariantSetter<COMPtr<T> > : COMVariantSetterBase<COMPtr<T> >
{
    static const VARENUM VariantType = VT_UNKNOWN;

    static void setVariant(VARIANT* variant, const COMPtr<T>& value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        V_VT(variant) = VariantType;
        V_UNKNOWN(variant) = value.get();
        value->AddRef();
    }
};

template<typename COMType, typename UnderlyingType>
struct COMIUnknownVariantSetter : COMVariantSetterBase<UnderlyingType>
{
    static const VARENUM VariantType = VT_UNKNOWN;

    static void setVariant(VARIANT* variant, const UnderlyingType& value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        V_VT(variant) = VariantType;
        V_UNKNOWN(variant) = COMType::createInstance(value);
    }
};

class COMVariant {
public:
    COMVariant()
    {
        ::VariantInit(&m_variant);
    }

    template<typename UnderlyingType>
    COMVariant(UnderlyingType value)
    {
        ::VariantInit(&m_variant);
        COMVariantSetter<UnderlyingType>::setVariant(&m_variant, value);
    }

    ~COMVariant()
    {
        ::VariantClear(&m_variant);
    }

    COMVariant(const COMVariant& other)
    {
        ::VariantInit(&m_variant);
        other.copyTo(&m_variant);
    }

    COMVariant& operator=(const COMVariant& other)
    {
        other.copyTo(&m_variant);
        return *this;
    }

    void copyTo(VARIANT* dest) const
    {
        ::VariantCopy(dest, const_cast<VARIANT*>(&m_variant));
    }

    VARENUM variantType() const { return static_cast<VARENUM>(V_VT(&m_variant)); }

private:
    VARIANT m_variant;
};

template<> struct COMVariantSetter<COMVariant>
{
    static inline VARENUM variantType(const COMVariant& value)
    {
        return value.variantType();
    }

    static void setVariant(VARIANT* variant, const COMVariant& value)
    {
        ASSERT(V_VT(variant) == VT_EMPTY);

        value.copyTo(variant);
    }
};

#endif // COMVariantSetter
