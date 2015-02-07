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
******************************************************************************
*
*   Copyright (C) 1997-2004, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
******************************************************************************
*
*  FILE NAME : platform.h
*
*   Date        Name        Description
*   05/13/98    nos         Creation (content moved here from ptypes.h).
*   03/02/99    stephen     Added AS400 support.
*   03/30/99    stephen     Added Linux support.
*   04/13/99    stephen     Reworked for autoconf.
******************************************************************************
*/

/* Define the platform we're on. */
#ifndef U_DARWIN
#define U_DARWIN
#endif

/* Define whether inttypes.h is available */
#ifndef U_HAVE_INTTYPES_H
#define U_HAVE_INTTYPES_H 1
#endif

/*
 * Define what support for C++ streams is available.
 *     If U_IOSTREAM_SOURCE is set to 199711, then <iostream> is available
 * (1997711 is the date the ISO/IEC C++ FDIS was published), and then
 * one should qualify streams using the std namespace in ICU header
 * files.
 *     If U_IOSTREAM_SOURCE is set to 198506, then <iostream.h> is
 * available instead (198506 is the date when Stroustrup published
 * "An Extensible I/O Facility for C++" at the summer USENIX conference).
 *     If U_IOSTREAM_SOURCE is 0, then C++ streams are not available and
 * support for them will be silently suppressed in ICU.
 *
 */

#ifndef U_IOSTREAM_SOURCE
#define U_IOSTREAM_SOURCE 199711
#endif

/* Determines whether specific types are available */
#ifndef U_HAVE_INT8_T
#define U_HAVE_INT8_T 1
#endif

#ifndef U_HAVE_UINT8_T
#define U_HAVE_UINT8_T 0
#endif

#ifndef U_HAVE_INT16_T
#define U_HAVE_INT16_T 1
#endif

#ifndef U_HAVE_UINT16_T
#define U_HAVE_UINT16_T 0
#endif

#ifndef U_HAVE_INT32_T
#define U_HAVE_INT32_T 1
#endif

#ifndef U_HAVE_UINT32_T
#define U_HAVE_UINT32_T 0
#endif

#ifndef U_HAVE_INT64_T
#define U_HAVE_INT64_T 1
#endif

#ifndef U_HAVE_UINT64_T
#define U_HAVE_UINT64_T 0
#endif

/*===========================================================================*/
/* Generic data types                                                        */
/*===========================================================================*/

#include <sys/types.h>

/* If your platform does not have the <inttypes.h> header, you may
   need to edit the typedefs below. */
#if U_HAVE_INTTYPES_H

/* autoconf 2.13 sometimes can't properly find the data types in <inttypes.h> */
/* os/390 needs <inttypes.h>, but it doesn't have int8_t, and it sometimes */
/* doesn't have uint8_t depending on the OS version. */
/* So we have this work around. */
#ifdef OS390
/* The features header is needed to get (u)int64_t sometimes. */
#include <features.h>
#if ! U_HAVE_INT8_T
typedef signed char int8_t;
#endif
#if !defined(__uint8_t)
#define __uint8_t 1
typedef unsigned char uint8_t;
#endif
#endif /* OS390 */

#include <inttypes.h>

#else /* U_HAVE_INTTYPES_H */

#if ! U_HAVE_INT8_T
typedef signed char int8_t;
#endif

#if ! U_HAVE_UINT8_T
typedef unsigned char uint8_t;
#endif

#if ! U_HAVE_INT16_T
typedef signed short int16_t;
#endif

#if ! U_HAVE_UINT16_T
typedef unsigned short uint16_t;
#endif

#if ! U_HAVE_INT32_T
typedef signed int int32_t;
#endif

#if ! U_HAVE_UINT32_T
typedef unsigned int uint32_t;
#endif

#if ! U_HAVE_INT64_T
    typedef signed long long int64_t;
/* else we may not have a 64-bit type */
#endif

#if ! U_HAVE_UINT64_T
    typedef unsigned long long uint64_t;
/* else we may not have a 64-bit type */
#endif

#endif

/*===========================================================================*/
/* Compiler and environment features                                         */
/*===========================================================================*/

/* Define whether namespace is supported */
#ifndef U_HAVE_NAMESPACE
#define U_HAVE_NAMESPACE 1
#endif

/* Determines the endianness of the platform
   It's done this way in case multiple architectures are being built at once.
   For example, Darwin supports fat binaries, which can be both PPC and x86 based. */
#if defined(BYTE_ORDER) && defined(BIG_ENDIAN)
#define U_IS_BIG_ENDIAN (BYTE_ORDER == BIG_ENDIAN)
#else
#define U_IS_BIG_ENDIAN 1
#endif

/* 1 or 0 to enable or disable threads.  If undefined, default is: enable threads. */
#define ICU_USE_THREADS 1

#ifndef U_DEBUG
#define U_DEBUG 0
#endif

#ifndef U_RELEASE
#define U_RELEASE 1
#endif

/* Determine whether to disable renaming or not. This overrides the
   setting in umachine.h which is for all platforms. */
#ifndef U_DISABLE_RENAMING
#define U_DISABLE_RENAMING 1
#endif

/* Determine whether to override new and delete. */
#ifndef U_OVERRIDE_CXX_ALLOCATION
#define U_OVERRIDE_CXX_ALLOCATION 1
#endif
/* Determine whether to override placement new and delete for STL. */
#ifndef U_HAVE_PLACEMENT_NEW
#define U_HAVE_PLACEMENT_NEW 1
#endif

/* Determine whether to enable tracing. */
#ifndef U_ENABLE_TRACING
#define U_ENABLE_TRACING 1
#endif

/* Define the library suffix in a C syntax. */
#define U_HAVE_LIB_SUFFIX 0
#define U_LIB_SUFFIX_C_NAME 
#define U_LIB_SUFFIX_C_NAME_STRING ""

/*===========================================================================*/
/* Character data types                                                      */
/*===========================================================================*/

#if defined(OS390) || defined(OS400)
#   define U_CHARSET_FAMILY 1
#endif

/*===========================================================================*/
/* Information about wchar support                                           */
/*===========================================================================*/

#define U_HAVE_WCHAR_H      1
#define U_SIZEOF_WCHAR_T    4

#define U_HAVE_WCSCPY       1

/*===========================================================================*/
/* Information about POSIX support                                           */
/*===========================================================================*/

#define U_HAVE_NL_LANGINFO          1
#define U_HAVE_NL_LANGINFO_CODESET  1
#define U_NL_LANGINFO_CODESET       CODESET

#if 1
#define U_TZSET         tzset
#endif
#if 0
#define U_TIMEZONE      
#endif
#if 1
#define U_TZNAME        tzname
#endif

#define U_HAVE_MMAP     1
#define U_HAVE_POPEN    1

/*===========================================================================*/
/* Symbol import-export control                                              */
/*===========================================================================*/

#define U_EXPORT
/* U_CALLCONV is releated to U_EXPORT2 */
#define U_EXPORT2

/* cygwin needs to export/import data */
#ifdef U_CYGWIN
#define U_IMPORT __declspec(dllimport)
#else
#define U_IMPORT 
#endif

/*===========================================================================*/
/* Code alignment and C function inlining                                    */
/*===========================================================================*/

#ifndef U_INLINE
#define U_INLINE inline
#endif

#define U_ALIGN_CODE(n) 

/*===========================================================================*/
/* Programs used by ICU code                                                 */
/*===========================================================================*/

#define U_MAKE  "/usr/bin/gnumake"
