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
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2009 Kenneth Rohde Christiansen
 * All rights reserved.
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

#ifndef RenderThemeGdk_h
#define RenderThemeGdk_h

#include "RenderTheme.h"

#include <gtk/gtk.h>

namespace WebCore {

class RenderThemeGtk : public RenderTheme {
private:
    RenderThemeGtk();
    virtual ~RenderThemeGtk();

public:
    static PassRefPtr<RenderTheme> create();

    // A method asking if the theme's controls actually care about redrawing when hovered.
    virtual bool supportsHover(const RenderStyle* style) const { return true; }

    // A method asking if the theme is able to draw the focus ring.
    virtual bool supportsFocusRing(const RenderStyle*) const;

    // A method asking if the control changes its tint when the window has focus or not.
    virtual bool controlSupportsTints(const RenderObject*) const;

    // A general method asking if any control tinting is supported at all.
    virtual bool supportsControlTints() const { return true; }

    // A method to obtain the baseline position for a "leaf" control.  This will only be used if a baseline
    // position cannot be determined by examining child content. Checkboxes and radio buttons are examples of
    // controls that need to do this.
    virtual int baselinePosition(const RenderObject*) const;

    // The platform selection color.
    virtual Color platformActiveSelectionBackgroundColor() const;
    virtual Color platformInactiveSelectionBackgroundColor() const;
    virtual Color platformActiveSelectionForegroundColor() const;
    virtual Color platformInactiveSelectionForegroundColor() const;

    // List Box selection color
    virtual Color activeListBoxSelectionBackgroundColor() const;
    virtual Color activeListBoxSelectionForegroundColor() const;
    virtual Color inactiveListBoxSelectionBackgroundColor() const;
    virtual Color inactiveListBoxSelectionForegroundColor() const;

    virtual double caretBlinkInterval() const;

    // System fonts.
    virtual void systemFont(int propId, FontDescription&) const;

protected:
    virtual bool paintCheckbox(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r);
    virtual void setCheckboxSize(RenderStyle* style) const;

    virtual bool paintRadio(RenderObject* o, const RenderObject::PaintInfo& i, const IntRect& r);
    virtual void setRadioSize(RenderStyle* style) const;

    virtual void adjustButtonStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustTextFieldStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintTextField(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual bool paintTextArea(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustMenuListStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintMenuList(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustSearchFieldResultsDecorationStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintSearchFieldResultsDecoration(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustSearchFieldStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintSearchField(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustSearchFieldResultsButtonStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintSearchFieldResultsButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustSearchFieldCancelButtonStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintSearchFieldCancelButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);


private:
    /*
     * hold the state
     */
    GtkWidget* gtkEntry() const;
    GtkWidget* gtkTreeView() const;

    /*
     * unmapped GdkWindow having a container. This is holding all
     * our fake widgets
     */
    GtkContainer* gtkContainer() const;

private:
    mutable GtkWidget* m_gtkWindow;
    mutable GtkContainer* m_gtkContainer;
    mutable GtkWidget* m_gtkEntry;
    mutable GtkWidget* m_gtkTreeView;
};

}

#endif
