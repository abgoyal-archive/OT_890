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
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
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

#ifndef Animation_h
#define Animation_h

#include "PlatformString.h"
#include "TimingFunction.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

namespace WebCore {

const int cAnimateNone = 0;
const int cAnimateAll = -2;

class Animation : public RefCounted<Animation> {
public:
    ~Animation();

    static PassRefPtr<Animation> create() { return adoptRef(new Animation); };
    
    bool isDelaySet() const { return m_delaySet; }
    bool isDirectionSet() const { return m_directionSet; }
    bool isDurationSet() const { return m_durationSet; }
    bool isIterationCountSet() const { return m_iterationCountSet; }
    bool isNameSet() const { return m_nameSet; }
    bool isPlayStateSet() const { return m_playStateSet; }
    bool isPropertySet() const { return m_propertySet; }
    bool isTimingFunctionSet() const { return m_timingFunctionSet; }

    // Flags this to be the special "none" animation (animation-name: none)
    bool isNoneAnimation() const { return m_isNone; }
    // We can make placeholder Animation objects to keep the comma-separated lists
    // of properties in sync. isValidAnimation means this is not a placeholder.
    bool isValidAnimation() const { return !m_isNone && !m_name.isEmpty(); }

    bool isEmpty() const
    {
        return (!m_directionSet && !m_durationSet && !m_nameSet && !m_playStateSet && 
               !m_iterationCountSet && !m_delaySet && !m_timingFunctionSet && !m_propertySet);
    }

    bool isEmptyOrZeroDuration() const
    {
        return isEmpty() || (m_duration == 0 && m_delay <= 0);
    }

    void clearDelay() { m_delaySet = false; }
    void clearDirection() { m_directionSet = false; }
    void clearDuration() { m_durationSet = false; }
    void clearIterationCount() { m_iterationCountSet = false; }
    void clearName() { m_nameSet = false; }
    void clearPlayState() { m_playStateSet = AnimPlayStatePlaying; }
    void clearProperty() { m_propertySet = false; }
    void clearTimingFunction() { m_timingFunctionSet = false; }

    double delay() const { return m_delay; }

    enum AnimationDirection { AnimationDirectionNormal, AnimationDirectionAlternate };
    AnimationDirection direction() const { return m_direction; }

    double duration() const { return m_duration; }

    enum { IterationCountInfinite = -1 };
    int iterationCount() const { return m_iterationCount; }
    const String& name() const { return m_name; }
    unsigned playState() const { return m_playState; }
    int property() const { return m_property; }
    const TimingFunction& timingFunction() const { return m_timingFunction; }

    void setDelay(double c) { m_delay = c; m_delaySet = true; }
    void setDirection(AnimationDirection d) { m_direction = d; m_directionSet = true; }
    void setDuration(double d) { ASSERT(d >= 0); m_duration = d; m_durationSet = true; }
    void setIterationCount(int c) { m_iterationCount = c; m_iterationCountSet = true; }
    void setName(const String& n) { m_name = n; m_nameSet = true; }
    void setPlayState(unsigned d) { m_playState = d; m_playStateSet = true; }
    void setProperty(int t) { m_property = t; m_propertySet = true; }
    void setTimingFunction(const TimingFunction& f) { m_timingFunction = f; m_timingFunctionSet = true; }

    void setIsNoneAnimation(bool n) { m_isNone = n; }

    Animation& operator=(const Animation& o);

    // return true if all members of this class match (excluding m_next)
    bool animationsMatch(const Animation*, bool matchPlayStates = true) const;

    // return true every Animation in the chain (defined by m_next) match 
    bool operator==(const Animation& o) const { return animationsMatch(&o); }
    bool operator!=(const Animation& o) const { return !(*this == o); }

private:
    Animation();
    Animation(const Animation& o);
    
    String m_name;
    int m_property;
    int m_iterationCount;
    double m_delay;
    double m_duration;
    TimingFunction m_timingFunction;
    AnimationDirection m_direction : 1;

    unsigned m_playState     : 2;

    bool m_delaySet          : 1;
    bool m_directionSet      : 1;
    bool m_durationSet       : 1;
    bool m_iterationCountSet : 1;
    bool m_nameSet           : 1;
    bool m_playStateSet      : 1;
    bool m_propertySet       : 1;
    bool m_timingFunctionSet : 1;
    
    bool m_isNone            : 1;

public:
    static float initialAnimationDelay() { return 0; }
    static AnimationDirection initialAnimationDirection() { return AnimationDirectionNormal; }
    static double initialAnimationDuration() { return 0; }
    static int initialAnimationIterationCount() { return 1; }
    static String initialAnimationName() { return String("none"); }
    static unsigned initialAnimationPlayState() { return AnimPlayStatePlaying; }
    static int initialAnimationProperty() { return cAnimateAll; }
    static TimingFunction initialAnimationTimingFunction() { return TimingFunction(); }
};

} // namespace WebCore

#endif // Animation_h
