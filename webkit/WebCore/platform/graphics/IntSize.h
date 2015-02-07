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
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef IntSize_h
#define IntSize_h

#include <wtf/Platform.h>

#if PLATFORM(CG)
typedef struct CGSize CGSize;
#endif

#if PLATFORM(MAC)
#ifdef NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES
typedef struct CGSize NSSize;
#else
typedef struct _NSSize NSSize;
#endif
#endif

#if PLATFORM(WIN)
typedef struct tagSIZE SIZE;
#elif PLATFORM(QT)
#include <qglobal.h>
QT_BEGIN_NAMESPACE
class QSize;
QT_END_NAMESPACE
#endif

namespace WebCore {

class IntSize {
public:
    IntSize() : m_width(0), m_height(0) { }
    IntSize(int width, int height) : m_width(width), m_height(height) { }
    
    int width() const { return m_width; }
    int height() const { return m_height; }

    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }

    bool isEmpty() const { return m_width <= 0 || m_height <= 0; }

    void expand(int width, int height)
    {
        m_width += width;
        m_height += height;
    }
    
    void scale(float scale)
    {
        m_width = static_cast<int>(static_cast<float>(m_width) * scale);
        m_height = static_cast<int>(static_cast<float>(m_height) * scale);
    }
    
    IntSize expandedTo(const IntSize& other) const
    {
        return IntSize(m_width > other.m_width ? m_width : other.m_width,
            m_height > other.m_height ? m_height : other.m_height);
    }

    IntSize shrunkTo(const IntSize& other) const
    {
        return IntSize(m_width < other.m_width ? m_width : other.m_width,
            m_height < other.m_height ? m_height : other.m_height);
    }

    void clampNegativeToZero()
    {
        *this = expandedTo(IntSize());
    }

#if PLATFORM(CG)
    explicit IntSize(const CGSize&); // don't do this implicitly since it's lossy
    operator CGSize() const;
#endif

#if PLATFORM(MAC) && !defined(NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES)
    explicit IntSize(const NSSize &); // don't do this implicitly since it's lossy
    operator NSSize() const;
#endif

#if PLATFORM(WIN)
    IntSize(const SIZE&);
    operator SIZE() const;
#endif

#if PLATFORM(QT)
    IntSize(const QSize&);
    operator QSize() const;
#endif


private:
    int m_width, m_height;
};

inline IntSize& operator+=(IntSize& a, const IntSize& b)
{
    a.setWidth(a.width() + b.width());
    a.setHeight(a.height() + b.height());
    return a;
}

inline IntSize& operator-=(IntSize& a, const IntSize& b)
{
    a.setWidth(a.width() - b.width());
    a.setHeight(a.height() - b.height());
    return a;
}

inline IntSize operator+(const IntSize& a, const IntSize& b)
{
    return IntSize(a.width() + b.width(), a.height() + b.height());
}

inline IntSize operator-(const IntSize& a, const IntSize& b)
{
    return IntSize(a.width() - b.width(), a.height() - b.height());
}

inline IntSize operator-(const IntSize& size)
{
    return IntSize(-size.width(), -size.height());
}

inline bool operator==(const IntSize& a, const IntSize& b)
{
    return a.width() == b.width() && a.height() == b.height();
}

inline bool operator!=(const IntSize& a, const IntSize& b)
{
    return a.width() != b.width() || a.height() != b.height();
}

} // namespace WebCore

#endif // IntSize_h
