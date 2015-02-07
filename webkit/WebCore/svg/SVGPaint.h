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
                  2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmial.com)

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

#ifndef SVGPaint_h
#define SVGPaint_h
#if ENABLE(SVG)

#include "SVGColor.h"
#include "PlatformString.h"

namespace WebCore {

    class SVGPaint : public SVGColor {
    public:
        enum SVGPaintType {
            SVG_PAINTTYPE_UNKNOWN               = 0,
            SVG_PAINTTYPE_RGBCOLOR              = 1,
            SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR     = 2,
            SVG_PAINTTYPE_NONE                  = 101,
            SVG_PAINTTYPE_CURRENTCOLOR          = 102,
            SVG_PAINTTYPE_URI_NONE              = 103,
            SVG_PAINTTYPE_URI_CURRENTCOLOR      = 104,
            SVG_PAINTTYPE_URI_RGBCOLOR          = 105,
            SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR = 106,
            SVG_PAINTTYPE_URI                   = 107
        };

        static PassRefPtr<SVGPaint> create()
        {
            return adoptRef(new SVGPaint);
        }
        static PassRefPtr<SVGPaint> create(SVGPaintType type)
        {
            return adoptRef(new SVGPaint(type));
        }
        static PassRefPtr<SVGPaint> create(const Color& color)
        {
            return adoptRef(new SVGPaint(color));
        }
        static PassRefPtr<SVGPaint> create(SVGPaintType type, const String& uri)
        {
            return adoptRef(new SVGPaint(type, uri, String(), String()));
        }
        static PassRefPtr<SVGPaint> create(const String& uri, const Color& color)
        {
            return adoptRef(new SVGPaint(uri, color));
        }

        virtual ~SVGPaint();

        // 'SVGPaint' functions
        SVGPaintType paintType() const { return m_paintType; }
        String uri() const;

        void setUri(const String&);
        void setPaint(SVGPaintType, const String& uri, const String& rgbPaint, const String& iccPaint, ExceptionCode&);

        virtual String cssText() const;
        
        static SVGPaint* defaultFill();
        static SVGPaint* defaultStroke();

    private:
        SVGPaint();
        SVGPaint(const String& uri);
        SVGPaint(SVGPaintType);
        SVGPaint(SVGPaintType, const String& uri, const String& rgbPaint, const String& iccPaint);
        SVGPaint(const Color& c);
        SVGPaint(const String& uri, const Color& c);

        virtual bool isSVGPaint() const { return true; }

        SVGPaintType m_paintType;
        String m_uri;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // SVGPaint_h

// vim:ts=4:noet
