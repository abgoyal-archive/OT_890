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
 *  include/asm-s390/ebcdic.h
 *    EBCDIC -> ASCII, ASCII -> EBCDIC conversion routines.
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#ifndef _EBCDIC_H
#define _EBCDIC_H

#ifndef _S390_TYPES_H
#include <types.h>
#endif

extern __u8 _ascebc_500[256];   /* ASCII -> EBCDIC 500 conversion table */
extern __u8 _ebcasc_500[256];   /* EBCDIC 500 -> ASCII conversion table */
extern __u8 _ascebc[256];   /* ASCII -> EBCDIC conversion table */
extern __u8 _ebcasc[256];   /* EBCDIC -> ASCII conversion table */
extern __u8 _ebc_tolower[256]; /* EBCDIC -> lowercase */
extern __u8 _ebc_toupper[256]; /* EBCDIC -> uppercase */

static inline void
codepage_convert(const __u8 *codepage, volatile __u8 * addr, unsigned long nr)
{
	if (nr-- <= 0)
		return;
	asm volatile(
		"	bras	1,1f\n"
		"	tr	0(1,%0),0(%2)\n"
		"0:	tr	0(256,%0),0(%2)\n"
		"	la	%0,256(%0)\n"
		"1:	ahi	%1,-256\n"
		"	jnm	0b\n"
		"	ex	%1,0(1)"
		: "+&a" (addr), "+&a" (nr)
		: "a" (codepage) : "cc", "memory", "1");
}

#define ASCEBC(addr,nr) codepage_convert(_ascebc, addr, nr)
#define EBCASC(addr,nr) codepage_convert(_ebcasc, addr, nr)
#define ASCEBC_500(addr,nr) codepage_convert(_ascebc_500, addr, nr)
#define EBCASC_500(addr,nr) codepage_convert(_ebcasc_500, addr, nr)
#define EBC_TOLOWER(addr,nr) codepage_convert(_ebc_tolower, addr, nr)
#define EBC_TOUPPER(addr,nr) codepage_convert(_ebc_toupper, addr, nr)

#endif

