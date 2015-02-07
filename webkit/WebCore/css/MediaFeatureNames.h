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
 * This file is part of the CSS implementation for KDE.
 *
 * Copyright (C) 2005 Apple Computer, Inc.
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
#ifndef MediaFeatureNames_h
#define MediaFeatureNames_h

#include "AtomicString.h"

namespace WebCore {
    namespace MediaFeatureNames {

#define CSS_MEDIAQUERY_NAMES_FOR_EACH_MEDIAFEATURE(macro) \
    macro(color, "color") \
    macro(grid, "grid") \
    macro(monochrome, "monochrome") \
    macro(height, "height") \
    macro(width, "width") \
    macro(orientation, "orientation") \
    macro(aspect_ratio, "aspect-ratio") \
    macro(device_aspect_ratio, "device-aspect-ratio") \
    macro(device_pixel_ratio, "-webkit-device-pixel-ratio") \
    macro(device_height, "device-height") \
    macro(device_width, "device-width") \
    macro(max_color, "max-color") \
    macro(max_aspect_ratio, "max-aspect-ratio") \
    macro(max_device_aspect_ratio, "max-device-aspect-ratio") \
    macro(max_device_pixel_ratio, "-webkit-max-device-pixel-ratio") \
    macro(max_device_height, "max-device-height") \
    macro(max_device_width, "max-device-width") \
    macro(max_height, "max-height") \
    macro(max_monochrome, "max-monochrome") \
    macro(max_width, "max-width") \
    macro(min_color, "min-color") \
    macro(min_aspect_ratio, "min-aspect-ratio") \
    macro(min_device_aspect_ratio, "min-device-aspect-ratio") \
    macro(min_device_pixel_ratio, "-webkit-min-device-pixel-ratio") \
    macro(min_device_height, "min-device-height") \
    macro(min_device_width, "min-device-width") \
    macro(min_height, "min-height") \
    macro(min_monochrome, "min-monochrome") \
    macro(min_width, "min-width") \
    macro(transform_2d, "-webkit-transform-2d") \
    macro(transform_3d, "-webkit-transform-3d") \
    macro(transition, "-webkit-transition") \
    macro(animation, "-webkit-animation") \
// end of macro

#ifndef CSS_MEDIAQUERY_NAMES_HIDE_GLOBALS
    #define CSS_MEDIAQUERY_NAMES_DECLARE(name, str) extern const AtomicString name##MediaFeature;
    CSS_MEDIAQUERY_NAMES_FOR_EACH_MEDIAFEATURE(CSS_MEDIAQUERY_NAMES_DECLARE)
    #undef CSS_MEDIAQUERY_NAMES_DECLARE
#endif

        void init();

    } // namespace MediaFeatureNames
} // namespace WebCore

#endif // MediaFeatureNames_h
