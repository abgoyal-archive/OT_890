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
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
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

#ifndef StyleRareNonInheritedData_h
#define StyleRareNonInheritedData_h

#include "CounterDirectives.h"
#include "CursorData.h"
#include "DataRef.h"
#include "FillLayer.h"
#include "NinePieceImage.h"
#include "StyleTransformData.h"
#include <wtf/OwnPtr.h>
#include <wtf/PassRefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class AnimationList;
class CSSStyleSelector;
class StyleFlexibleBoxData;
class StyleMarqueeData;
class StyleMultiColData;
class StyleReflection;
class StyleTransformData;
struct ContentData;
struct ShadowData;

#if ENABLE(DASHBOARD_SUPPORT)
class StyleDashboardRegion;
#endif

#if ENABLE(XBL)
class BindingURI;
#endif

// This struct is for rarely used non-inherited CSS3, CSS2, and WebKit-specific properties.
// By grouping them together, we save space, and only allocate this object when someone
// actually uses one of these properties.
class StyleRareNonInheritedData : public RefCounted<StyleRareNonInheritedData> {
public:
    static PassRefPtr<StyleRareNonInheritedData> create() { return adoptRef(new StyleRareNonInheritedData); }
    PassRefPtr<StyleRareNonInheritedData> copy() const { return adoptRef(new StyleRareNonInheritedData(*this)); }
    ~StyleRareNonInheritedData();
    
#if ENABLE(XBL)
    bool bindingsEquivalent(const StyleRareNonInheritedData&) const;
#endif

    bool operator==(const StyleRareNonInheritedData&) const;
    bool operator!=(const StyleRareNonInheritedData& o) const { return !(*this == o); }

    bool contentDataEquivalent(const StyleRareNonInheritedData& o) const;
    bool shadowDataEquivalent(const StyleRareNonInheritedData& o) const;
    bool reflectionDataEquivalent(const StyleRareNonInheritedData& o) const;
    bool animationDataEquivalent(const StyleRareNonInheritedData&) const;
    bool transitionDataEquivalent(const StyleRareNonInheritedData&) const;

    int lineClamp; // An Apple extension.
#if ENABLE(DASHBOARD_SUPPORT)
    Vector<StyleDashboardRegion> m_dashboardRegions;
#endif
    float opacity; // Whether or not we're transparent.

    DataRef<StyleFlexibleBoxData> flexibleBox; // Flexible box properties 
    DataRef<StyleMarqueeData> marquee; // Marquee properties
    DataRef<StyleMultiColData> m_multiCol; //  CSS3 multicol properties
    DataRef<StyleTransformData> m_transform; // Transform properties (rotate, scale, skew, etc.)

    OwnPtr<ContentData> m_content;
    OwnPtr<CounterDirectiveMap> m_counterDirectives;

    unsigned userDrag : 2; // EUserDrag
    bool textOverflow : 1; // Whether or not lines that spill out should be truncated with "..."
    unsigned marginTopCollapse : 2; // EMarginCollapse
    unsigned marginBottomCollapse : 2; // EMarginCollapse
    unsigned matchNearestMailBlockquoteColor : 1; // EMatchNearestMailBlockquoteColor, FIXME: This property needs to be eliminated. It should never have been added.
    unsigned m_appearance : 6; // EAppearance
    unsigned m_borderFit : 1; // EBorderFit
#if USE(ACCELERATED_COMPOSITING)
    bool m_runningAcceleratedAnimation : 1;
#endif    
    OwnPtr<ShadowData> m_boxShadow;  // For box-shadow decorations.
    
    RefPtr<StyleReflection> m_boxReflect;

    OwnPtr<AnimationList> m_animations;
    OwnPtr<AnimationList> m_transitions;

    FillLayer m_mask;
    NinePieceImage m_maskBoxImage;

    ETransformStyle3D m_transformStyle3D;
    EBackfaceVisibility m_backfaceVisibility;
    float m_perspective;
    Length m_perspectiveOriginX;
    Length m_perspectiveOriginY;

#if ENABLE(XBL)
    OwnPtr<BindingURI> bindingURI; // The XBL binding URI list.
#endif
    
private:
    StyleRareNonInheritedData();
    StyleRareNonInheritedData(const StyleRareNonInheritedData&);
};

} // namespace WebCore

#endif // StyleRareNonInheritedData_h
