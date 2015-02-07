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

/* Boot entry point for a compressed MN10300 kernel
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
	.section	.text

#define DEBUG

#include <linux/linkage.h>
#include <asm/cpu-regs.h>

	.globl startup_32
startup_32:
	# first save off parameters from bootloader
	mov	param_save_area,a0
	mov	d0,(a0)
	mov	d1,(4,a0)
	mov	d2,(8,a0)

	mov	sp,a3
	mov	decomp_stack+0x2000-4,a0
	mov	a0,sp

	# invalidate and enable both of the caches
	mov	CHCTR,a0
	clr	d0
	movhu	d0,(a0)					# turn off first
	mov	CHCTR_ICINV|CHCTR_DCINV,d0
	movhu	d0,(a0)
	setlb
	mov	(a0),d0
	btst	CHCTR_ICBUSY|CHCTR_DCBUSY,d0		# wait till not busy
	lne
	mov	CHCTR_ICEN|CHCTR_DCEN|CHCTR_DCWTMD,d0	# writethru dcache
	movhu	d0,(a0)					# enable

	# clear the BSS area
	mov	__bss_start,a0
	mov	_end,a1
	clr	d0
bssclear:
	cmp	a1,a0
	bge	bssclear_end
	movbu	d0,(a0)
	inc	a0
	bra	bssclear
bssclear_end:

	# decompress the kernel
	call	decompress_kernel[],0

	# disable caches again
	mov	CHCTR,a0
	clr	d0
	movhu	d0,(a0)
	setlb
	mov	(a0),d0
	btst	CHCTR_ICBUSY|CHCTR_DCBUSY,d0		# wait till not busy
	lne

	mov	param_save_area,a0
	mov	(a0),d0
	mov	(4,a0),d1
	mov	(8,a0),d2

	mov	a3,sp
	mov	CONFIG_KERNEL_TEXT_ADDRESS,a0
	jmp	(a0)

	.data
	.align		4
param_save_area:
	.rept 3
	.word		0
	.endr

	.section	.bss
	.align		4
decomp_stack:
	.space		0x2000
