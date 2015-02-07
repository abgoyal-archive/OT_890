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
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 2000 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006 Apple Computer, Inc.
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

#ifndef ProcessingInstruction_h
#define ProcessingInstruction_h

#include "CachedResourceClient.h"
#include "CachedResourceHandle.h"
#include "ContainerNode.h"

namespace WebCore {

class StyleSheet;
class CSSStyleSheet;

class ProcessingInstruction : public ContainerNode, private CachedResourceClient {
public:
    ProcessingInstruction(Document*);
    ProcessingInstruction(Document*, const String& target, const String& data);
    virtual ~ProcessingInstruction();

    // DOM methods & attributes for Notation
    String target() const { return m_target; }
    String data() const { return m_data; }
    void setData(const String&, ExceptionCode&);

    virtual String nodeName() const;
    virtual NodeType nodeType() const;
    virtual String nodeValue() const;
    virtual void setNodeValue(const String&, ExceptionCode&);
    virtual PassRefPtr<Node> cloneNode(bool deep);
    virtual bool childTypeAllowed(NodeType);
    virtual bool offsetInCharacters() const;
    virtual int maxCharacterOffset() const;

    virtual void insertedIntoDocument();
    virtual void removedFromDocument();
    void setCreatedByParser(bool createdByParser) { m_createdByParser = createdByParser; }
    virtual void finishParsingChildren();

    // Other methods (not part of DOM)
    String localHref() const { return m_localHref; }
    StyleSheet* sheet() const { return m_sheet.get(); }
    void checkStyleSheet();
    virtual void setCSSStyleSheet(const String& url, const String& charset, const CachedCSSStyleSheet*);
#if ENABLE(XSLT)
    virtual void setXSLStyleSheet(const String& url, const String& sheet);
#endif
    void setCSSStyleSheet(PassRefPtr<CSSStyleSheet>);
    bool isLoading() const;
    virtual bool sheetLoaded();

#if ENABLE(XSLT)
    bool isXSL() const { return m_isXSL; }
#endif

    virtual void addSubresourceAttributeURLs(ListHashSet<KURL>&) const;

private:
    void parseStyleSheet(const String& sheet);

    String m_target;
    String m_data;
    String m_localHref;
    String m_title;
    String m_media;
    CachedResourceHandle<CachedResource> m_cachedSheet;
    RefPtr<StyleSheet> m_sheet;
    bool m_loading;
    bool m_alternate;
    bool m_createdByParser;
#if ENABLE(XSLT)
    bool m_isXSL;
#endif
};

} //namespace

#endif
