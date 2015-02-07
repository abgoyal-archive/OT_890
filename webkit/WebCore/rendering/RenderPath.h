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
    Copyright (C) 2004, 2005, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>
                  2006 Apple Computer, Inc
                  2009 Google, Inc.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef RenderPath_h
#define RenderPath_h

#if ENABLE(SVG)

#include "FloatRect.h"
#include "RenderSVGModelObject.h"
#include "TransformationMatrix.h"

namespace WebCore {

class FloatPoint;
class RenderSVGContainer;
class SVGStyledTransformableElement;

class RenderPath : public RenderSVGModelObject {
public:
    RenderPath(SVGStyledTransformableElement*);

    const Path& path() const;

private:
    // Hit-detection seperated for the fill and the stroke
    bool fillContains(const FloatPoint&, bool requiresFill = true) const;
    bool strokeContains(const FloatPoint&, bool requiresStroke = true) const;

    virtual FloatRect objectBoundingBox() const;
    virtual FloatRect repaintRectInLocalCoordinates() const;

    virtual TransformationMatrix localToParentTransform() const;

    void setPath(const Path&);

    virtual bool isRenderPath() const { return true; }
    virtual const char* renderName() const { return "RenderPath"; }

    virtual void layout();
    virtual void paint(PaintInfo&, int parentX, int parentY);
    virtual void addFocusRingRects(GraphicsContext*, int tx, int ty);

    virtual bool nodeAtFloatPoint(const HitTestRequest&, HitTestResult&, const FloatPoint& pointInParent, HitTestAction);

    FloatRect drawMarkersIfNeeded(GraphicsContext*, const FloatRect&, const Path&) const;

private:
    virtual TransformationMatrix localTransform() const;

    mutable Path m_path;
    mutable FloatRect m_cachedLocalFillBBox;
    mutable FloatRect m_cachedLocalRepaintRect;
    FloatRect m_markerBounds;
    TransformationMatrix m_localTransform;
};

inline RenderPath* toRenderPath(RenderObject* object)
{
    ASSERT(!object || object->isRenderPath());
    return static_cast<RenderPath*>(object);
}

inline const RenderPath* toRenderPath(const RenderObject* object)
{
    ASSERT(!object || object->isRenderPath());
    return static_cast<const RenderPath*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderPath(const RenderPath*);

}

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
