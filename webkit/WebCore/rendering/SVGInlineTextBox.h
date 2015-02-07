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
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 2007 Rob Buis <buis@kde.org>
 *           (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>
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

#ifndef SVGInlineTextBox_h
#define SVGInlineTextBox_h

#if ENABLE(SVG)
#include "InlineTextBox.h"

namespace WebCore {

    class SVGRootInlineBox;

    struct SVGChar;
    struct SVGTextDecorationInfo;

    class SVGInlineTextBox : public InlineTextBox {
    public:
        SVGInlineTextBox(RenderObject* obj);

        virtual int virtualHeight() const { return m_height; }
        void setHeight(int h) { m_height = h; }

        virtual int selectionTop();
        virtual int selectionHeight();

        virtual int offsetForPosition(int x, bool includePartialGlyphs = true) const;
        virtual int positionForOffset(int offset) const;

        virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty);
        virtual IntRect selectionRect(int absx, int absy, int startPos, int endPos);

        // SVGs custom paint text method
        void paintCharacters(RenderObject::PaintInfo&, int tx, int ty, const SVGChar&, const UChar* chars, int length, SVGPaintServer*);

        // SVGs custom paint selection method
        void paintSelection(int boxStartOffset, const SVGChar&, const UChar*, int length, GraphicsContext*, RenderStyle*, const Font&);

        // SVGs custom paint decoration method
        void paintDecoration(ETextDecoration, GraphicsContext*, int tx, int ty, int width, const SVGChar&, const SVGTextDecorationInfo&);
 
        SVGRootInlineBox* svgRootInlineBox() const;

        // Helper functions shared with SVGRootInlineBox     
        float calculateGlyphWidth(RenderStyle* style, int offset, int extraCharsAvailable, int& charsConsumed, String& glyphName) const;
        float calculateGlyphHeight(RenderStyle*, int offset, int extraCharsAvailable) const;

        FloatRect calculateGlyphBoundaries(RenderStyle*, int offset, const SVGChar&) const;
        SVGChar* closestCharacterToPosition(int x, int y, int& offset) const;

    private:
        friend class RenderSVGInlineText;
        bool svgCharacterHitsPosition(int x, int y, int& offset) const;
        
        int m_height;
    };

} // namespace WebCore

#endif
#endif // SVGInlineTextBox_h
