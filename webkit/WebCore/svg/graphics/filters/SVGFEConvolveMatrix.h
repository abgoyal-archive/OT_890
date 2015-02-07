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
    Copyright (C) 2004, 2005, 2006, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SVGFEConvolveMatrix_h
#define SVGFEConvolveMatrix_h

#if ENABLE(SVG) && ENABLE(FILTERS)
#include "FilterEffect.h"
#include "FloatPoint.h"
#include "FloatSize.h"
#include "Filter.h"
#include <wtf/Vector.h>

namespace WebCore {

    enum EdgeModeType {
        EDGEMODE_UNKNOWN   = 0,
        EDGEMODE_DUPLICATE = 1,
        EDGEMODE_WRAP      = 2,
        EDGEMODE_NONE      = 3
    };

    class FEConvolveMatrix : public FilterEffect {
    public:
        static PassRefPtr<FEConvolveMatrix> create(FilterEffect*, FilterEffect*, const FloatSize&, 
                const float&, const float&, const FloatSize&, EdgeModeType, const FloatPoint&,
                bool, const Vector<float>&);

        FloatSize kernelSize() const;
        void setKernelSize(FloatSize);

        const Vector<float>& kernel() const;
        void setKernel(const Vector<float>&);

        float divisor() const;
        void setDivisor(float);

        float bias() const;
        void setBias(float);

        FloatSize targetOffset() const;
        void setTargetOffset(FloatSize);

        EdgeModeType edgeMode() const;
        void setEdgeMode(EdgeModeType);

        FloatPoint kernelUnitLength() const;
        void setKernelUnitLength(FloatPoint);

        bool preserveAlpha() const;
        void setPreserveAlpha(bool);

        virtual FloatRect uniteChildEffectSubregions(Filter* filter) { return calculateUnionOfChildEffectSubregions(filter, m_in.get(), m_in2.get()); }
        void apply(Filter*);
        void dump();
        TextStream& externalRepresentation(TextStream& ts) const;

    private:
        FEConvolveMatrix(FilterEffect*, FilterEffect*, const FloatSize&, const float&, const float&,
                const FloatSize&, EdgeModeType, const FloatPoint&, bool, const Vector<float>&);

        RefPtr<FilterEffect> m_in;
        RefPtr<FilterEffect> m_in2;
        FloatSize m_kernelSize;
        float m_divisor;
        float m_bias;
        FloatSize m_targetOffset;
        EdgeModeType m_edgeMode;
        FloatPoint m_kernelUnitLength;
        bool m_preserveAlpha;
        Vector<float> m_kernelMatrix;
    };

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)

#endif // SVGFEConvolveMatrix_h
