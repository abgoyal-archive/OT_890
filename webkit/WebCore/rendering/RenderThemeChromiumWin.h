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
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2008, 2009 Google, Inc.
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

#ifndef RenderThemeChromiumWin_h
#define RenderThemeChromiumWin_h

#include "RenderThemeChromiumSkia.h"

#if WIN32
typedef void* HANDLE;
typedef struct HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
#endif

namespace WebCore {

    struct ThemeData {
        ThemeData() : m_part(0), m_state(0), m_classicState(0) {}

        unsigned m_part;
        unsigned m_state;
        unsigned m_classicState;
    };

    class RenderThemeChromiumWin : public RenderThemeChromiumSkia {
    public:
        static PassRefPtr<RenderTheme> create();

        // A method asking if the theme is able to draw the focus ring.
        virtual bool supportsFocusRing(const RenderStyle*) const;

        // The platform selection color.
        virtual Color platformActiveSelectionBackgroundColor() const;
        virtual Color platformInactiveSelectionBackgroundColor() const;
        virtual Color platformActiveSelectionForegroundColor() const;
        virtual Color platformInactiveSelectionForegroundColor() const;
        virtual Color platformActiveTextSearchHighlightColor() const;
        virtual Color platformInactiveTextSearchHighlightColor() const;

        // System fonts.
        virtual void systemFont(int propId, FontDescription&) const;

        virtual void adjustSliderThumbSize(RenderObject*) const;

        // Various paint functions.
        virtual bool paintCheckbox(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
        virtual bool paintRadio(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
        virtual bool paintButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
        virtual bool paintTextField(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
        virtual bool paintSliderTrack(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
        virtual bool paintSliderThumb(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

        // MenuList refers to an unstyled menulist (meaning a menulist without
        // background-color or border set) and MenuListButton refers to a styled
        // menulist (a menulist with background-color or border set). They have
        // this distinction to support showing aqua style themes whenever they
        // possibly can, which is something we don't want to replicate.
        //
        // In short, we either go down the MenuList code path or the MenuListButton
        // codepath. We never go down both. And in both cases, they render the
        // entire menulist.
        virtual bool paintMenuList(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

        // Override RenderThemeChromiumSkia's setDefaultFontSize method to also reset the local font property caches.
        // See comment in RenderThemeChromiumSkia::setDefaultFontSize() regarding ugliness of this hack.
        static void setDefaultFontSize(int);

    protected:
        virtual double caretBlinkIntervalInternal() const;

    private:
        RenderThemeChromiumWin() { }
        virtual ~RenderThemeChromiumWin() { }

        unsigned determineState(RenderObject*);
        unsigned determineSliderThumbState(RenderObject*);
        unsigned determineClassicState(RenderObject*);

        ThemeData getThemeData(RenderObject*);

        bool paintTextFieldInternal(RenderObject*, const RenderObject::PaintInfo&, const IntRect&, bool);
    };

} // namespace WebCore

#endif
