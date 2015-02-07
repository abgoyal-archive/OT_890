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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_ARC_TYPES_H
#define _ASM_ARC_TYPES_H


#ifdef CONFIG_ARC32

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
typedef	long		LONG __attribute__ ((__mode__ (__SI__)));
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__SI__)));
typedef void		VOID;

/* The pointer types.  Note that we're using a 64-bit compiler but all
   pointer in the ARC structures are only 32-bit, so we need some disgusting
   workarounds.  Keep your vomit bag handy.  */
typedef LONG		_PCHAR;
typedef LONG		_PSHORT;
typedef LONG		_PLARGE_INTEGER;
typedef	LONG		_PLONG;
typedef LONG		_PUCHAR;
typedef LONG		_PUSHORT;
typedef LONG		_PULONG;
typedef LONG		_PVOID;

#endif /* CONFIG_ARC32 */

#ifdef CONFIG_ARC64

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
typedef	long		LONG __attribute__ ((__mode__ (__DI__)));
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__DI__)));
typedef void		VOID;

/* The pointer types.  We're 64-bit and the firmware is also 64-bit, so
   live is sane ...  */
typedef CHAR		*_PCHAR;
typedef SHORT		*_PSHORT;
typedef LARGE_INTEGER	*_PLARGE_INTEGER;
typedef	LONG		*_PLONG;
typedef UCHAR		*_PUCHAR;
typedef USHORT		*_PUSHORT;
typedef ULONG		*_PULONG;
typedef VOID		*_PVOID;

#endif /* CONFIG_ARC64  */

typedef CHAR		*PCHAR;
typedef SHORT		*PSHORT;
typedef LARGE_INTEGER	*PLARGE_INTEGER;
typedef	LONG		*PLONG;
typedef UCHAR		*PUCHAR;
typedef USHORT		*PUSHORT;
typedef ULONG		*PULONG;
typedef VOID		*PVOID;

/*
 * Return type of ArcGetDisplayStatus()
 */
typedef struct {
	USHORT	CursorXPosition;
	USHORT	CursorYPosition;
	USHORT	CursorMaxXPosition;
	USHORT	CursorMaxYPosition;
	USHORT	ForegroundColor;
	USHORT	BackgroundColor;
	UCHAR	HighIntensity;
	UCHAR	Underscored;
	UCHAR	ReverseVideo;
} DISPLAY_STATUS;

#endif /* _ASM_ARC_TYPES_H */
