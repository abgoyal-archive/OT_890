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

// This file has no guards on purpose in order to detect redundant includes. This is a private header
// and so this may catch someone trying to include this file in public cpp files.

#include "FontSelector.h"
#include "SimpleFontData.h"
#include <wtf/Forward.h>

namespace WebCore {

class Font;
class GlyphPageTreeNode;
class GraphicsContext;
class IntRect;
class FontDescription;
class FontPlatformData;
class FontSelector;

const int cAllFamiliesScanned = -1;

class FontFallbackList : public RefCounted<FontFallbackList> {
public:
    static PassRefPtr<FontFallbackList> create() { return adoptRef(new FontFallbackList()); }

    ~FontFallbackList() { releaseFontData(); }
    void invalidate(PassRefPtr<FontSelector>);
    
    bool isFixedPitch(const Font* f) const { if (m_pitch == UnknownPitch) determinePitch(f); return m_pitch == FixedPitch; };
    void determinePitch(const Font*) const;

    bool loadingCustomFonts() const { return m_loadingCustomFonts; }

    FontSelector* fontSelector() const { return m_fontSelector.get(); }
    unsigned generation() const { return m_generation; }

private:
    FontFallbackList();

    const SimpleFontData* primarySimpleFontData(const Font* f)
    { 
        ASSERT(isMainThread());
        if (!m_cachedPrimarySimpleFontData)
            m_cachedPrimarySimpleFontData = primaryFontData(f)->fontDataForCharacter(' ');
        return m_cachedPrimarySimpleFontData;
    }

    const FontData* primaryFontData(const Font* f) const { return fontDataAt(f, 0); }
    const FontData* fontDataAt(const Font*, unsigned index) const;
    const FontData* fontDataForCharacters(const Font*, const UChar*, int length) const;
    
    void setPlatformFont(const FontPlatformData&);

    void releaseFontData();

    mutable Vector<pair<const FontData*, bool>, 1> m_fontList;
    mutable HashMap<int, GlyphPageTreeNode*> m_pages;
    mutable GlyphPageTreeNode* m_pageZero;
    mutable const SimpleFontData* m_cachedPrimarySimpleFontData;
    RefPtr<FontSelector> m_fontSelector;
    mutable int m_familyIndex;
    mutable Pitch m_pitch;
    mutable bool m_loadingCustomFonts;
    unsigned m_generation;

    friend class Font;
};

}

