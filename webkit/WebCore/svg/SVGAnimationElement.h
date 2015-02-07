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
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>
    Copyright (C) 2007 Eric Seidel <eric@webkit.org>
    Copyright (C) 2008 Apple Inc. All rights reserved.

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SVGAnimationElement_h
#define SVGAnimationElement_h
#if ENABLE(SVG_ANIMATION)

#include "ElementTimeControl.h"
#include "Path.h"
#include "SMILTime.h"
#include "SVGSMILElement.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGStringList.h"
#include "SVGTests.h"
#include "UnitBezier.h"

namespace WebCore {
    
    class ConditionEventListener;
    class TimeContainer;

    class SVGAnimationElement : public SVGSMILElement,
                                public SVGTests,
                                public SVGExternalResourcesRequired,
                                public ElementTimeControl {
    public:
        SVGAnimationElement(const QualifiedName&, Document*);
        virtual ~SVGAnimationElement();
        
        virtual void parseMappedAttribute(MappedAttribute*);
        virtual void attributeChanged(Attribute*, bool preserveDecls);

        // SVGAnimationElement
        float getStartTime() const;
        float getCurrentTime() const;
        float getSimpleDuration(ExceptionCode&) const;
        
        // ElementTimeControl
        virtual bool beginElement(ExceptionCode&);
        virtual bool beginElementAt(float offset, ExceptionCode&);
        virtual bool endElement(ExceptionCode&);
        virtual bool endElementAt(float offset, ExceptionCode&);
        
        static bool attributeIsCSS(const String& attributeName);

    protected:
        virtual const SVGElement* contextElement() const { return this; }
 
        enum CalcMode { CalcModeDiscrete, CalcModeLinear, CalcModePaced, CalcModeSpline };
        CalcMode calcMode() const;
        
        enum AttributeType { AttributeTypeCSS, AttributeTypeXML, AttributeTypeAuto };
        AttributeType attributeType() const;
        
        String toValue() const;
        String byValue() const;
        String fromValue() const;
        
        enum AnimationMode { NoAnimation, ToAnimation, ByAnimation, ValuesAnimation, FromToAnimation, FromByAnimation, PathAnimation };
        AnimationMode animationMode() const;

        virtual bool hasValidTarget() const;
        
        String targetAttributeBaseValue() const;
        void setTargetAttributeAnimatedValue(const String&);
        bool targetAttributeIsCSS() const;
        
        bool isAdditive() const;
        bool isAccumulated() const;
    
        // from SVGSMILElement
        virtual void startedActiveInterval();
        virtual void updateAnimation(float percent, unsigned repeat, SVGSMILElement* resultElement);
        virtual void endedActiveInterval();
        
    private:
        virtual bool calculateFromAndToValues(const String& fromString, const String& toString) = 0;
        virtual bool calculateFromAndByValues(const String& fromString, const String& byString) = 0;
        virtual void calculateAnimatedValue(float percentage, unsigned repeat, SVGSMILElement* resultElement) = 0;
        virtual float calculateDistance(const String& /*fromString*/, const String& /*toString*/) { return -1.f; }
        virtual Path animationPath() const { return Path(); }
        
        void currentValuesForValuesAnimation(float percent, float& effectivePercent, String& from, String& to) const;
        void calculateKeyTimesForCalcModePaced();
        float calculatePercentFromKeyPoints(float percent) const;
        void currentValuesFromKeyPoints(float percent, float& effectivePercent, String& from, String& to) const;
        float calculatePercentForSpline(float percent, unsigned splineIndex) const;
        
    protected:
        bool m_animationValid;

        Vector<String> m_values;
        Vector<float> m_keyTimes;
        Vector<float> m_keyPoints;
        Vector<UnitBezier> m_keySplines;
        String m_lastValuesAnimationFrom;
        String m_lastValuesAnimationTo;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // SVGAnimationElement_h
