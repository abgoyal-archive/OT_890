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
    Copyright (C) 1999 Lars Knoll (knoll@kde.org)
    Copyright (C) 2006, 2008 Apple Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef Length_h
#define Length_h

#include <wtf/Assertions.h>
#include <wtf/MathExtras.h>

namespace WebCore {

class String;

const int undefinedLength = -1;
const int percentScaleFactor = 128;

enum LengthType { Auto, Relative, Percent, Fixed, Static, Intrinsic, MinIntrinsic };

struct Length {
    Length()
        : m_value(0)
    {
    }

    Length(LengthType t)
        : m_value(t)
    {
    }

    Length(int v, LengthType t, bool q = false)
        : m_value((v * 16) | (q << 3) | t) // FIXME: Doesn't work if the passed-in value is very large!
    {
        ASSERT(t != Percent);
    }

    Length(double v, LengthType t, bool q = false)
        : m_value(static_cast<int>(v * percentScaleFactor) * 16 | (q << 3) | t)
    {
        ASSERT(t == Percent);
    }

    bool operator==(const Length& o) const { return m_value == o.m_value; }
    bool operator!=(const Length& o) const { return m_value != o.m_value; }

    int value() const {
        ASSERT(type() != Percent);
        return rawValue();
    }

    int rawValue() const { return (m_value & ~0xF) / 16; }

    double percent() const
    {
        ASSERT(type() == Percent);
        return static_cast<double>(rawValue()) / percentScaleFactor;
    }

    LengthType type() const { return static_cast<LengthType>(m_value & 7); }
    bool quirk() const { return (m_value >> 3) & 1; }

    void setValue(LengthType t, int value)
    {
        ASSERT(t != Percent);
        setRawValue(t, value);
    }

    void setRawValue(LengthType t, int value) { m_value = value * 16 | (m_value & 0x8) | t; }

    void setValue(int value)
    {
        ASSERT(!value || type() != Percent);
        setRawValue(value);
    }

    void setRawValue(int value) { m_value = value * 16 | (m_value & 0xF); }

    void setValue(LengthType t, double value)
    {
        ASSERT(t == Percent);
        m_value = static_cast<int>(value * percentScaleFactor) * 16 | (m_value & 0x8) | t;
    }

    void setValue(double value)
    {
        ASSERT(type() == Percent);
        m_value = static_cast<int>(value * percentScaleFactor) * 16 | (m_value & 0xF);
    }

    // note: works only for certain types, returns undefinedLength otherwise
    int calcValue(int maxValue, bool roundPercentages = false) const
    {
        switch (type()) {
            case Fixed:
                return value();
            case Percent:
                if (roundPercentages)
                    return static_cast<int>(round(maxValue * percent() / 100.0));
                return maxValue * rawValue() / (100 * percentScaleFactor);
            case Auto:
                return maxValue;
            default:
                return undefinedLength;
        }
    }

    int calcMinValue(int maxValue, bool roundPercentages = false) const
    {
        switch (type()) {
            case Fixed:
                return value();
            case Percent:
                if (roundPercentages)
                    return static_cast<int>(round(maxValue * percent() / 100.0));
                return maxValue * rawValue() / (100 * percentScaleFactor);
            case Auto:
            default:
                return 0;
        }
    }

    float calcFloatValue(int maxValue) const
    {
        switch (type()) {
            case Fixed:
                return static_cast<float>(value());
            case Percent:
                return static_cast<float>(maxValue * percent() / 100.0);
            case Auto:
                return static_cast<float>(maxValue);
            default:
                return static_cast<float>(undefinedLength);
        }
    }

    bool isUndefined() const { return rawValue() == undefinedLength; }
    bool isZero() const { return !(m_value & ~0xF); }
    bool isPositive() const { return rawValue() > 0; }
    bool isNegative() const { return rawValue() < 0; }

    bool isAuto() const { return type() == Auto; }
    bool isRelative() const { return type() == Relative; }
    bool isPercent() const { return type() == Percent; }
    bool isFixed() const { return type() == Fixed; }
    bool isStatic() const { return type() == Static; }
    bool isIntrinsicOrAuto() const { return type() == Auto || type() == MinIntrinsic || type() == Intrinsic; }

    Length blend(const Length& from, double progress) const
    {
        // Blend two lengths to produce a new length that is in between them.  Used for animation.
        if (!from.isZero() && !isZero() && from.type() != type())
            return *this;

        if (from.isZero() && isZero())
            return *this;
        
        LengthType resultType = type();
        if (isZero())
            resultType = from.type();
        
        if (resultType == Percent) {
            double fromPercent = from.isZero() ? 0. : from.percent();
            double toPercent = isZero() ? 0. : percent();
            return Length(fromPercent + (toPercent - fromPercent) * progress, Percent);
        } 
            
        int fromValue = from.isZero() ? 0 : from.value();
        int toValue = isZero() ? 0 : value();
        return Length(int(fromValue + (toValue - fromValue) * progress), resultType);
    }

private:
    int m_value;
};

Length* newCoordsArray(const String&, int& len);
Length* newLengthArray(const String&, int& len);

} // namespace WebCore

#endif // Length_h
