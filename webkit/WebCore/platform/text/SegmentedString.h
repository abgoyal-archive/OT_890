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
    Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.

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

#ifndef SegmentedString_h
#define SegmentedString_h

#include "PlatformString.h"
#include <wtf/Deque.h>

namespace WebCore {

class SegmentedString;

class SegmentedSubstring {
public:
    SegmentedSubstring() : m_length(0), m_current(0), m_doNotExcludeLineNumbers(true) {}
    SegmentedSubstring(const String& str)
        : m_length(str.length())
        , m_current(str.isEmpty() ? 0 : str.characters())
        , m_string(str)
        , m_doNotExcludeLineNumbers(true)
    {
    }

    SegmentedSubstring(const UChar* str, int length) : m_length(length), m_current(length == 0 ? 0 : str), m_doNotExcludeLineNumbers(true) {}

    void clear() { m_length = 0; m_current = 0; }
    
    bool excludeLineNumbers() const { return !m_doNotExcludeLineNumbers; }
    bool doNotExcludeLineNumbers() const { return m_doNotExcludeLineNumbers; }

    void setExcludeLineNumbers() { m_doNotExcludeLineNumbers = false; }

    void appendTo(String& str) const
    {
        if (m_string.characters() == m_current) {
            if (str.isEmpty())
                str = m_string;
            else
                str.append(m_string);
        } else {
            str.append(String(m_current, m_length));
        }
    }

public:
    int m_length;
    const UChar* m_current;

private:
    String m_string;
    bool m_doNotExcludeLineNumbers;
};

class SegmentedString {
public:
    SegmentedString()
        : m_pushedChar1(0), m_pushedChar2(0), m_currentChar(0), m_composite(false) {}
    SegmentedString(const UChar* str, int length) : m_pushedChar1(0), m_pushedChar2(0)
        , m_currentString(str, length), m_currentChar(m_currentString.m_current), m_composite(false) {}
    SegmentedString(const String& str)
        : m_pushedChar1(0), m_pushedChar2(0), m_currentString(str)
        , m_currentChar(m_currentString.m_current), m_composite(false) {}
    SegmentedString(const SegmentedString&);

    const SegmentedString& operator=(const SegmentedString&);

    void clear();

    void append(const SegmentedString&);
    void prepend(const SegmentedString&);
    
    bool excludeLineNumbers() const { return m_currentString.excludeLineNumbers(); }
    void setExcludeLineNumbers();

    void push(UChar c)
    {
        if (!m_pushedChar1) {
            m_pushedChar1 = c;
            m_currentChar = m_pushedChar1 ? &m_pushedChar1 : m_currentString.m_current;
        } else {
            ASSERT(!m_pushedChar2);
            m_pushedChar2 = c;
        }
    }
    
    bool isEmpty() const { return !current(); }
    unsigned length() const;

    void advance()
    {
        if (!m_pushedChar1 && m_currentString.m_length > 1) {
            --m_currentString.m_length;
            m_currentChar = ++m_currentString.m_current;
            return;
        }
        advanceSlowCase();
    }
    
    void advancePastNewline(int& lineNumber)
    {
        ASSERT(*current() == '\n');
        if (!m_pushedChar1 && m_currentString.m_length > 1) {
            lineNumber += m_currentString.doNotExcludeLineNumbers();
            --m_currentString.m_length;
            m_currentChar = ++m_currentString.m_current;
            return;
        }
        advanceSlowCase(lineNumber);
    }
    
    void advancePastNonNewline()
    {
        ASSERT(*current() != '\n');
        if (!m_pushedChar1 && m_currentString.m_length > 1) {
            --m_currentString.m_length;
            m_currentChar = ++m_currentString.m_current;
            return;
        }
        advanceSlowCase();
    }
    
    void advance(int& lineNumber)
    {
        if (!m_pushedChar1 && m_currentString.m_length > 1) {
            lineNumber += (*m_currentString.m_current == '\n') & m_currentString.doNotExcludeLineNumbers();
            --m_currentString.m_length;
            m_currentChar = ++m_currentString.m_current;
            return;
        }
        advanceSlowCase(lineNumber);
    }
    
    bool escaped() const { return m_pushedChar1; }
    
    String toString() const;

    const UChar& operator*() const { return *current(); }
    const UChar* operator->() const { return current(); }
    
private:
    void append(const SegmentedSubstring&);
    void prepend(const SegmentedSubstring&);

    void advanceSlowCase();
    void advanceSlowCase(int& lineNumber);
    void advanceSubstring();
    const UChar* current() const { return m_currentChar; }

    UChar m_pushedChar1;
    UChar m_pushedChar2;
    SegmentedSubstring m_currentString;
    const UChar* m_currentChar;
    Deque<SegmentedSubstring> m_substrings;
    bool m_composite;
};

}

#endif
