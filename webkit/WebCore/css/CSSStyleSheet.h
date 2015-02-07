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
 * Copyright (C) 2004, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef CSSStyleSheet_h
#define CSSStyleSheet_h

#include "CSSRuleList.h"
#include "StyleSheet.h"

namespace WebCore {

class CSSNamespace;
class CSSParser;
class CSSRule;
class DocLoader;
class Document;

typedef int ExceptionCode;

class CSSStyleSheet : public StyleSheet {
public:
    static PassRefPtr<CSSStyleSheet> create()
    {
        return adoptRef(new CSSStyleSheet(static_cast<CSSStyleSheet*>(0), String(), String()));
    }
    static PassRefPtr<CSSStyleSheet> create(Node* ownerNode)
    {
        return adoptRef(new CSSStyleSheet(ownerNode, String(), String()));
    }
    static PassRefPtr<CSSStyleSheet> create(Node* ownerNode, const String& href)
    {
        return adoptRef(new CSSStyleSheet(ownerNode, href, String()));
    }
    static PassRefPtr<CSSStyleSheet> create(Node* ownerNode, const String& href, const String& charset)
    {
        return adoptRef(new CSSStyleSheet(ownerNode, href, charset));
    }
    static PassRefPtr<CSSStyleSheet> create(CSSRule* ownerRule, const String& href, const String& charset)
    {
        return adoptRef(new CSSStyleSheet(ownerRule, href, charset));
    }

    virtual ~CSSStyleSheet();
    
    CSSRule* ownerRule() const;
    PassRefPtr<CSSRuleList> cssRules(bool omitCharsetRules = false);
    unsigned insertRule(const String& rule, unsigned index, ExceptionCode&);
    void deleteRule(unsigned index, ExceptionCode&);

    // IE Extensions
    PassRefPtr<CSSRuleList> rules() { return cssRules(true); }
    int addRule(const String& selector, const String& style, int index, ExceptionCode&);
    int addRule(const String& selector, const String& style, ExceptionCode&);
    void removeRule(unsigned index, ExceptionCode& ec) { deleteRule(index, ec); }

    void addNamespace(CSSParser*, const AtomicString& prefix, const AtomicString& uri);
    const AtomicString& determineNamespace(const AtomicString& prefix);
    
    virtual void styleSheetChanged();

    virtual bool parseString(const String&, bool strict = true);

    virtual bool isLoading();

    virtual void checkLoaded();

    Document* doc() { return m_doc; }

    const String& charset() const { return m_charset; }

    bool loadCompleted() const { return m_loadCompleted; }

    virtual KURL completeURL(const String& url) const;
    virtual void addSubresourceStyleURLs(ListHashSet<KURL>&);

    void setStrictParsing(bool b) { m_strictParsing = b; }
    bool useStrictParsing() const { return m_strictParsing; }

private:
    CSSStyleSheet(Node* ownerNode, const String& href, const String& charset);
    CSSStyleSheet(CSSStyleSheet* parentSheet, const String& href, const String& charset);
    CSSStyleSheet(CSSRule* ownerRule, const String& href, const String& charset);
    
    virtual bool isCSSStyleSheet() const { return true; }
    virtual String type() const { return "text/css"; }

    Document* m_doc;
    CSSNamespace* m_namespaces;
    String m_charset;
    bool m_loadCompleted : 1;
    bool m_strictParsing : 1;
};

} // namespace

#endif
