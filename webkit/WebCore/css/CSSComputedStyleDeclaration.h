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
 * Copyright (C) 2004 Zack Rusin <zack@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef CSSComputedStyleDeclaration_h
#define CSSComputedStyleDeclaration_h

#include "CSSStyleDeclaration.h"
#include "Node.h"

namespace WebCore {

class CSSMutableStyleDeclaration;

enum EUpdateLayout { DoNotUpdateLayout = false, UpdateLayout = true };

class CSSComputedStyleDeclaration : public CSSStyleDeclaration {
public:
    friend PassRefPtr<CSSComputedStyleDeclaration> computedStyle(PassRefPtr<Node>);
    virtual ~CSSComputedStyleDeclaration();

    virtual String cssText() const;

    virtual unsigned length() const;
    virtual String item(unsigned index) const;

    virtual PassRefPtr<CSSValue> getPropertyCSSValue(int propertyID) const;
    virtual String getPropertyValue(int propertyID) const;
    virtual bool getPropertyPriority(int propertyID) const;
    virtual int getPropertyShorthand(int /*propertyID*/) const { return -1; }
    virtual bool isPropertyImplicit(int /*propertyID*/) const { return false; }

    virtual PassRefPtr<CSSMutableStyleDeclaration> copy() const;
    virtual PassRefPtr<CSSMutableStyleDeclaration> makeMutable();

    PassRefPtr<CSSValue> getPropertyCSSValue(int propertyID, EUpdateLayout) const;
    PassRefPtr<CSSValue> getFontSizeCSSValuePreferringKeyword() const;
#if ENABLE(SVG)
    PassRefPtr<CSSValue> getSVGPropertyCSSValue(int propertyID, EUpdateLayout) const;
#endif

protected:
    virtual bool cssPropertyMatches(const CSSProperty*) const;

private:
    CSSComputedStyleDeclaration(PassRefPtr<Node>);

    virtual void setCssText(const String&, ExceptionCode&);

    virtual String removeProperty(int propertyID, ExceptionCode&);
    virtual void setProperty(int propertyId, const String& value, bool important, ExceptionCode&);

    RefPtr<Node> m_node;
};

inline PassRefPtr<CSSComputedStyleDeclaration> computedStyle(PassRefPtr<Node> node)
{
    return adoptRef(new CSSComputedStyleDeclaration(node));
}

} // namespace WebCore

#endif // CSSComputedStyleDeclaration_h
