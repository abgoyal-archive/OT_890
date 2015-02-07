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
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2001 Tobias Anton (anton@stud.fbi.fh-darmstadt.de)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
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

#ifndef MouseRelatedEvent_h
#define MouseRelatedEvent_h

#include "IntPoint.h"
#include "UIEventWithKeyState.h"

namespace WebCore {

    // Internal only: Helper class for what's common between mouse and wheel events.
    class MouseRelatedEvent : public UIEventWithKeyState {
    public:
        // Note that these values are adjusted to counter the effects of zoom, so that values
        // exposed via DOM APIs are invariant under zooming.
        int screenX() const { return m_screenX; }
        int screenY() const { return m_screenY; }
        int clientX() const { return m_clientX; }
        int clientY() const { return m_clientY; }
        int layerX() const { return m_layerX; }
        int layerY() const { return m_layerY; }
        int offsetX() const { return m_offsetX; }
        int offsetY() const { return m_offsetY; }
        bool isSimulated() const { return m_isSimulated; }
        virtual int pageX() const;
        virtual int pageY() const;
        int x() const;
        int y() const;

        // Page point in "absolute" coordinates (i.e. post-zoomed, page-relative coords,
        // usable with RenderObject::absoluteToLocal).
        IntPoint absoluteLocation() const { return m_absoluteLocation; }
        void setAbsoluteLocation(const IntPoint& p) { m_absoluteLocation = p; }
    
    protected:
        MouseRelatedEvent();
        MouseRelatedEvent(const AtomicString& type, bool canBubble, bool cancelable, PassRefPtr<AbstractView>,
                          int detail, int screenX, int screenY, int pageX, int pageY,
                          bool ctrlKey, bool altKey, bool shiftKey, bool metaKey, bool isSimulated = false);

        void initCoordinates();
        void initCoordinates(int clientX, int clientY);
        virtual void receivedTarget();

        void computePageLocation();
        
        // Expose these so MouseEvent::initMouseEvent can set them.
        int m_screenX;
        int m_screenY;
        int m_clientX;
        int m_clientY;

    private:
        int m_pageX;
        int m_pageY;
        int m_layerX;
        int m_layerY;
        int m_offsetX;
        int m_offsetY;
        IntPoint m_absoluteLocation;
        bool m_isSimulated;
    };

} // namespace WebCore

#endif // MouseRelatedEvent_h
