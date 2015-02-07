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
    Copyright (C) 2004, 2005, 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006, 2008 Rob Buis <buis@kde.org>

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

#ifndef SVGPathSeg_h
#define SVGPathSeg_h

#if ENABLE(SVG)
#include "PlatformString.h"
#include "SVGNames.h"

#include <wtf/RefCounted.h>

namespace WebCore {
    class SVGPathElement;
    class SVGStyledElement;

    class SVGPathSeg : public RefCounted<SVGPathSeg> {
    public:
        virtual ~SVGPathSeg() { }

        enum SVGPathSegType {
            PATHSEG_UNKNOWN                         = 0,
            PATHSEG_CLOSEPATH                       = 1,
            PATHSEG_MOVETO_ABS                      = 2,
            PATHSEG_MOVETO_REL                      = 3,
            PATHSEG_LINETO_ABS                      = 4,
            PATHSEG_LINETO_REL                      = 5,
            PATHSEG_CURVETO_CUBIC_ABS               = 6,
            PATHSEG_CURVETO_CUBIC_REL               = 7,
            PATHSEG_CURVETO_QUADRATIC_ABS           = 8,
            PATHSEG_CURVETO_QUADRATIC_REL           = 9,
            PATHSEG_ARC_ABS                         = 10,
            PATHSEG_ARC_REL                         = 11,
            PATHSEG_LINETO_HORIZONTAL_ABS           = 12,
            PATHSEG_LINETO_HORIZONTAL_REL           = 13,
            PATHSEG_LINETO_VERTICAL_ABS             = 14,
            PATHSEG_LINETO_VERTICAL_REL             = 15,
            PATHSEG_CURVETO_CUBIC_SMOOTH_ABS        = 16,
            PATHSEG_CURVETO_CUBIC_SMOOTH_REL        = 17,
            PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS    = 18,
            PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL    = 19
        };

        virtual unsigned short pathSegType() const { return PATHSEG_UNKNOWN; }
        virtual String pathSegTypeAsLetter() const { return ""; }
        virtual String toString() const { return ""; }

        const QualifiedName& associatedAttributeName() const { return SVGNames::dAttr; }
        
    protected:
        SVGPathSeg() { }
    };

    class SVGPathSegSingleCoord : public SVGPathSeg { 
    public:
        SVGPathSegSingleCoord(float x, float y)
        : SVGPathSeg() , m_x(x) , m_y(y) {}

        void setX(float x) { m_x = x; }
        float x() const { return m_x; }

        void setY(float y) { m_y = y; }
        float y() const { return m_y; }

        virtual String toString() const { return pathSegTypeAsLetter() + String::format(" %.6lg %.6lg", m_x, m_y); }

    private:
        float m_x;
        float m_y;
    };


} // namespace WebCore

#endif // ENABLE(SVG)
#endif
