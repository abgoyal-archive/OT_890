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

#ifndef StyleRareInheritedData_h
#define StyleRareInheritedData_h

#include "AtomicString.h"
#include "Color.h"
#include <wtf/RefCounted.h>
#include <wtf/PassRefPtr.h>

namespace WebCore {

struct ShadowData;

// This struct is for rarely used inherited CSS3, CSS2, and WebKit-specific properties.
// By grouping them together, we save space, and only allocate this object when someone
// actually uses one of these properties.
class StyleRareInheritedData : public RefCounted<StyleRareInheritedData> {
public:
    static PassRefPtr<StyleRareInheritedData> create() { return adoptRef(new StyleRareInheritedData); }
    PassRefPtr<StyleRareInheritedData> copy() const { return adoptRef(new StyleRareInheritedData(*this)); }
    ~StyleRareInheritedData();

    bool operator==(const StyleRareInheritedData& o) const;
    bool operator!=(const StyleRareInheritedData& o) const
    {
        return !(*this == o);
    }
    bool shadowDataEquivalent(const StyleRareInheritedData&) const;

    Color textStrokeColor;
    float textStrokeWidth;
    Color textFillColor;
#ifdef ANDROID_CSS_TAP_HIGHLIGHT_COLOR
    Color tapHighlightColor;
#endif

    ShadowData* textShadow; // Our text shadow information for shadowed text drawing.
    AtomicString highlight; // Apple-specific extension for custom highlight rendering.
    unsigned textSecurity : 2; // ETextSecurity
    unsigned userModify : 2; // EUserModify (editing)
    unsigned wordBreak : 2; // EWordBreak
    unsigned wordWrap : 1; // EWordWrap 
    unsigned nbspMode : 1; // ENBSPMode
    unsigned khtmlLineBreak : 1; // EKHTMLLineBreak
    bool textSizeAdjust : 1; // An Apple extension.
    unsigned resize : 2; // EResize
    unsigned userSelect : 1;  // EUserSelect
    
private:
    StyleRareInheritedData();
    StyleRareInheritedData(const StyleRareInheritedData&);
};

} // namespace WebCore

#endif // StyleRareInheritedData_h
