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

/* linux/include/asm-arm/plat-s3c/debug-macro.S
 *
 * Copyright 2005, 2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <plat/regs-serial.h>

/* The S3C2440 implementations are used by default as they are the
 * most widely re-used */

	.macro fifo_level_s3c2440 rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		and	\rd, \rd, #S3C2440_UFSTAT_TXMASK
	.endm

#ifndef fifo_level
#define fifo_level fifo_level_s3c2440
#endif

	.macro  fifo_full_s3c2440 rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
		tst	\rd, #S3C2440_UFSTAT_TXFULL
	.endm

#ifndef fifo_full
#define fifo_full fifo_full_s3c2440
#endif

	.macro	senduart,rd,rx
		strb 	\rd, [\rx, # S3C2410_UTXH ]
	.endm

	.macro	busyuart, rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFCON ]
		tst	\rd, #S3C2410_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		fifo_full \rd, \rx
		bne	1003b
		b	1002f

1001:
		@ busy waiting for non fifo
		ldr	\rd, [ \rx, # S3C2410_UTRSTAT ]
		tst	\rd, #S3C2410_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
	.endm

	.macro	waituart,rd,rx
		ldr	\rd, [ \rx, # S3C2410_UFCON ]
		tst	\rd, #S3C2410_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		fifo_level \rd, \rx
		teq	\rd, #0
		bne	1003b
		b	1002f
1001:
		@ idle waiting for non fifo
		ldr	\rd, [ \rx, # S3C2410_UTRSTAT ]
		tst	\rd, #S3C2410_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
	.endm
