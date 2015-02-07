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
    Copyright (C) 2004, 2005 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>
                  2005 Oliver Hunt <oliver@nerget.com>

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

#ifndef SVGFELightElement_h
#define SVGFELightElement_h

#if ENABLE(SVG) && ENABLE(FILTERS)
#include "SVGElement.h"
#include "SVGLightSource.h"

namespace WebCore {

    extern char SVGFELightElementIdentifier[];

    class SVGFELightElement : public SVGElement {
    public:
        SVGFELightElement(const QualifiedName&, Document*);
        virtual ~SVGFELightElement();
        
        virtual LightSource* lightSource() const = 0;
        virtual void parseMappedAttribute(MappedAttribute*);

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::azimuthAttrString, float, Azimuth, azimuth)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::elevationAttrString, float, Elevation, elevation)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::xAttrString, float, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::yAttrString, float, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::zAttrString, float, Z, z)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::pointsAtXAttrString, float, PointsAtX, pointsAtX)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::pointsAtYAttrString, float, PointsAtY, pointsAtY)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::pointsAtZAttrString, float, PointsAtZ, pointsAtZ)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::specularExponentAttrString, float, SpecularExponent, specularExponent)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFELightElement, SVGFELightElementIdentifier, SVGNames::limitingConeAngleAttrString, float, LimitingConeAngle, limitingConeAngle)
    };

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)
#endif
