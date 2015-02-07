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
 * This file is part of the DOM implementation for KDE.
 *
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

#ifndef StaticConstructors_h
#define StaticConstructors_h

// For WebCore we need to avoid having static constructors. We achieve this
// with two separate methods for GCC and MSVC. Both methods prevent the static
// initializers from being registered and called on program startup. On GCC, we
// declare the global objects with a different type that can be POD default
// initialized by the linker/loader. On MSVC we use a special compiler feature
// to have the CRT ignore our static initializers. The constructors will never
// be called and the objects will be left uninitialized.
//
// With both of these approaches, we must define and explicitly call an init
// routine that uses placement new to create the objects and overwrite the
// uninitialized placeholders.
//
// This is not completely portable, but is what we have for now without
// changing how a lot of code accesses these global objects.

#ifdef SKIP_STATIC_CONSTRUCTORS_ON_MSVC
// - Assume that all includes of this header want ALL of their static
//   initializers ignored. This is currently the case. This means that if
//   a .cc includes this header (or it somehow gets included), all static
//   initializers after the include will not be executed.
// - We do this with a pragma, so that all of the static initializer pointers
//   go into our own section, and the CRT won't call them. Eventually it would
//   be nice if the section was discarded, because we don't want the pointers.
//   See: http://msdn.microsoft.com/en-us/library/7977wcck(VS.80).aspx
#pragma warning(disable:4075)
#pragma init_seg(".unwantedstaticinits")
#endif

#ifndef SKIP_STATIC_CONSTRUCTORS_ON_GCC
    // Define an global in the normal way.
#if COMPILER(MSVC7)
#define DEFINE_GLOBAL(type, name) \
    const type name;
#else
#define DEFINE_GLOBAL(type, name, ...) \
    const type name;
#endif

#else
// Define an correctly-sized array of pointers to avoid static initialization.
// Use an array of pointers instead of an array of char in case there is some alignment issue.
#if COMPILER(MSVC7)
#define DEFINE_GLOBAL(type, name) \
    void * name[(sizeof(type) + sizeof(void *) - 1) / sizeof(void *)];
#else
#define DEFINE_GLOBAL(type, name, ...) \
    void * name[(sizeof(type) + sizeof(void *) - 1) / sizeof(void *)];
#endif
#endif

#endif // StaticConstructors_h
