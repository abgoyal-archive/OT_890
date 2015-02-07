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
 * This file is part of the HTML rendering engine for KDE.
 *
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
#ifndef HitTestResult_h
#define HitTestResult_h

#include "IntPoint.h"
#include "TextDirection.h"
#include <wtf/RefPtr.h>

namespace WebCore {

class Element;
class Frame;
class Image;
class IntRect;
class KURL;
class Node;
class Scrollbar;
class String;

class HitTestResult {
public:
    HitTestResult(const IntPoint&);
    HitTestResult(const HitTestResult&);
    ~HitTestResult();
    HitTestResult& operator=(const HitTestResult&);

    Node* innerNode() const { return m_innerNode.get(); }
    Node* innerNonSharedNode() const { return m_innerNonSharedNode.get(); }
    IntPoint point() const { return m_point; }
    IntPoint localPoint() const { return m_localPoint; }
    Element* URLElement() const { return m_innerURLElement.get(); }
    Scrollbar* scrollbar() const { return m_scrollbar.get(); }
    bool isOverWidget() const { return m_isOverWidget; }

    void setToNonShadowAncestor();

    void setInnerNode(Node*);
    void setInnerNonSharedNode(Node*);
    void setPoint(const IntPoint& p) { m_point = p; }
    void setLocalPoint(const IntPoint& p) { m_localPoint = p; }
    void setURLElement(Element*);
    void setScrollbar(Scrollbar*);
    void setIsOverWidget(bool b) { m_isOverWidget = b; }

    Frame* targetFrame() const;
    IntRect boundingBox() const;
    bool isSelected() const;
    String spellingToolTip(TextDirection&) const;
    String replacedString() const;
    String title(TextDirection&) const;
    String altDisplayString() const;
    String titleDisplayString() const;
    Image* image() const;
    IntRect imageRect() const;
    KURL absoluteImageURL() const;
    KURL absoluteMediaURL() const;
    KURL absoluteLinkURL() const;
    String textContent() const;
    bool isLiveLink() const;
    bool isContentEditable() const;

private:
    RefPtr<Node> m_innerNode;
    RefPtr<Node> m_innerNonSharedNode;
    IntPoint m_point;
    IntPoint m_localPoint; // A point in the local coordinate space of m_innerNonSharedNode's renderer.  Allows us to efficiently
                           // determine where inside the renderer we hit on subsequent operations.
    RefPtr<Element> m_innerURLElement;
    RefPtr<Scrollbar> m_scrollbar;
    bool m_isOverWidget; // Returns true if we are over a widget (and not in the border/padding area of a RenderWidget for example).
};

String displayString(const String&, const Node*);

} // namespace WebCore

#endif // HitTestResult_h
