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
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef Icon_h
#define Icon_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/Forward.h>
#include <wtf/Vector.h>

#if PLATFORM(MAC)
#include <wtf/RetainPtr.h>
#ifdef __OBJC__
@class NSImage;
#else
class NSImage;
#endif
#elif PLATFORM(WIN)
typedef struct HICON__* HICON;
#elif PLATFORM(QT)
#include <QIcon>
#elif PLATFORM(GTK)
typedef struct _GdkPixbuf GdkPixbuf;
#elif PLATFORM(CHROMIUM)
#include "PlatformIcon.h"
#endif

namespace WebCore {

class GraphicsContext;
class IntRect;
class String;
    
class Icon : public RefCounted<Icon> {
public:
    static PassRefPtr<Icon> createIconForFile(const String& filename);
    static PassRefPtr<Icon> createIconForFiles(const Vector<String>& filenames);

    ~Icon();

    void paint(GraphicsContext*, const IntRect&);

#if PLATFORM(WIN)
    static PassRefPtr<Icon> create(HICON hIcon) { return adoptRef(new Icon(hIcon)); }
#endif

private:
#if PLATFORM(MAC)
    Icon(NSImage*);
    RetainPtr<NSImage> m_nsImage;
#elif PLATFORM(WIN)
    Icon(HICON);
    HICON m_hIcon;
#elif PLATFORM(QT)
    Icon();
    QIcon m_icon;
#elif PLATFORM(GTK)
    Icon();
    GdkPixbuf* m_icon;
#elif PLATFORM(CHROMIUM)
    Icon(const PlatformIcon&);
    PlatformIcon m_icon;
#endif
};

}

#endif
