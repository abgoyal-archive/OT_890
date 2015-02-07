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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
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
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef ClassNames_h
#define ClassNames_h

#include "AtomicString.h"
#include <wtf/OwnPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

    class ClassNamesData : public Noncopyable {
    public:
        ClassNamesData(const String& string, bool shouldFoldCase)
            : m_string(string), m_shouldFoldCase(shouldFoldCase), m_createdVector(false)
        {
        }

        bool contains(const AtomicString& string)
        {
            ensureVector();
            size_t size = m_vector.size();
            for (size_t i = 0; i < size; ++i) {
                if (m_vector[i] == string)
                    return true;
            }
            return false;
        }

        bool containsAll(ClassNamesData&);

        size_t size() { ensureVector(); return m_vector.size(); }
        const AtomicString& operator[](size_t i) { ensureVector(); ASSERT(i < size()); return m_vector[i]; }

    private:
        void ensureVector() { if (!m_createdVector) createVector(); }
        void createVector();

        typedef Vector<AtomicString, 8> ClassNameVector;
        String m_string;
        ClassNameVector m_vector;
        bool m_shouldFoldCase;
        bool m_createdVector;
    };

    class ClassNames {
    public:
        ClassNames() { }
        ClassNames(const String& string, bool shouldFoldCase) : m_data(new ClassNamesData(string, shouldFoldCase)) { }

        void set(const String& string, bool shouldFoldCase) { m_data.set(new ClassNamesData(string, shouldFoldCase)); }
        void clear() { m_data.clear(); }

        bool contains(const AtomicString& string) const { return m_data && m_data->contains(string); }
        bool containsAll(const ClassNames& names) const { return !names.m_data || (m_data && m_data->containsAll(*names.m_data)); }

        size_t size() const { return m_data ? m_data->size() : 0; }
        const AtomicString& operator[](size_t i) const { ASSERT(i < size()); return (*m_data)[i]; }

    private:
        OwnPtr<ClassNamesData> m_data;
    };

    inline bool isClassWhitespace(UChar c)
    {
        return c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\f';
    }

} // namespace WebCore

#endif // ClassNames_h
