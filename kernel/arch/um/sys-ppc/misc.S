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
 * This file contains miscellaneous low-level functions.
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 * Largely rewritten by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras.
 *
 * A couple of functions stolen from arch/ppc/kernel/misc.S for UML
 * by Chris Emerson.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */

#include <asm/processor.h>
#include "ppc_asm.h"

#if defined(CONFIG_4xx) || defined(CONFIG_8xx)
#define CACHE_LINE_SIZE		16
#define LG_CACHE_LINE_SIZE	4
#define MAX_COPY_PREFETCH	1
#else
#define CACHE_LINE_SIZE		32
#define LG_CACHE_LINE_SIZE	5
#define MAX_COPY_PREFETCH	4
#endif /* CONFIG_4xx || CONFIG_8xx */

	.text

/*
 * Clear a page using the dcbz instruction, which doesn't cause any
 * memory traffic (except to write out any cache lines which get
 * displaced).  This only works on cacheable memory.
 */
_GLOBAL(clear_page)
	li	r0,4096/CACHE_LINE_SIZE
	mtctr	r0
#ifdef CONFIG_8xx
	li	r4, 0
1:	stw	r4, 0(r3)
	stw	r4, 4(r3)
	stw	r4, 8(r3)
	stw	r4, 12(r3)
#else
1:	dcbz	0,r3
#endif
	addi	r3,r3,CACHE_LINE_SIZE
	bdnz	1b
	blr

/*
 * Copy a whole page.  We use the dcbz instruction on the destination
 * to reduce memory traffic (it eliminates the unnecessary reads of
 * the destination into cache).  This requires that the destination
 * is cacheable.
 */
#define COPY_16_BYTES		\
	lwz	r6,4(r4);	\
	lwz	r7,8(r4);	\
	lwz	r8,12(r4);	\
	lwzu	r9,16(r4);	\
	stw	r6,4(r3);	\
	stw	r7,8(r3);	\
	stw	r8,12(r3);	\
	stwu	r9,16(r3)

_GLOBAL(copy_page)
	addi	r3,r3,-4
	addi	r4,r4,-4
	li	r5,4

#ifndef CONFIG_8xx
#if MAX_COPY_PREFETCH > 1
	li	r0,MAX_COPY_PREFETCH
	li	r11,4
	mtctr	r0
11:	dcbt	r11,r4
	addi	r11,r11,CACHE_LINE_SIZE
	bdnz	11b
#else /* MAX_COPY_PREFETCH == 1 */
	dcbt	r5,r4
	li	r11,CACHE_LINE_SIZE+4
#endif /* MAX_COPY_PREFETCH */
#endif /* CONFIG_8xx */

	li	r0,4096/CACHE_LINE_SIZE
	mtctr	r0
1:
#ifndef CONFIG_8xx
	dcbt	r11,r4
	dcbz	r5,r3
#endif
	COPY_16_BYTES
#if CACHE_LINE_SIZE >= 32
	COPY_16_BYTES
#if CACHE_LINE_SIZE >= 64
	COPY_16_BYTES
	COPY_16_BYTES
#if CACHE_LINE_SIZE >= 128
	COPY_16_BYTES
	COPY_16_BYTES
	COPY_16_BYTES
	COPY_16_BYTES
#endif
#endif
#endif
	bdnz	1b
	blr
