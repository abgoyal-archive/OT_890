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
 Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
 This file is part of the WebKit project
 
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

#ifndef SVGAnimateMotionElement_h
#define SVGAnimateMotionElement_h
#if ENABLE(SVG_ANIMATION)

#include "SVGAnimationElement.h"
#include "TransformationMatrix.h"
#include "Path.h"

namespace WebCore {
            
    class SVGAnimateMotionElement : public SVGAnimationElement {
    public:
        SVGAnimateMotionElement(const QualifiedName&, Document*);
        virtual ~SVGAnimateMotionElement();

        virtual bool hasValidTarget() const;

        virtual void parseMappedAttribute(MappedAttribute*);
        
    private:
        virtual void resetToBaseValue(const String&);
        virtual bool calculateFromAndToValues(const String& fromString, const String& toString);
        virtual bool calculateFromAndByValues(const String& fromString, const String& byString);
        virtual void calculateAnimatedValue(float percentage, unsigned repeat, SVGSMILElement* resultElement);
        virtual void applyResultsToTarget();
        virtual float calculateDistance(const String& fromString, const String& toString);
        virtual Path animationPath() const;
        
        enum RotateMode {
            RotateAngle,
            RotateAuto,
            RotateAutoReverse
        };
        RotateMode rotateMode() const;

        FloatSize m_animatedTranslation;
        float m_animatedAngle;

        // Note: we do not support percentage values for to/from coords as the spec implies we should (opera doesn't either)
        FloatPoint m_fromPoint;
        float m_fromAngle;
        FloatPoint m_toPoint;
        float m_toAngle;

        unsigned m_baseIndexInTransformList;

        Path m_path;
        Vector<float> m_keyPoints;
        float m_angle;
    };
    
} // namespace WebCore

#endif // ENABLE(SVG_ANIMATION)
#endif // SVGAnimateMotionElement_h

// vim:ts=4:noet
