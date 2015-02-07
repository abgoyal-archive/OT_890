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
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2006, 2007, 2009 Apple Inc. All rights reserved.
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

#ifndef RenderBoxModelObject_h
#define RenderBoxModelObject_h

#include "RenderObject.h"

namespace WebCore {

// Values for vertical alignment.
const int PositionTop = -0x7fffffff;
const int PositionBottom = 0x7fffffff;
const int PositionUndefined = 0x80000000;

// This class is the base for all objects that adhere to the CSS box model as described
// at http://www.w3.org/TR/CSS21/box.html

class RenderBoxModelObject : public RenderObject {
public:
    RenderBoxModelObject(Node*);
    virtual ~RenderBoxModelObject();
    
    virtual void destroy();

    int relativePositionOffsetX() const;
    int relativePositionOffsetY() const;
    IntSize relativePositionOffset() const { return IntSize(relativePositionOffsetX(), relativePositionOffsetY()); }

    // IE extensions. Used to calculate offsetWidth/Height.  Overridden by inlines (RenderFlow)
    // to return the remaining width on a given line (and the height of a single line).
    virtual int offsetLeft() const;
    virtual int offsetTop() const;
    virtual int offsetWidth() const = 0;
    virtual int offsetHeight() const = 0;

    virtual void styleWillChange(StyleDifference, const RenderStyle* newStyle);
    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle);
    virtual void updateBoxModelInfoFromStyle();

    bool hasSelfPaintingLayer() const;
    RenderLayer* layer() const { return m_layer; }
    virtual bool requiresLayer() const { return isRoot() || isPositioned() || isRelPositioned() || isTransparent() || hasOverflowClip() || hasTransform() || hasMask() || hasReflection(); }

    // This will work on inlines to return the bounding box of all of the lines' border boxes.
    virtual IntRect borderBoundingBox() const = 0;

    // Virtual since table cells override
    virtual int paddingTop(bool includeIntrinsicPadding = true) const;
    virtual int paddingBottom(bool includeIntrinsicPadding = true) const;
    virtual int paddingLeft(bool includeIntrinsicPadding = true) const;
    virtual int paddingRight(bool includeIntrinsicPadding = true) const;

    virtual int borderTop() const { return style()->borderTopWidth(); }
    virtual int borderBottom() const { return style()->borderBottomWidth(); }
    virtual int borderLeft() const { return style()->borderLeftWidth(); }
    virtual int borderRight() const { return style()->borderRightWidth(); }

    virtual int marginTop() const = 0;
    virtual int marginBottom() const = 0;
    virtual int marginLeft() const = 0;
    virtual int marginRight() const = 0;

    bool hasHorizontalBordersPaddingOrMargin() const { return hasHorizontalBordersOrPadding() || marginLeft() != 0 || marginRight() != 0; }
    bool hasHorizontalBordersOrPadding() const { return borderLeft() != 0 || borderRight() != 0 || paddingLeft() != 0 || paddingRight() != 0; }

    virtual int containingBlockWidthForContent() const;

    virtual void childBecameNonInline(RenderObject* /*child*/) { }

    void paintBorder(GraphicsContext*, int tx, int ty, int w, int h, const RenderStyle*, bool begin = true, bool end = true);
    bool paintNinePieceImage(GraphicsContext*, int tx, int ty, int w, int h, const RenderStyle*, const NinePieceImage&, CompositeOperator = CompositeSourceOver);
    void paintBoxShadow(GraphicsContext*, int tx, int ty, int w, int h, const RenderStyle*, ShadowStyle, bool begin = true, bool end = true);
    void paintFillLayerExtended(const PaintInfo&, const Color&, const FillLayer*, int tx, int ty, int width, int height, InlineFlowBox* = 0, CompositeOperator = CompositeSourceOver);

    // The difference between this inline's baseline position and the line's baseline position.
    int verticalPosition(bool firstLine) const;

    // Called by RenderObject::destroy() (and RenderWidget::destroy()) and is the only way layers should ever be destroyed
    void destroyLayer();

protected:
    void calculateBackgroundImageGeometry(const FillLayer*, int tx, int ty, int w, int h, IntRect& destRect, IntPoint& phase, IntSize& tileSize);
    IntSize calculateBackgroundSize(const FillLayer*, int scaledWidth, int scaledHeight) const;

private:
    virtual bool isBoxModelObject() const { return true; }
    friend class RenderView;

    RenderLayer* m_layer;
    
    // Used to store state between styleWillChange and styleDidChange
    static bool s_wasFloating;
    static bool s_hadLayer;
    static bool s_layerWasSelfPainting;
};

inline RenderBoxModelObject* toRenderBoxModelObject(RenderObject* object)
{ 
    ASSERT(!object || object->isBoxModelObject());
    return static_cast<RenderBoxModelObject*>(object);
}

inline const RenderBoxModelObject* toRenderBoxModelObject(const RenderObject* object)
{ 
    ASSERT(!object || object->isBoxModelObject());
    return static_cast<const RenderBoxModelObject*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderBoxModelObject(const RenderBoxModelObject*);

} // namespace WebCore

#endif // RenderBoxModelObject_h
