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
 */

#ifndef FrameTree_h
#define FrameTree_h

#include "AtomicString.h"

namespace WebCore {

    class Frame;

    class FrameTree : public Noncopyable {
    public:
        FrameTree(Frame* thisFrame, Frame* parentFrame) 
            : m_thisFrame(thisFrame)
            , m_parent(parentFrame)
            , m_previousSibling(0)
            , m_lastChild(0)
            , m_childCount(0)
        {
        }
        ~FrameTree();

        const AtomicString& name() const { return m_name; }
        void setName(const AtomicString&);
        void clearName();
        Frame* parent(bool checkForDisconnectedFrame = false) const;
        void setParent(Frame* parent) { m_parent = parent; }
        
        Frame* nextSibling() const { return m_nextSibling.get(); }
        Frame* previousSibling() const { return m_previousSibling; }
        Frame* firstChild() const { return m_firstChild.get(); }
        Frame* lastChild() const { return m_lastChild; }
        unsigned childCount() const { return m_childCount; }

        bool isDescendantOf(const Frame* ancestor) const;
        Frame* traverseNext(const Frame* stayWithin = 0) const;
        Frame* traverseNextWithWrap(bool) const;
        Frame* traversePreviousWithWrap(bool) const;
        
        void appendChild(PassRefPtr<Frame>);
        void detachFromParent() { m_parent = 0; }
        void removeChild(Frame*);

        Frame* child(unsigned index) const;
        Frame* child(const AtomicString& name) const;
        Frame* find(const AtomicString& name) const;

        AtomicString uniqueChildName(const AtomicString& requestedName) const;

        Frame* top(bool checkForDisconnectedFrame = false) const;

    private:
        Frame* deepLastChild() const;

        Frame* m_thisFrame;

        Frame* m_parent;
        AtomicString m_name;

        // FIXME: use ListRefPtr?
        RefPtr<Frame> m_nextSibling;
        Frame* m_previousSibling;
        RefPtr<Frame> m_firstChild;
        Frame* m_lastChild;
        unsigned m_childCount;
    };

} // namespace WebCore

#endif // FrameTree_h
