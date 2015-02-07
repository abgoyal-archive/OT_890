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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef ScrollbarTheme_h
#define ScrollbarTheme_h

#include "GraphicsContext.h"
#include "IntRect.h"
#include "ScrollTypes.h"

namespace WebCore {

class PlatformMouseEvent;
class Scrollbar;
class ScrollView;

class ScrollbarTheme {
public:
    virtual ~ScrollbarTheme() {};

    virtual bool paint(Scrollbar*, GraphicsContext*, const IntRect& /*damageRect*/) { return false; }
    virtual ScrollbarPart hitTest(Scrollbar*, const PlatformMouseEvent&) { return NoPart; }
    
    virtual int scrollbarThickness(ScrollbarControlSize = RegularScrollbar) { return 0; }

    virtual ScrollbarButtonsPlacement buttonsPlacement() const { return ScrollbarButtonsSingle; }

    virtual bool supportsControlTints() const { return false; }

    virtual void themeChanged() {}
    
    virtual bool invalidateOnMouseEnterExit() { return false; }

    void invalidateParts(Scrollbar* scrollbar, ScrollbarControlPartMask mask)
    {
        if (mask & BackButtonStartPart)
            invalidatePart(scrollbar, BackButtonStartPart);
        if (mask & ForwardButtonStartPart)
            invalidatePart(scrollbar, ForwardButtonStartPart);
        if (mask & BackTrackPart)
            invalidatePart(scrollbar, BackTrackPart);
        if (mask & ThumbPart)
            invalidatePart(scrollbar, ThumbPart);
        if (mask & ForwardTrackPart)
            invalidatePart(scrollbar, ForwardTrackPart);
        if (mask & BackButtonEndPart)
            invalidatePart(scrollbar, BackButtonEndPart);
        if (mask & ForwardButtonEndPart)
            invalidatePart(scrollbar, ForwardButtonEndPart);
    }

    virtual void invalidatePart(Scrollbar*, ScrollbarPart) {}

    virtual void paintScrollCorner(ScrollView*, GraphicsContext* context, const IntRect& cornerRect) { context->fillRect(cornerRect, Color::white); }

    virtual bool shouldCenterOnThumb(Scrollbar*, const PlatformMouseEvent&) { return false; }
    virtual bool shouldSnapBackToDragOrigin(Scrollbar*, const PlatformMouseEvent&) { return false; }
    virtual int thumbPosition(Scrollbar*) { return 0; } // The position of the thumb relative to the track.
    virtual int thumbLength(Scrollbar*) { return 0; } // The length of the thumb along the axis of the scrollbar.
    virtual int trackPosition(Scrollbar*) { return 0; } // The position of the track relative to the scrollbar.
    virtual int trackLength(Scrollbar*) { return 0; } // The length of the track along the axis of the scrollbar.
    
    virtual double initialAutoscrollTimerDelay() { return 0.25; }
    virtual double autoscrollTimerDelay() { return 0.05; }

    virtual void registerScrollbar(Scrollbar*) {}
    virtual void unregisterScrollbar(Scrollbar*) {}

    static ScrollbarTheme* nativeTheme(); // Must be implemented to return the correct theme subclass.
};

}
#endif
