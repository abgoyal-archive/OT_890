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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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

#ifndef JSByteArray_h
#define JSByteArray_h

#include "JSObject.h"

#include <wtf/ByteArray.h>

namespace JSC {

    class JSByteArray : public JSObject {
        friend struct VPtrSet;
    public:
        bool canAccessIndex(unsigned i) { return i < m_storage->length(); }
        JSValue getIndex(ExecState* exec, unsigned i)
        {
            ASSERT(canAccessIndex(i));
            return jsNumber(exec, m_storage->data()[i]);
        }

        void setIndex(unsigned i, int value)
        {
            ASSERT(canAccessIndex(i));
            if (value & ~0xFF) {
                if (value < 0)
                    value = 0;
                else
                    value = 255;
            }
            m_storage->data()[i] = static_cast<unsigned char>(value);
        }
        
        void setIndex(unsigned i, double value)
        {
            ASSERT(canAccessIndex(i));
            if (!(value > 0)) // Clamp NaN to 0
                value = 0;
            else if (value > 255)
                value = 255;
            m_storage->data()[i] = static_cast<unsigned char>(value + 0.5);
        }
        
        void setIndex(ExecState* exec, unsigned i, JSValue value)
        {
            double byteValue = value.toNumber(exec);
            if (exec->hadException())
                return;
            if (canAccessIndex(i))
                setIndex(i, byteValue);
        }

        JSByteArray(ExecState* exec, PassRefPtr<Structure>, WTF::ByteArray* storage, const JSC::ClassInfo* = &s_defaultInfo);
        static PassRefPtr<Structure> createStructure(JSValue prototype);
        
        virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertySlot&);
        virtual bool getOwnPropertySlot(JSC::ExecState*, unsigned propertyName, JSC::PropertySlot&);
        virtual void put(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::JSValue, JSC::PutPropertySlot&);
        virtual void put(JSC::ExecState*, unsigned propertyName, JSC::JSValue);

        virtual void getPropertyNames(JSC::ExecState*, JSC::PropertyNameArray&);

        virtual const ClassInfo* classInfo() const { return m_classInfo; }
        static const ClassInfo s_defaultInfo;
        
        size_t length() const { return m_storage->length(); }

        WTF::ByteArray* storage() const { return m_storage.get(); }

    private:
        enum VPtrStealingHackType { VPtrStealingHack };
        JSByteArray(VPtrStealingHackType) 
            : JSObject(createStructure(jsNull()))
            , m_classInfo(0)
        {
        }

        RefPtr<WTF::ByteArray> m_storage;
        const ClassInfo* m_classInfo;
    };
    
    JSByteArray* asByteArray(JSValue value);
    inline JSByteArray* asByteArray(JSValue value)
    {
        return static_cast<JSByteArray*>(asCell(value));
    }

    inline bool isJSByteArray(JSGlobalData* globalData, JSValue v) { return v.isCell() && v.asCell()->vptr() == globalData->jsByteArrayVPtr; }

} // namespace JSC

#endif // JSByteArray_h
