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
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
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

#ifndef HTMLTableElement_h
#define HTMLTableElement_h

#include "HTMLElement.h"

namespace WebCore {

class HTMLCollection;
class HTMLTableCaptionElement;
class HTMLTableSectionElement;

class HTMLTableElement : public HTMLElement {
public:
    HTMLTableElement(const QualifiedName&, Document*);

    virtual HTMLTagStatus endTagRequirement() const { return TagStatusRequired; }
    virtual int tagPriority() const { return 9; }
    virtual bool checkDTD(const Node*);

    HTMLTableCaptionElement* caption() const;
    void setCaption(PassRefPtr<HTMLTableCaptionElement>, ExceptionCode&);

    HTMLTableSectionElement* tHead() const;
    void setTHead(PassRefPtr<HTMLTableSectionElement>, ExceptionCode&);

    HTMLTableSectionElement* tFoot() const;
    void setTFoot(PassRefPtr<HTMLTableSectionElement>, ExceptionCode&);

    PassRefPtr<HTMLElement> createTHead();
    void deleteTHead();
    PassRefPtr<HTMLElement> createTFoot();
    void deleteTFoot();
    PassRefPtr<HTMLElement> createCaption();
    void deleteCaption();
    PassRefPtr<HTMLElement> insertRow(int index, ExceptionCode&);
    void deleteRow(int index, ExceptionCode&);

    PassRefPtr<HTMLCollection> rows();
    PassRefPtr<HTMLCollection> tBodies();

    String align() const;
    void setAlign(const String&);

    String bgColor() const;
    void setBgColor(const String&);

    String border() const;
    void setBorder(const String&);

    String cellPadding() const;
    void setCellPadding(const String&);

    String cellSpacing() const;
    void setCellSpacing(const String&);

    String frame() const;
    void setFrame(const String&);

    String rules() const;
    void setRules(const String&);

    String summary() const;
    void setSummary(const String&);

    String width() const;
    void setWidth(const String&);

    virtual ContainerNode* addChild(PassRefPtr<Node>);
    virtual bool mapToEntry(const QualifiedName&, MappedAttributeEntry&) const;
    virtual void parseMappedAttribute(MappedAttribute*);
    virtual void attach();
    virtual bool isURLAttribute(Attribute*) const;

    // Used to obtain either a solid or outset border decl and to deal with the frame
    // and rules attributes.
    virtual bool canHaveAdditionalAttributeStyleDecls() const { return true; }
    virtual void additionalAttributeStyleDecls(Vector<CSSMutableStyleDeclaration*>&);
    void addSharedCellDecls(Vector<CSSMutableStyleDeclaration*>&);
    void addSharedGroupDecls(bool rows, Vector<CSSMutableStyleDeclaration*>&);

    virtual void addSubresourceAttributeURLs(ListHashSet<KURL>&) const;

private:
    void addSharedCellBordersDecl(Vector<CSSMutableStyleDeclaration*>&);
    void addSharedCellPaddingDecl(Vector<CSSMutableStyleDeclaration*>&);
    
    enum TableRules { UnsetRules, NoneRules, GroupsRules, RowsRules, ColsRules, AllRules };
    enum CellBorders { NoBorders, SolidBorders, InsetBorders, SolidBordersColsOnly, SolidBordersRowsOnly };

    CellBorders cellBorders() const;

    HTMLTableSectionElement* lastBody() const;

    bool m_borderAttr;          // Sets a precise border width and creates an outset border for the table and for its cells.
    bool m_borderColorAttr;     // Overrides the outset border and makes it solid for the table and cells instead.
    bool m_frameAttr;           // Implies a thin border width if no border is set and then a certain set of solid/hidden borders based off the value.
    TableRules m_rulesAttr;     // Implies a thin border width, a collapsing border model, and all borders on the table becoming set to hidden (if frame/border
                                // are present, to none otherwise).
   
    unsigned short m_padding;
    RefPtr<CSSMappedAttributeDeclaration> m_paddingDecl;
};

} //namespace

#endif
