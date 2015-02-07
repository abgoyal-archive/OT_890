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
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
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

#ifndef TransformOperation_h
#define TransformOperation_h

#include "TransformationMatrix.h"
#include "IntSize.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

namespace WebCore {

// CSS Transforms (may become part of CSS3)

class TransformOperation : public RefCounted<TransformOperation> {
public:
    enum OperationType {
        SCALE_X, SCALE_Y, SCALE, 
        TRANSLATE_X, TRANSLATE_Y, TRANSLATE, 
        ROTATE,
        ROTATE_Z = ROTATE,
        SKEW_X, SKEW_Y, SKEW, 
        MATRIX,
        SCALE_Z, SCALE_3D,
        TRANSLATE_Z, TRANSLATE_3D,
        ROTATE_X, ROTATE_Y, ROTATE_3D,
        MATRIX_3D,
        PERSPECTIVE,
        IDENTITY, NONE
    };

    virtual ~TransformOperation() { }

    virtual bool operator==(const TransformOperation&) const = 0;
    bool operator!=(const TransformOperation& o) const { return !(*this == o); }

    virtual bool isIdentity() const = 0;

    // Return true if the borderBoxSize was used in the computation, false otherwise.
    virtual bool apply(TransformationMatrix&, const IntSize& borderBoxSize) const = 0;

    virtual PassRefPtr<TransformOperation> blend(const TransformOperation* from, double progress, bool blendToIdentity = false) = 0;

    virtual OperationType getOperationType() const = 0;
    virtual bool isSameType(const TransformOperation&) const { return false; }
    
    bool is3DOperation() const
    {
        OperationType opType = getOperationType();
        return opType == SCALE_Z ||
               opType == SCALE_3D ||
               opType == TRANSLATE_Z ||
               opType == TRANSLATE_3D ||
               opType == ROTATE_X ||
               opType == ROTATE_Y ||
               opType == ROTATE_3D ||
               opType == MATRIX_3D ||
               opType == PERSPECTIVE;
    }
};

} // namespace WebCore

#endif // TransformOperation_h
