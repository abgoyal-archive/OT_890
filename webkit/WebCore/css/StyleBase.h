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
 * Copyright (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 1999 Waldo Bastian (bastian@kde.org)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmial.com)
 * Copyright (C) 2004, 2006, 2008 Apple Inc. All rights reserved.
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
 */

#ifndef StyleBase_h
#define StyleBase_h

#include <wtf/RefCounted.h>

namespace WebCore {

    class String;
    class StyleSheet;
    class KURL;

    // Base class for most CSS DOM objects.

    // FIXME: We don't need these to all share one base class.
    // Refactor so they don't any more.

    class StyleBase : public RefCounted<StyleBase> {
    public:
        virtual ~StyleBase() { }

        StyleBase* parent() const { return m_parent; }
        void setParent(StyleBase* parent) { m_parent = parent; }

        // returns the url of the style sheet this object belongs to
        KURL baseURL() const;

        virtual bool isCSSStyleSheet() const { return false; }
        virtual bool isCharsetRule() { return false; }
        virtual bool isFontFaceRule() { return false; }
        virtual bool isImportRule() { return false; }
        virtual bool isKeyframeRule() { return false; }
        virtual bool isKeyframesRule() { return false; }
        virtual bool isMediaRule() { return false; }
        virtual bool isVariablesRule() { return false; }
        
        virtual bool isRule() { return false; }
        virtual bool isStyleRule() { return false; }
        virtual bool isStyleSheet() const { return false; }
        virtual bool isXSLStyleSheet() const { return false; }

        virtual bool isMutableStyleDeclaration() const { return false; }

        virtual String cssText() const;

        virtual void checkLoaded();

        bool useStrictParsing() const { return !m_parent || m_parent->useStrictParsing(); }

        virtual void insertedIntoParent() { }

        StyleSheet* stylesheet();

#ifdef ANDROID_INSTRUMENT
        // Overridden to prevent the normal new from being called.
        void* operator new(size_t) throw();

        // Overridden to prevent the normal delete from being called.
        void operator delete(void*, size_t);

        static size_t reportStyleSize();
#endif

    protected:
        StyleBase(StyleBase* parent)
            : m_parent(parent)
        {
        }

    private:
        StyleBase* m_parent;
    };
}

#endif
