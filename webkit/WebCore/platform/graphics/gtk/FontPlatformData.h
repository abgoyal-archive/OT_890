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
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007 Pioneer Research Center USA, Inc.
 * All rights reserved.
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

#ifndef FontPlatformData_h
#define FontPlatformData_h

#include "GlyphBuffer.h"
#include "FontDescription.h"
#include <cairo.h>
#if defined(USE_FREETYPE)
#include <cairo-ft.h>
#include <fontconfig/fcfreetype.h>
#elif defined(USE_PANGO)
#include <pango/pangocairo.h>
#else
#error "Must defined a font backend"
#endif

namespace WebCore {

class String;

class FontPlatformData {
public:
    FontPlatformData(WTF::HashTableDeletedValueType)
#if defined(USE_FREETYPE)
        : m_pattern(hashTableDeletedFontValue())
        , m_fallbacks(0)
#elif defined(USE_PANGO)
        : m_context(0)
        , m_font(hashTableDeletedFontValue())
#else
#error "Must defined a font backend"
#endif
        , m_scaledFont(0)
        { }

    FontPlatformData()
#if defined(USE_FREETYPE)
        : m_pattern(0)
        , m_fallbacks(0)
#elif defined(USE_PANGO)
        : m_context(0)
        , m_font(0)
#else
#error "Must defined a font backend"
#endif
        , m_scaledFont(0)
        { }

    FontPlatformData(const FontDescription&, const AtomicString& family);

    FontPlatformData(float size, bool bold, bool italic);
    FontPlatformData(cairo_font_face_t* fontFace, int size, bool bold, bool italic);
    FontPlatformData(const FontPlatformData&);

    ~FontPlatformData();

    static bool init();

    bool isFixedPitch();
    float size() const { return m_size; }
    bool syntheticBold() const { return m_syntheticBold; }
    bool syntheticOblique() const { return m_syntheticOblique; }

    void setFont(cairo_t*) const;

    unsigned hash() const
    {
#if defined(USE_FREETYPE)
        if (m_pattern)
            return FcPatternHash(m_pattern);
#endif
        uintptr_t hashCodes[1] = { reinterpret_cast<uintptr_t>(m_scaledFont) };
        return StringImpl::computeHash(reinterpret_cast<UChar*>(hashCodes), sizeof(hashCodes) / sizeof(UChar));
    }

    bool operator==(const FontPlatformData&) const;
    FontPlatformData& operator=(const FontPlatformData&);
    bool isHashTableDeletedValue() const {
#if defined(USE_FREETYPE)
        return m_pattern == hashTableDeletedFontValue();
#elif defined(USE_PANGO)
        return m_font == hashTableDeletedFontValue();
#endif
    };

#ifndef NDEBUG
    String description() const;
#endif

#if defined(USE_FREETYPE)
    FcPattern* m_pattern;
    FcFontSet* m_fallbacks;
#elif defined(USE_PANGO)
    static PangoFontMap* m_fontMap;
    static GHashTable* m_hashTable;

    PangoContext* m_context;
    PangoFont* m_font;
#else
#error "Must defined a font backend"
#endif
    float m_size;
    bool m_syntheticBold;
    bool m_syntheticOblique;
    cairo_scaled_font_t* m_scaledFont;
private:
#if defined(USE_FREETYPE)
    static FcPattern *hashTableDeletedFontValue() { return reinterpret_cast<FcPattern*>(-1); }
#elif defined(USE_PANGO)
    static PangoFont *hashTableDeletedFontValue() { return reinterpret_cast<PangoFont*>(-1); }
#endif
};

}

#endif
