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
 * Copyright (C) 2006 Apple Computer, Inc.
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

#ifndef PopupMenuClient_h
#define PopupMenuClient_h

#include "PopupMenuStyle.h"
#include "ScrollTypes.h"

namespace WebCore {

class Color;
class FontSelector;
class HostWindow;
class Scrollbar;
class ScrollbarClient;
class String;

class PopupMenuClient {
public:
    virtual ~PopupMenuClient() {}
    virtual void valueChanged(unsigned listIndex, bool fireEvents = true) = 0;

    virtual String itemText(unsigned listIndex) const = 0;
    virtual String itemToolTip(unsigned listIndex) const = 0;
    virtual bool itemIsEnabled(unsigned listIndex) const = 0;
    virtual PopupMenuStyle itemStyle(unsigned listIndex) const = 0;
    virtual PopupMenuStyle menuStyle() const = 0;
    virtual int clientInsetLeft() const = 0;
    virtual int clientInsetRight() const = 0;
    virtual int clientPaddingLeft() const = 0;
    virtual int clientPaddingRight() const = 0;
    virtual int listSize() const = 0;
    virtual int selectedIndex() const = 0;
    virtual void hidePopup() = 0;
    virtual bool itemIsSeparator(unsigned listIndex) const = 0;
    virtual bool itemIsLabel(unsigned listIndex) const = 0;
    virtual bool itemIsSelected(unsigned listIndex) const = 0;
    virtual bool shouldPopOver() const = 0;
    virtual bool valueShouldChangeOnHotTrack() const = 0;
    virtual void setTextFromItem(unsigned listIndex) = 0;
    
    virtual FontSelector* fontSelector() const = 0;
    virtual HostWindow* hostWindow() const = 0;
    
    virtual PassRefPtr<Scrollbar> createScrollbar(ScrollbarClient*, ScrollbarOrientation, ScrollbarControlSize) = 0;
};

}

#endif
