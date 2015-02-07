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
 * This file is part of the html renderer for KDE.
 *
 * Copyright (C) 2005 Apple Computer
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

#ifndef RenderButton_h
#define RenderButton_h

#include "RenderFlexibleBox.h"
#include "Timer.h"
#include <wtf/OwnPtr.h>

namespace WebCore {

class RenderTextFragment;

// RenderButtons are just like normal flexboxes except that they will generate an anonymous block child.
// For inputs, they will also generate an anonymous RenderText and keep its style and content up
// to date as the button changes.
class RenderButton : public RenderFlexibleBox {
public:
    RenderButton(Node*);

    virtual const char* renderName() const { return "RenderButton"; }
    virtual bool isRenderButton() const { return true; }

    virtual void addChild(RenderObject* newChild, RenderObject *beforeChild = 0);
    virtual void removeChild(RenderObject*);
    virtual void removeLeftoverAnonymousBlock(RenderBlock*) { }
    virtual bool createsAnonymousWrapper() const { return true; }

    void setupInnerStyle(RenderStyle*);
    virtual void updateFromElement();

    virtual void updateBeforeAfterContent(PseudoId);

    virtual bool hasControlClip() const { return true; }
    virtual IntRect controlClipRect(int /*tx*/, int /*ty*/) const;

    void setText(const String&);
    String text() const;

    virtual bool canHaveChildren() const;

protected:
    virtual void styleWillChange(StyleDifference, const RenderStyle* newStyle);
    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle);

    virtual bool hasLineIfEmpty() const { return true; }

    void timerFired(Timer<RenderButton>*);

    RenderTextFragment* m_buttonText;
    RenderBlock* m_inner;

    OwnPtr<Timer<RenderButton> > m_timer;
    bool m_default;
};

inline RenderButton* toRenderButton(RenderObject* object)
{ 
    ASSERT(!object || object->isRenderButton());
    return static_cast<RenderButton*>(object);
}

inline const RenderButton* toRenderButton(const RenderObject* object)
{ 
    ASSERT(!object || object->isRenderButton());
    return static_cast<const RenderButton*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderButton(const RenderButton*);

} // namespace WebCore

#endif // RenderButton_h
