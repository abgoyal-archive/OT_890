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
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2001 Tobias Anton (anton@stud.fbi.fh-darmstadt.de)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
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

#ifndef WheelEvent_h
#define WheelEvent_h

#include "MouseRelatedEvent.h"

namespace WebCore {

    // extension: mouse wheel event
    class WheelEvent : public MouseRelatedEvent {
    public:
        static PassRefPtr<WheelEvent> create()
        {
            return adoptRef(new WheelEvent);
        }
        static PassRefPtr<WheelEvent> create(float wheelTicksX, float wheelTicksY, PassRefPtr<AbstractView> view,
            int screenX, int screenY, int pageX, int pageY,
            bool ctrlKey, bool altKey, bool shiftKey, bool metaKey)
        {
            return adoptRef(new WheelEvent(wheelTicksX, wheelTicksY, view, screenX, screenY, pageX, pageY,
                ctrlKey, altKey, shiftKey, metaKey));
        }

        void initWheelEvent(int wheelDeltaX, int wheelDeltaY, PassRefPtr<AbstractView>,
                            int screenX, int screenY, int pageX, int pageY,
                            bool ctrlKey, bool altKey, bool shiftKey, bool metaKey);

        int wheelDelta() const { if (m_wheelDeltaY == 0) return m_wheelDeltaX; return m_wheelDeltaY; }
        int wheelDeltaX() const { return m_wheelDeltaX; }
        int wheelDeltaY() const { return m_wheelDeltaY; }

        // Needed for Objective-C legacy support
        bool isHorizontal() const { return m_wheelDeltaX; }

    private:
        WheelEvent();
        WheelEvent(float wheelTicksX, float wheelTicksY, PassRefPtr<AbstractView>,
                   int screenX, int screenY, int pageX, int pageY,
                   bool ctrlKey, bool altKey, bool shiftKey, bool metaKey);

        virtual bool isWheelEvent() const;

        int m_wheelDeltaX;
        int m_wheelDeltaY;
    };

} // namespace WebCore

#endif // WheelEvent_h
