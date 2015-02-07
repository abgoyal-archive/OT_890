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
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2008 Apple Inc. All rights reserved.
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
 */

#ifndef MediaList_h
#define MediaList_h

#include "StyleBase.h"
#include <wtf/PassRefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class CSSImportRule;
class CSSStyleSheet;
class MediaQuery;
class String;

typedef int ExceptionCode;

class MediaList : public StyleBase {
public:
    static PassRefPtr<MediaList> create()
    {
        return adoptRef(new MediaList(0, false));
    }
    static PassRefPtr<MediaList> create(CSSImportRule* parentRule, const String& media)
    {
        return adoptRef(new MediaList(parentRule, media));
    }
    static PassRefPtr<MediaList> create(CSSStyleSheet* parentSheet, const String& media)
    {
        return adoptRef(new MediaList(parentSheet, media, false));
    }

    static PassRefPtr<MediaList> createAllowingDescriptionSyntax(const String& mediaQueryOrDescription)
    {
        return adoptRef(new MediaList(0, mediaQueryOrDescription, true));
    }
    static PassRefPtr<MediaList> createAllowingDescriptionSyntax(CSSStyleSheet* parentSheet, const String& mediaQueryOrDescription)
    {
        return adoptRef(new MediaList(parentSheet, mediaQueryOrDescription, true));
    }

    static PassRefPtr<MediaList> create(const String& media, bool allowDescriptionSyntax)
    {
        return adoptRef(new MediaList(0, media, allowDescriptionSyntax));
    }

    virtual ~MediaList();

    unsigned length() const { return m_queries.size(); }
    String item(unsigned index) const;
    void deleteMedium(const String& oldMedium, ExceptionCode&);
    void appendMedium(const String& newMedium, ExceptionCode&);

    String mediaText() const;
    void setMediaText(const String&, ExceptionCode&xo);

    void appendMediaQuery(MediaQuery*);
    const Vector<MediaQuery*>& mediaQueries() const { return m_queries; }

private:
    MediaList(CSSStyleSheet* parentSheet, bool fallbackToDescription);
    MediaList(CSSStyleSheet* parentSheet, const String& media, bool fallbackToDescription);
    MediaList(CSSImportRule* parentRule, const String& media);

    void notifyChanged();

    Vector<MediaQuery*> m_queries;
    bool m_fallback; // true if failed media query parsing should fallback to media description parsing
};

} // namespace

#endif
