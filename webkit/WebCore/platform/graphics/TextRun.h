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
 * Copyright (C) 2003, 2006, 2007 Apple Computer, Inc.
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

#ifndef TextRun_h
#define TextRun_h

#include "PlatformString.h"

namespace WebCore {

class RenderObject;
class SVGPaintServer;

class TextRun {
public:
    TextRun(const UChar* c, int len, bool allowTabs = false, int xpos = 0, int padding = 0, bool rtl = false, bool directionalOverride = false,
              bool applyRunRounding = true, bool applyWordRounding = true)
        : m_characters(c)
        , m_len(len)
        , m_xpos(xpos)
        , m_padding(padding)
        , m_allowTabs(allowTabs)
        , m_rtl(rtl)
        , m_directionalOverride(directionalOverride)
        , m_applyRunRounding(applyRunRounding)
        , m_applyWordRounding(applyWordRounding)
        , m_disableSpacing(false)
#if ENABLE(SVG_FONTS)
        , m_referencingRenderObject(0)
        , m_activePaintServer(0)
#endif
    {
    }

    TextRun(const String& s, bool allowTabs = false, int xpos = 0, int padding = 0, bool rtl = false, bool directionalOverride = false,
              bool applyRunRounding = true, bool applyWordRounding = true)
        : m_characters(s.characters())
        , m_len(s.length())
        , m_xpos(xpos)
        , m_padding(padding)
        , m_allowTabs(allowTabs)
        , m_rtl(rtl)
        , m_directionalOverride(directionalOverride)
        , m_applyRunRounding(applyRunRounding)
        , m_applyWordRounding(applyWordRounding)
        , m_disableSpacing(false)
#if ENABLE(SVG_FONTS)
        , m_referencingRenderObject(0)
        , m_activePaintServer(0)
#endif
    {
    }

    UChar operator[](int i) const { return m_characters[i]; }
    const UChar* data(int i) const { return &m_characters[i]; }

    const UChar* characters() const { return m_characters; }
    int length() const { return m_len; }

    void setText(const UChar* c, int len) { m_characters = c; m_len = len; }

    bool allowTabs() const { return m_allowTabs; }
    int xPos() const { return m_xpos; }
    int padding() const { return m_padding; }
    bool rtl() const { return m_rtl; }
    bool ltr() const { return !m_rtl; }
    bool directionalOverride() const { return m_directionalOverride; }
    bool applyRunRounding() const { return m_applyRunRounding; }
    bool applyWordRounding() const { return m_applyWordRounding; }
    bool spacingDisabled() const { return m_disableSpacing; }

    void disableSpacing() { m_disableSpacing = true; }
    void disableRoundingHacks() { m_applyRunRounding = m_applyWordRounding = false; }
    void setRTL(bool b) { m_rtl = b; }
    void setDirectionalOverride(bool override) { m_directionalOverride = override; }

#if ENABLE(SVG_FONTS)
    RenderObject* referencingRenderObject() const { return m_referencingRenderObject; }
    void setReferencingRenderObject(RenderObject* object) { m_referencingRenderObject = object; }

    SVGPaintServer* activePaintServer() const { return m_activePaintServer; }
    void setActivePaintServer(SVGPaintServer* object) { m_activePaintServer = object; }
#endif

private:
    const UChar* m_characters;
    int m_len;

    int m_xpos;
    int m_padding;
    bool m_allowTabs;
    bool m_rtl;
    bool m_directionalOverride;
    bool m_applyRunRounding;
    bool m_applyWordRounding;
    bool m_disableSpacing;

#if ENABLE(SVG_FONTS)
    RenderObject* m_referencingRenderObject;
    SVGPaintServer* m_activePaintServer;
#endif
};

}

#endif
