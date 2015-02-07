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
    Copyright (C) 2006 Alexander Kellett <lypanov@kde.org>
    Copyright (C) 2006, 2009 Apple Inc. All rights reserved.
    Copyright (C) 2007 Rob Buis <buis@kde.org>
    Copyright (C) 2009 Google, Inc.

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

#ifndef RenderSVGImage_h
#define RenderSVGImage_h

#if ENABLE(SVG)

#include "TransformationMatrix.h"
#include "FloatRect.h"
#include "RenderImage.h"
#include "SVGRenderSupport.h"

namespace WebCore {

    class SVGImageElement;
    class SVGPreserveAspectRatio;

    class RenderSVGImage : public RenderImage, SVGRenderBase {
    public:
        RenderSVGImage(SVGImageElement*);

    private:
        virtual const char* renderName() const { return "RenderSVGImage"; }
        virtual bool isSVGImage() const { return true; }

        virtual TransformationMatrix localToParentTransform() const { return m_localTransform; }

        virtual FloatRect objectBoundingBox() const;
        virtual FloatRect repaintRectInLocalCoordinates() const;

        virtual IntRect clippedOverflowRectForRepaint(RenderBoxModelObject* repaintContainer);
        virtual void computeRectForRepaint(RenderBoxModelObject* repaintContainer, IntRect&, bool fixed = false);

        virtual void mapLocalToContainer(RenderBoxModelObject* repaintContainer, bool useTransforms, bool fixed, TransformState&) const;

        virtual void absoluteRects(Vector<IntRect>&, int tx, int ty);
        virtual void absoluteQuads(Vector<FloatQuad>&);
        virtual void addFocusRingRects(GraphicsContext*, int tx, int ty);

        virtual void imageChanged(WrappedImagePtr, const IntRect* = 0);
        void adjustRectsForAspectRatio(FloatRect& destRect, FloatRect& srcRect, SVGPreserveAspectRatio*);
        
        virtual void layout();
        virtual void paint(PaintInfo&, int parentX, int parentY);

        virtual bool requiresLayer() const { return false; }

        virtual bool nodeAtFloatPoint(const HitTestRequest&, HitTestResult&, const FloatPoint& pointInParent, HitTestAction);
        virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty, HitTestAction);

        virtual TransformationMatrix localTransform() const { return m_localTransform; }

        TransformationMatrix m_localTransform;
        FloatRect m_localBounds;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // RenderSVGImage_h

// vim:ts=4:noet
