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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AnimationControllerPrivate_h
#define AnimationControllerPrivate_h

#include "AtomicString.h"
#include "CSSPropertyNames.h"
#include "PlatformString.h"
#include "Timer.h"
#include <wtf/HashMap.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class AnimationBase;
class CompositeAnimation;
class Document;
class Element;
class Frame;
class Node;
class RenderObject;
class RenderStyle;

class AnimationControllerPrivate {
public:
    AnimationControllerPrivate(Frame*);
    ~AnimationControllerPrivate();

    PassRefPtr<CompositeAnimation> accessCompositeAnimation(RenderObject*);
    bool clear(RenderObject*);

    void animationTimerFired(Timer<AnimationControllerPrivate>*);
    void updateAnimationTimer(bool callSetChanged = false);

    void updateStyleIfNeededDispatcherFired(Timer<AnimationControllerPrivate>*);
    void startUpdateStyleIfNeededDispatcher();
    void addEventToDispatch(PassRefPtr<Element> element, const AtomicString& eventType, const String& name, double elapsedTime);
    void addNodeChangeToDispatch(PassRefPtr<Node>);

    bool hasAnimations() const { return !m_compositeAnimations.isEmpty(); }

    void suspendAnimations(Document*);
    void resumeAnimations(Document*);

    bool isAnimatingPropertyOnRenderer(RenderObject*, CSSPropertyID, bool isRunningNow) const;

    bool pauseAnimationAtTime(RenderObject*, const String& name, double t);
    bool pauseTransitionAtTime(RenderObject*, const String& property, double t);
    unsigned numberOfActiveAnimations() const;

    PassRefPtr<RenderStyle> getAnimatedStyleForRenderer(RenderObject* renderer);

    double beginAnimationUpdateTime();
    void setBeginAnimationUpdateTime(double t) { m_beginAnimationUpdateTime = t; }
    void endAnimationUpdate()
    {
        styleAvailable();
        if (!m_waitingForAResponse)
            startTimeResponse(beginAnimationUpdateTime());
    }
    
    void receivedStartTimeResponse(double t)
    {
        m_waitingForAResponse = false;
        startTimeResponse(t);
    }
    
    void addToStyleAvailableWaitList(AnimationBase*);
    void removeFromStyleAvailableWaitList(AnimationBase*);    
    
    void addToStartTimeResponseWaitList(AnimationBase*, bool willGetResponse);
    void removeFromStartTimeResponseWaitList(AnimationBase*);    
    void startTimeResponse(double t);
    
private:
    void styleAvailable();

    typedef HashMap<RenderObject*, RefPtr<CompositeAnimation> > RenderObjectAnimationMap;

    RenderObjectAnimationMap m_compositeAnimations;
    Timer<AnimationControllerPrivate> m_animationTimer;
    Timer<AnimationControllerPrivate> m_updateStyleIfNeededDispatcher;
    Frame* m_frame;
    
    class EventToDispatch {
    public:
        RefPtr<Element> element;
        AtomicString eventType;
        String name;
        double elapsedTime;
    };
    
    Vector<EventToDispatch> m_eventsToDispatch;
    Vector<RefPtr<Node> > m_nodeChangesToDispatch;
    
    double m_beginAnimationUpdateTime;
    AnimationBase* m_styleAvailableWaiters;
    AnimationBase* m_lastStyleAvailableWaiter;
    
    AnimationBase* m_responseWaiters;
    AnimationBase* m_lastResponseWaiter;
    bool m_waitingForAResponse;
};

} // namespace WebCore

#endif // AnimationControllerPrivate_h
