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
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef NamedAttrMap_h
#define NamedAttrMap_h

#include "Attribute.h"

#ifdef __OBJC__
#define id id_AVOID_KEYWORD
#endif

namespace WebCore {

class Node;

typedef int ExceptionCode;

class NamedNodeMap : public RefCounted<NamedNodeMap> {
    friend class Element;

protected:
    NamedNodeMap(Element* element) : m_element(element) { }

public:
    static PassRefPtr<NamedNodeMap> create(Element* element) { return adoptRef(new NamedNodeMap(element)); }

    virtual ~NamedNodeMap();

    // Public DOM interface.

    PassRefPtr<Node> getNamedItem(const String& name) const;
    PassRefPtr<Node> removeNamedItem(const String& name, ExceptionCode&);

    PassRefPtr<Node> getNamedItemNS(const String& namespaceURI, const String& localName) const;
    PassRefPtr<Node> removeNamedItemNS(const String& namespaceURI, const String& localName, ExceptionCode&);

    PassRefPtr<Node> getNamedItem(const QualifiedName& name) const;
    PassRefPtr<Node> removeNamedItem(const QualifiedName& name, ExceptionCode&);
    PassRefPtr<Node> setNamedItem(Node*, ExceptionCode&);
    PassRefPtr<Node> setNamedItemNS(Node* node, ExceptionCode& ec) { return setNamedItem(node, ec); }

    PassRefPtr<Node> item(unsigned index) const;
    size_t length() const { return m_attributes.size(); }
    bool isEmpty() const { return !length(); }

    // Internal interface.

    void setAttributes(const NamedNodeMap&);

    Attribute* attributeItem(unsigned index) const { return m_attributes[index].get(); }
    Attribute* getAttributeItem(const QualifiedName&) const;

    void shrinkToLength() { m_attributes.shrinkCapacity(length()); }
    void reserveInitialCapacity(unsigned capacity) { m_attributes.reserveInitialCapacity(capacity); }

    // Used during parsing: only inserts if not already there. No error checking!
    void insertAttribute(PassRefPtr<Attribute> newAttribute, bool allowDuplicates)
    {
        ASSERT(!m_element);
        if (allowDuplicates || !getAttributeItem(newAttribute->name()))
            addAttribute(newAttribute);
    }

    virtual bool isMappedAttributeMap() const;

    const AtomicString& id() const { return m_id; }
    void setID(const AtomicString& newId) { m_id = newId; }

    bool mapsEquivalent(const NamedNodeMap* otherMap) const;

    // These functions do no error checking.
    void addAttribute(PassRefPtr<Attribute>);
    void removeAttribute(const QualifiedName&);

protected:
    virtual void clearAttributes();

    Element* element() const { return m_element; }

private:
    void detachAttributesFromElement();
    void detachFromElement();
    Attribute* getAttributeItem(const String& name, bool shouldIgnoreAttributeCase) const;

    Element* m_element;
    Vector<RefPtr<Attribute> > m_attributes;
    AtomicString m_id;
};

} //namespace

#undef id

#endif
