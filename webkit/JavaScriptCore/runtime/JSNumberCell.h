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
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef JSNumberCell_h
#define JSNumberCell_h

#include "CallFrame.h"
#include "JSCell.h"
#include "JSImmediate.h"
#include "Collector.h"
#include "UString.h"
#include <stddef.h> // for size_t

namespace JSC {

    extern const double NaN;
    extern const double Inf;

#if USE(JSVALUE32)
    JSValue jsNumberCell(ExecState*, double);

    class Identifier;
    class JSCell;
    class JSObject;
    class JSString;
    class PropertySlot;

    struct ClassInfo;
    struct Instruction;

    class JSNumberCell : public JSCell {
        friend class JIT;
        friend JSValue jsNumberCell(JSGlobalData*, double);
        friend JSValue jsNumberCell(ExecState*, double);

    public:
        double value() const { return m_value; }

        virtual JSValue toPrimitive(ExecState*, PreferredPrimitiveType) const;
        virtual bool getPrimitiveNumber(ExecState*, double& number, JSValue& value);
        virtual bool toBoolean(ExecState*) const;
        virtual double toNumber(ExecState*) const;
        virtual UString toString(ExecState*) const;
        virtual JSObject* toObject(ExecState*) const;

        virtual UString toThisString(ExecState*) const;
        virtual JSObject* toThisObject(ExecState*) const;
        virtual JSValue getJSNumber();

        void* operator new(size_t size, ExecState* exec)
        {
    #ifdef JAVASCRIPTCORE_BUILDING_ALL_IN_ONE_FILE
            return exec->heap()->inlineAllocateNumber(size);
    #else
            return exec->heap()->allocateNumber(size);
    #endif
        }

        void* operator new(size_t size, JSGlobalData* globalData)
        {
    #ifdef JAVASCRIPTCORE_BUILDING_ALL_IN_ONE_FILE
            return globalData->heap.inlineAllocateNumber(size);
    #else
            return globalData->heap.allocateNumber(size);
    #endif
        }

        static PassRefPtr<Structure> createStructure(JSValue proto) { return Structure::create(proto, TypeInfo(NumberType, NeedsThisConversion)); }

    private:
        JSNumberCell(JSGlobalData* globalData, double value)
            : JSCell(globalData->numberStructure.get())
            , m_value(value)
        {
        }

        JSNumberCell(ExecState* exec, double value)
            : JSCell(exec->globalData().numberStructure.get())
            , m_value(value)
        {
        }

        virtual bool getUInt32(uint32_t&) const;

        double m_value;
    };

    JSValue jsNumberCell(JSGlobalData*, double);

    inline bool isNumberCell(JSValue v)
    {
        return v.isCell() && v.asCell()->isNumber();
    }

    inline JSNumberCell* asNumberCell(JSValue v)
    {
        ASSERT(isNumberCell(v));
        return static_cast<JSNumberCell*>(v.asCell());
    }

    inline JSValue::JSValue(ExecState* exec, double d)
    {
        JSValue v = JSImmediate::from(d);
        *this = v ? v : jsNumberCell(exec, d);
    }

    inline JSValue::JSValue(ExecState* exec, int i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline JSValue::JSValue(ExecState* exec, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline JSValue::JSValue(ExecState* exec, long i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline JSValue::JSValue(ExecState* exec, unsigned long i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, i);
    }

    inline JSValue::JSValue(ExecState* exec, long long i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, static_cast<double>(i));
    }

    inline JSValue::JSValue(ExecState* exec, unsigned long long i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(exec, static_cast<double>(i));
    }

    inline JSValue::JSValue(JSGlobalData* globalData, double d)
    {
        JSValue v = JSImmediate::from(d);
        *this = v ? v : jsNumberCell(globalData, d);
    }

    inline JSValue::JSValue(JSGlobalData* globalData, int i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(globalData, i);
    }

    inline JSValue::JSValue(JSGlobalData* globalData, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        *this = v ? v : jsNumberCell(globalData, i);
    }

    inline bool JSValue::isDouble() const
    {
        return isNumberCell(asValue());
    }

    inline double JSValue::asDouble() const
    {
        return asNumberCell(asValue())->value();
    }

    inline bool JSValue::isNumber() const
    {
        return JSImmediate::isNumber(asValue()) || isDouble();
    }

    inline double JSValue::uncheckedGetNumber() const
    {
        ASSERT(isNumber());
        return JSImmediate::isImmediate(asValue()) ? JSImmediate::toDouble(asValue()) : asDouble();
    }

#endif // USE(JSVALUE32)

#if USE(JSVALUE64)
    inline JSValue::JSValue(ExecState*, double d)
    {
        JSValue v = JSImmediate::from(d);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, int i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, long i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned long i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, long long i)
    {
        JSValue v = JSImmediate::from(static_cast<double>(i));
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned long long i)
    {
        JSValue v = JSImmediate::from(static_cast<double>(i));
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, double d)
    {
        JSValue v = JSImmediate::from(d);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, int i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        ASSERT(v);
        *this = v;
    }

    inline bool JSValue::isDouble() const
    {
        return JSImmediate::isDouble(asValue());
    }

    inline double JSValue::asDouble() const
    {
        return JSImmediate::doubleValue(asValue());
    }

    inline bool JSValue::isNumber() const
    {
        return JSImmediate::isNumber(asValue());
    }

    inline double JSValue::uncheckedGetNumber() const
    {
        ASSERT(isNumber());
        return JSImmediate::toDouble(asValue());
    }

#endif // USE(JSVALUE64)

#if USE(JSVALUE32) || USE(JSVALUE64)

    inline JSValue::JSValue(ExecState*, char i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, unsigned char i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, short i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, unsigned short i)
    {
        ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue jsNaN(ExecState* exec)
    {
        return jsNumber(exec, NaN);
    }

    inline JSValue jsNaN(JSGlobalData* globalData)
    {
        return jsNumber(globalData, NaN);
    }

    // --- JSValue inlines ----------------------------

    ALWAYS_INLINE JSValue JSValue::toJSNumber(ExecState* exec) const
    {
        return isNumber() ? asValue() : jsNumber(exec, this->toNumber(exec));
    }

    inline bool JSValue::getNumber(double &result) const
    {
        if (isInt32())
            result = asInt32();
        else if (LIKELY(isDouble()))
            result = asDouble();
        else {
            ASSERT(!isNumber());
            return false;
        }
        return true;
    }

#endif // USE(JSVALUE32) || USE(JSVALUE64)

} // namespace JSC

#endif // JSNumberCell_h
