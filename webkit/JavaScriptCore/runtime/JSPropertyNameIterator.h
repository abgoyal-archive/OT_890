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
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef JSPropertyNameIterator_h
#define JSPropertyNameIterator_h

#include "JSObject.h"
#include "JSString.h"
#include "PropertyNameArray.h"

namespace JSC {

    class Identifier;
    class JSObject;

    class JSPropertyNameIterator : public JSCell {
    public:
        static JSPropertyNameIterator* create(ExecState*, JSValue);

        virtual ~JSPropertyNameIterator();

        virtual JSValue toPrimitive(ExecState*, PreferredPrimitiveType) const;
        virtual bool getPrimitiveNumber(ExecState*, double&, JSValue&);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual UString toString(ExecState*) const;
        virtual JSObject* toObject(ExecState*) const;

        virtual void markChildren(MarkStack&);

        JSValue next(ExecState*);
        void invalidate();
        
        static PassRefPtr<Structure> createStructure(JSValue prototype)
        {
            return Structure::create(prototype, TypeInfo(CompoundType));
        }
    private:
        JSPropertyNameIterator(ExecState*);
        JSPropertyNameIterator(ExecState*, JSObject*, PassRefPtr<PropertyNameArrayData> propertyNameArrayData);

        JSObject* m_object;
        RefPtr<PropertyNameArrayData> m_data;
        PropertyNameArrayData::const_iterator m_position;
        PropertyNameArrayData::const_iterator m_end;
    };

inline JSPropertyNameIterator::JSPropertyNameIterator(ExecState* exec)
    : JSCell(exec->globalData().propertyNameIteratorStructure.get())
    , m_object(0)
    , m_position(0)
    , m_end(0)
{
}

inline JSPropertyNameIterator::JSPropertyNameIterator(ExecState* exec, JSObject* object, PassRefPtr<PropertyNameArrayData> propertyNameArrayData)
    : JSCell(exec->globalData().propertyNameIteratorStructure.get())
    , m_object(object)
    , m_data(propertyNameArrayData)
    , m_position(m_data->begin())
    , m_end(m_data->end())
{
}

inline JSPropertyNameIterator* JSPropertyNameIterator::create(ExecState* exec, JSValue v)
{
    if (v.isUndefinedOrNull())
        return new (exec) JSPropertyNameIterator(exec);

    JSObject* o = v.toObject(exec);
    PropertyNameArray propertyNames(exec);
    o->getPropertyNames(exec, propertyNames);
    return new (exec) JSPropertyNameIterator(exec, o, propertyNames.releaseData());
}

inline JSValue JSPropertyNameIterator::next(ExecState* exec)
{
    if (m_position == m_end)
        return JSValue();

    if (m_data->cachedStructure() == m_object->structure() && m_data->cachedPrototypeChain() == m_object->structure()->prototypeChain(exec))
        return jsOwnedString(exec, (*m_position++).ustring());

    do {
        if (m_object->hasProperty(exec, *m_position))
            return jsOwnedString(exec, (*m_position++).ustring());
        m_position++;
    } while (m_position != m_end);

    return JSValue();
}

} // namespace JSC

#endif // JSPropertyNameIterator_h
