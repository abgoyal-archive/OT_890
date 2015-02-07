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

#ifndef SVGColor_h
#define SVGColor_h
#if ENABLE(SVG)

#include "CSSValue.h"
#include "Color.h"
#include <wtf/PassRefPtr.h>

namespace WebCore {

    class RGBColor;

    class SVGColor : public CSSValue {
    public:
        static PassRefPtr<SVGColor> create(const String& color)
        {
            return adoptRef(new SVGColor(color));
        }
        static PassRefPtr<SVGColor> create(const Color& color)
        {
            return adoptRef(new SVGColor(color));
        }
        static PassRefPtr<SVGColor> createCurrentColor()
        {
            return adoptRef(new SVGColor(SVG_COLORTYPE_CURRENTCOLOR));
        }

        virtual ~SVGColor();

        enum SVGColorType {
            SVG_COLORTYPE_UNKNOWN                   = 0,
            SVG_COLORTYPE_RGBCOLOR                  = 1,
            SVG_COLORTYPE_RGBCOLOR_ICCCOLOR         = 2,
            SVG_COLORTYPE_CURRENTCOLOR              = 3
        };

        // 'SVGColor' functions
        unsigned short colorType() const;

        RGBColor* rgbColor() const;
        
        static Color colorFromRGBColorString(const String&);

        void setRGBColor(const String& rgbColor) { ExceptionCode ignored = 0; setRGBColor(rgbColor, ignored); }
        void setRGBColor(const String& rgbColor, ExceptionCode&);
        void setRGBColorICCColor(const String& rgbColor, const String& iccColor, ExceptionCode&);
        void setColor(unsigned short colorType, const String& rgbColor, const String& iccColor, ExceptionCode&);

        virtual String cssText() const;

        // Helpers
        const Color& color() const;

    protected:
        SVGColor();
        SVGColor(const String& color);
        SVGColor(const Color&);

    private:
        SVGColor(SVGColorType);

        static void create(int); // compile-time guard 

        virtual bool isSVGColor() const { return true; }

        Color m_color;
        unsigned short m_colorType;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // SVGColor_h

// vim:ts=4:noet
