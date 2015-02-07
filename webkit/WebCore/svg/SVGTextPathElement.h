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
    Copyright (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>

    This file is part of the KDE project

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

#ifndef SVGTextPathElement_h
#define SVGTextPathElement_h

#if ENABLE(SVG)
#include "SVGTextContentElement.h"

#include "SVGURIReference.h"

namespace WebCore {
    enum SVGTextPathMethodType {
        SVG_TEXTPATH_METHODTYPE_UNKNOWN = 0,
        SVG_TEXTPATH_METHODTYPE_ALIGN = 1,
        SVG_TEXTPATH_METHODTYPE_STRETCH = 2
    };

    enum SVGTextPathSpacingType {
        SVG_TEXTPATH_SPACINGTYPE_UNKNOWN = 0,
        SVG_TEXTPATH_SPACINGTYPE_AUTO = 1,
        SVG_TEXTPATH_SPACINGTYPE_EXACT = 2
    };

    class SVGTextPathElement : public SVGTextContentElement,
                               public SVGURIReference {
    public:
        // Forward declare these enums in the w3c naming scheme, for IDL generation
        enum {
            TEXTPATH_METHODTYPE_UNKNOWN = SVG_TEXTPATH_METHODTYPE_UNKNOWN,
            TEXTPATH_METHODTYPE_ALIGN = SVG_TEXTPATH_METHODTYPE_ALIGN,
            TEXTPATH_METHODTYPE_STRETCH = SVG_TEXTPATH_METHODTYPE_STRETCH,
            TEXTPATH_SPACINGTYPE_UNKNOWN = SVG_TEXTPATH_SPACINGTYPE_UNKNOWN,
            TEXTPATH_SPACINGTYPE_AUTO = SVG_TEXTPATH_SPACINGTYPE_AUTO,
            TEXTPATH_SPACINGTYPE_EXACT = SVG_TEXTPATH_SPACINGTYPE_EXACT
        };

        SVGTextPathElement(const QualifiedName&, Document*);
        virtual ~SVGTextPathElement();
 
        virtual void insertedIntoDocument();

        virtual void parseMappedAttribute(MappedAttribute*);
        virtual bool rendererIsNeeded(RenderStyle* style) { return StyledElement::rendererIsNeeded(style); }
        virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);

        bool childShouldCreateRenderer(Node*) const;

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPathElement, SVGNames::textPathTagString, SVGNames::startOffsetAttrString, SVGLength, StartOffset, startOffset)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPathElement, SVGNames::textPathTagString, SVGNames::methodAttrString, int, Method, method)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPathElement, SVGNames::textPathTagString, SVGNames::spacingAttrString, int, Spacing, spacing)
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif
