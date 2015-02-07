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
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Cameron Zwarich (cwzwarich@uwaterloo.ca)
 *  Copyright (C) 2007 Maks Orlovich
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef JSGlobalObjectFunctions_h
#define JSGlobalObjectFunctions_h

#include <wtf/unicode/Unicode.h>

namespace JSC {

    class ArgList;
    class ExecState;
    class JSObject;
    class JSValue;

    // FIXME: These functions should really be in JSGlobalObject.cpp, but putting them there
    // is a 0.5% reduction.

    JSValue JSC_HOST_CALL globalFuncEval(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncParseInt(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncParseFloat(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncIsNaN(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncIsFinite(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncDecodeURI(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncDecodeURIComponent(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncEncodeURI(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncEncodeURIComponent(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncEscape(ExecState*, JSObject*, JSValue, const ArgList&);
    JSValue JSC_HOST_CALL globalFuncUnescape(ExecState*, JSObject*, JSValue, const ArgList&);
#ifndef NDEBUG
    JSValue JSC_HOST_CALL globalFuncJSCPrint(ExecState*, JSObject*, JSValue, const ArgList&);
#endif

    static const double mantissaOverflowLowerBound = 9007199254740992.0;
    double parseIntOverflow(const char*, int length, int radix);
    bool isStrWhiteSpace(UChar);

} // namespace JSC

#endif // JSGlobalObjectFunctions_h
