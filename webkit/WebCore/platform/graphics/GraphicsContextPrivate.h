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

#ifndef GraphicsContextPrivate_h
#define GraphicsContextPrivate_h

#include "TransformationMatrix.h"
#include "Gradient.h"
#include "GraphicsContext.h"
#include "Pattern.h"

namespace WebCore {

    struct GraphicsContextState {
        GraphicsContextState()
            : textDrawingMode(cTextFill)
            , strokeStyle(SolidStroke)
            , strokeThickness(0)
#if PLATFORM(CAIRO)
            , globalAlpha(1.0f)
#endif
            , strokeColorSpace(SolidColorSpace)
            , strokeColor(Color::black)
            , fillRule(RULE_NONZERO)
            , fillColorSpace(SolidColorSpace)
            , fillColor(Color::black)
            , shouldAntialias(true)
            , paintingDisabled(false)
            , shadowBlur(0)
            , shadowsIgnoreTransforms(false)
        {
        }

        int textDrawingMode;
        
        StrokeStyle strokeStyle;
        float strokeThickness;
#if PLATFORM(CAIRO)
        float globalAlpha;
#elif PLATFORM(QT)
        TransformationMatrix pathTransform;
#endif
        ColorSpace strokeColorSpace;
        Color strokeColor;
        RefPtr<Gradient> strokeGradient;
        RefPtr<Pattern> strokePattern;
        
        WindRule fillRule;
        ColorSpace fillColorSpace;
        Color fillColor;
        RefPtr<Gradient> fillGradient;
        RefPtr<Pattern> fillPattern;

        bool shouldAntialias;

        bool paintingDisabled;
        
        IntSize shadowSize;
        unsigned shadowBlur;
        Color shadowColor;

        bool shadowsIgnoreTransforms;
    };

    class GraphicsContextPrivate {
    public:
        GraphicsContextPrivate()
            : m_focusRingWidth(0)
            , m_focusRingOffset(0)
            , m_updatingControlTints(false)
        {
        }

        GraphicsContextState state;
        Vector<GraphicsContextState> stack;
        Vector<IntRect> m_focusRingRects;
        int m_focusRingWidth;
        int m_focusRingOffset;
        bool m_updatingControlTints;
    };

} // namespace WebCore

#endif // GraphicsContextPrivate_h
