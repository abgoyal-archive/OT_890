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
    Copyright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>

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

#ifndef PatternAttributes_h
#define PatternAttributes_h

#if ENABLE(SVG)

namespace WebCore {
    struct PatternAttributes {
        PatternAttributes()
            : m_x()
            , m_y()
            , m_width()
            , m_height()
            , m_boundingBoxMode(true)
            , m_boundingBoxModeContent(false)
            , m_patternContentElement(0)
            , m_xSet(false)
            , m_ySet(false)
            , m_widthSet(false)
            , m_heightSet(false)
            , m_boundingBoxModeSet(false)
            , m_boundingBoxModeContentSet(false)
            , m_patternTransformSet(false)
            , m_patternContentElementSet(false)
        {
        }

        SVGLength x() const { return m_x; }
        SVGLength y() const { return m_y; }
        SVGLength width() const { return m_width; }
        SVGLength height() const { return m_height; }
        bool boundingBoxMode() const { return m_boundingBoxMode; }
        bool boundingBoxModeContent() const { return m_boundingBoxModeContent; }
        TransformationMatrix patternTransform() const { return m_patternTransform; }
        const SVGPatternElement* patternContentElement() const { return m_patternContentElement; }

        void setX(const SVGLength& value) { m_x = value; m_xSet = true; }
        void setY(const SVGLength& value) { m_y = value; m_ySet = true; }
        void setWidth(const SVGLength& value) { m_width = value; m_widthSet = true; }
        void setHeight(const SVGLength& value) { m_height = value; m_heightSet = true; }
        void setBoundingBoxMode(bool value) { m_boundingBoxMode = value; m_boundingBoxModeSet = true; }
        void setBoundingBoxModeContent(bool value) { m_boundingBoxModeContent = value; m_boundingBoxModeContentSet = true; }
        void setPatternTransform(const TransformationMatrix& value) { m_patternTransform = value; m_patternTransformSet = true; }
        void setPatternContentElement(const SVGPatternElement* value) { m_patternContentElement = value; m_patternContentElementSet = true; }

        bool hasX() const { return m_xSet; }
        bool hasY() const { return m_ySet; }
        bool hasWidth() const { return m_widthSet; }
        bool hasHeight() const { return m_heightSet; }
        bool hasBoundingBoxMode() const { return m_boundingBoxModeSet; }
        bool hasBoundingBoxModeContent() const { return m_boundingBoxModeContentSet; }
        bool hasPatternTransform() const { return m_patternTransformSet; }
        bool hasPatternContentElement() const { return m_patternContentElementSet; }

    private:
        // Properties
        SVGLength m_x;
        SVGLength m_y;
        SVGLength m_width;
        SVGLength m_height;
        bool m_boundingBoxMode;
        bool m_boundingBoxModeContent;
        TransformationMatrix m_patternTransform;
        const SVGPatternElement* m_patternContentElement;

        // Property states
        bool m_xSet : 1;
        bool m_ySet : 1;
        bool m_widthSet : 1;
        bool m_heightSet : 1;
        bool m_boundingBoxModeSet : 1;
        bool m_boundingBoxModeContentSet : 1;
        bool m_patternTransformSet : 1;
        bool m_patternContentElementSet : 1;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
