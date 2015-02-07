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

###############################################################################
#
# MN10300 Watchdog interrupt handler
#
# Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
# Written by David Howells (dhowells@redhat.com)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public Licence
# as published by the Free Software Foundation; either version
# 2 of the Licence, or (at your option) any later version.
#
###############################################################################
#include <linux/sys.h>
#include <linux/linkage.h>
#include <asm/intctl-regs.h>
#include <asm/timer-regs.h>
#include <asm/frame.inc>

	.text

###############################################################################
#
# Watchdog handler entry point
# - special non-maskable interrupt
#
###############################################################################
	.globl	watchdog_handler
	.type	watchdog_handler,@function
watchdog_handler:
	add	-4,sp
	SAVE_ALL

	mov	0xffffffff,d0
	mov	d0,(REG_ORIG_D0,fp)

	mov	fp,d0
	lsr	2,d1
	call	watchdog_interrupt[],0		# watchdog_interrupt(regs,irq)

	jmp	ret_from_intr

	.size	watchdog_handler,.-watchdog_handler

###############################################################################
#
# Watchdog touch entry point
# - kept to absolute minimum (unfortunately, it's prototyped in linux/nmi.h so
#   we can't inline it)
#
###############################################################################
	.globl	touch_nmi_watchdog
	.type	touch_nmi_watchdog,@function
touch_nmi_watchdog:
	clr	d0
	mov	d0,(watchdog_alert_counter)
	ret	[],0

	.size	touch_nmi_watchdog,.-touch_nmi_watchdog
