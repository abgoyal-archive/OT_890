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
 * This file is part of the internal font implementation.  It should not be included by anyone other than
 * FontMac.cpp, FontWin.cpp and Font.cpp.
 *
 * Copyright (C) 2006, 2007 Apple Inc.
 * Copyright (C) 2007-2008 Torch Mobile, Inc.
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
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef FontPlatformDataWince_H
#define FontPlatformDataWince_H

#include "FontDescription.h"
#include "StringImpl.h"
#include <wtf/Noncopyable.h>

typedef struct tagTEXTMETRICW TEXTMETRIC;
typedef struct tagLOGFONTW LOGFONT;

namespace WebCore {

    class FontPlatformPrivateData;
    class String;

    class FontPlatformData {

    public:

        FontPlatformData(): m_private(0) {}
        FontPlatformData(float size, bool bold, bool oblique);

        // Used for deleted values in the font cache's hash tables.
        FontPlatformData(WTF::HashTableDeletedValueType) : m_private((FontPlatformPrivateData*)1) {}
        bool isHashTableDeletedValue() const { return (unsigned)m_private == 1; }

        FontPlatformData(const FontDescription& fontDescription, const AtomicString& family, bool useDefaultFontIfNotPresent = true);

        ~FontPlatformData();

        FontPlatformData(const FontPlatformData& o) : m_private(0) { operator=(o); }
        FontPlatformData& operator=(const FontPlatformData& o);

        int isValid() const { return reinterpret_cast<unsigned>(m_private) & ~1; }
        HFONT hfont() const;
        const TEXTMETRIC& metrics() const;
        bool isSystemFont() const;
        int size() const;
        unsigned hash() const { return (unsigned)m_private; }
        const FontDescription& fontDescription() const;
        const AtomicString& family() const;
        bool operator==(const FontPlatformData& other) const {     return m_private == other.m_private; }
        HFONT getScaledFontHandle(int height, int width) const;
        const LOGFONT& logFont() const;
        int averageCharWidth() const;
        bool isDisabled() const;
        bool discardFontHandle();
        DWORD codePages() const;

        static bool isSongTiSupported();
        static bool mapKnownFont(DWORD codePages, String& family);
        static DWORD getKnownFontCodePages(const wchar_t* family);
        static const String& defaultFontFamily();
        static LONG adjustedGDIFontWeight(LONG gdiFontWeight, const String& family);

#ifndef NDEBUG
        String description() const;
#endif

    private:
        FontPlatformPrivateData* m_private;
    };

}

#endif
