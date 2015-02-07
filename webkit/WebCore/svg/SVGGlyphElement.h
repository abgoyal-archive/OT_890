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
   Copyright (C) 2007 Eric Seidel <eric@webkit.org>
   Copyright (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>
   Copyright (C) 2008 Rob Buis <buis@kde.org>

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

#ifndef SVGGlyphElement_h
#define SVGGlyphElement_h

#if ENABLE(SVG_FONTS)
#include "SVGStyledElement.h"

#include <limits>
#include "Path.h"

namespace WebCore {

    class AtomicString;
    class SVGFontData;

    // Describe a SVG <glyph> element
    struct SVGGlyphIdentifier {
        enum Orientation {
            Vertical,
            Horizontal,
            Both
        };

        // SVG Font depends on exactly this order.
        enum ArabicForm {
            None = 0,
            Isolated,
            Terminal,
            Initial,
            Medial
        };

        SVGGlyphIdentifier()
            : isValid(false)
            , orientation(Both)
            , arabicForm(None)
            , priority(0)
            , nameLength(0)
            , horizontalAdvanceX(0.0f)
            , verticalOriginX(0.0f)
            , verticalOriginY(0.0f)
            , verticalAdvanceY(0.0f)
        {
        }

        // Used to mark our float properties as "to be inherited from SVGFontData"
        static float inheritedValue()
        {
            static float s_inheritedValue = std::numeric_limits<float>::infinity();
            return s_inheritedValue;
        }

        bool operator==(const SVGGlyphIdentifier& other) const
        {
            return isValid == other.isValid &&
                   orientation == other.orientation &&
                   arabicForm == other.arabicForm &&
                   glyphName == other.glyphName &&
                   horizontalAdvanceX == other.horizontalAdvanceX &&
                   verticalOriginX == other.verticalOriginX &&
                   verticalOriginY == other.verticalOriginY &&
                   verticalAdvanceY == other.verticalAdvanceY &&
                   pathData.debugString() == other.pathData.debugString() &&
                   languages == other.languages;
        }

        bool isValid : 1;

        unsigned orientation : 2; // Orientation
        unsigned arabicForm : 3;  // ArabicForm
        int priority;
        size_t nameLength;
        String glyphName;

        float horizontalAdvanceX;
        float verticalOriginX;
        float verticalOriginY;
        float verticalAdvanceY;

        Path pathData;
        Vector<String> languages;
    };

    class SVGGlyphElement : public SVGStyledElement {
    public:
        SVGGlyphElement(const QualifiedName&, Document*);
        virtual ~SVGGlyphElement();

        virtual void parseMappedAttribute(MappedAttribute*);

        virtual void insertedIntoDocument();
        virtual void removedFromDocument();

        virtual bool rendererIsNeeded(RenderStyle*) { return false; }

        SVGGlyphIdentifier buildGlyphIdentifier() const;

        // Helper function used by SVGFont
        static void inheritUnspecifiedAttributes(SVGGlyphIdentifier&, const SVGFontData*);
        static String querySVGFontLanguage(const SVGElement*);

        // Helper function shared between SVGGlyphElement & SVGMissingGlyphElement
        static SVGGlyphIdentifier buildGenericGlyphIdentifier(const SVGElement*);
    private:
        void invalidateGlyphCache();
    };

} // namespace WebCore

#endif // ENABLE(SVG_FONTS)
#endif
