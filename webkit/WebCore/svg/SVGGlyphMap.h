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
   Copyright (C) 2008 Apple, Inc

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

#ifndef SVGGlyphMap_h
#define SVGGlyphMap_h

#if ENABLE(SVG_FONTS)
#include "SVGGlyphElement.h"


namespace WebCore {

    struct GlyphMapNode;

    typedef HashMap<UChar, RefPtr<GlyphMapNode> > GlyphMapLayer;

    struct GlyphMapNode : public RefCounted<GlyphMapNode> {
    private:
        GlyphMapNode() { }
    public:
        static PassRefPtr<GlyphMapNode> create() { return adoptRef(new GlyphMapNode); }

        Vector<SVGGlyphIdentifier> glyphs;

        GlyphMapLayer children;
    };

    class SVGGlyphMap {

    public:
        SVGGlyphMap() : m_currentPriority(0) { }

        void add(const String& string, const SVGGlyphIdentifier& glyph) 
        {
            size_t len = string.length();
            GlyphMapLayer* currentLayer = &m_rootLayer;

            RefPtr<GlyphMapNode> node;
            for (size_t i = 0; i < len; i++) {
                UChar curChar = string[i];
                node = currentLayer->get(curChar);
                if (!node) {
                    node = GlyphMapNode::create();
                    currentLayer->set(curChar, node);
                }
                currentLayer = &node->children;
            }

            if (node) {
                node->glyphs.append(glyph);
                node->glyphs.last().priority = m_currentPriority++;
                node->glyphs.last().nameLength = len;
                node->glyphs.last().isValid = true;
            }
        }

        static inline bool compareGlyphPriority(const SVGGlyphIdentifier& first, const SVGGlyphIdentifier& second)
        {
            return first.priority < second.priority;
        }

        void get(const String& string, Vector<SVGGlyphIdentifier>& glyphs)
        {
            GlyphMapLayer* currentLayer = &m_rootLayer;

            for (size_t i = 0; i < string.length(); i++) {
                UChar curChar = string[i];
                RefPtr<GlyphMapNode> node = currentLayer->get(curChar);
                if (!node)
                    break;
                glyphs.append(node->glyphs);
                currentLayer = &node->children;
            }
            std::sort(glyphs.begin(), glyphs.end(), compareGlyphPriority);
        }

        void clear() 
        { 
            m_rootLayer.clear(); 
            m_currentPriority = 0;
        }

    private:
        GlyphMapLayer m_rootLayer;
        int m_currentPriority;
    };

}

#endif // ENABLE(SVG_FONTS)


#endif //SVGGlyphMap_h
