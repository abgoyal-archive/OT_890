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
 *  Copyright (C) 2005, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef PropertySlot_h
#define PropertySlot_h

#include "Identifier.h"
#include "JSValue.h"
#include "Register.h"
#include <wtf/Assertions.h>
#include <wtf/NotFound.h>

namespace JSC {

    class ExecState;
    class JSObject;

#define JSC_VALUE_SLOT_MARKER 0
#define JSC_REGISTER_SLOT_MARKER reinterpret_cast<GetValueFunc>(1)

    class PropertySlot {
    public:
        PropertySlot()
        {
            clearBase();
            clearOffset();
            clearValue();
        }

        explicit PropertySlot(const JSValue base)
            : m_slotBase(base)
        {
            clearOffset();
            clearValue();
        }

        typedef JSValue (*GetValueFunc)(ExecState*, const Identifier&, const PropertySlot&);

        JSValue getValue(ExecState* exec, const Identifier& propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_data.valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_data.registerSlot).jsValue();
            return m_getValue(exec, propertyName, *this);
        }

        JSValue getValue(ExecState* exec, unsigned propertyName) const
        {
            if (m_getValue == JSC_VALUE_SLOT_MARKER)
                return *m_data.valueSlot;
            if (m_getValue == JSC_REGISTER_SLOT_MARKER)
                return (*m_data.registerSlot).jsValue();
            return m_getValue(exec, Identifier::from(exec, propertyName), *this);
        }

        bool isCacheable() const { return m_offset != WTF::notFound; }
        size_t cachedOffset() const
        {
            ASSERT(isCacheable());
            return m_offset;
        }

        void setValueSlot(JSValue* valueSlot) 
        {
            ASSERT(valueSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_data.valueSlot = valueSlot;
        }
        
        void setValueSlot(JSValue slotBase, JSValue* valueSlot)
        {
            ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_data.valueSlot = valueSlot;
        }
        
        void setValueSlot(JSValue slotBase, JSValue* valueSlot, size_t offset)
        {
            ASSERT(valueSlot);
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_slotBase = slotBase;
            m_data.valueSlot = valueSlot;
            m_offset = offset;
        }
        
        void setValue(JSValue value)
        {
            ASSERT(value);
            clearBase();
            clearOffset();
            m_getValue = JSC_VALUE_SLOT_MARKER;
            m_value = value;
            m_data.valueSlot = &m_value;
        }

        void setRegisterSlot(Register* registerSlot)
        {
            ASSERT(registerSlot);
            clearBase();
            clearOffset();
            m_getValue = JSC_REGISTER_SLOT_MARKER;
            m_data.registerSlot = registerSlot;
        }

        void setCustom(JSValue slotBase, GetValueFunc getValue)
        {
            ASSERT(slotBase);
            ASSERT(getValue);
            m_getValue = getValue;
            m_slotBase = slotBase;
        }

        void setCustomIndex(JSValue slotBase, unsigned index, GetValueFunc getValue)
        {
            ASSERT(slotBase);
            ASSERT(getValue);
            m_getValue = getValue;
            m_slotBase = slotBase;
            m_data.index = index;
        }
        
        void setGetterSlot(JSObject* getterFunc)
        {
            ASSERT(getterFunc);
            m_getValue = functionGetter;
            m_data.getterFunc = getterFunc;
        }
        
        void setUndefined()
        {
            setValue(jsUndefined());
        }

        JSValue slotBase() const
        {
            return m_slotBase;
        }

        void setBase(JSValue base)
        {
            ASSERT(m_slotBase);
            ASSERT(base);
            m_slotBase = base;
        }

        void clearBase()
        {
#ifndef NDEBUG
            m_slotBase = JSValue();
#endif
        }

        void clearValue()
        {
#ifndef NDEBUG
            m_value = JSValue();
#endif
        }

        void clearOffset()
        {
            // Clear offset even in release builds, in case this PropertySlot has been used before.
            // (For other data members, we don't need to clear anything because reuse would meaningfully overwrite them.)
            m_offset = WTF::notFound;
        }

        unsigned index() const { return m_data.index; }

    private:
        static JSValue functionGetter(ExecState*, const Identifier&, const PropertySlot&);

        GetValueFunc m_getValue;
        
        JSValue m_slotBase;
        union {
            JSObject* getterFunc;
            JSValue* valueSlot;
            Register* registerSlot;
            unsigned index;
        } m_data;

        JSValue m_value;

        size_t m_offset;
    };

} // namespace JSC

#endif // PropertySlot_h