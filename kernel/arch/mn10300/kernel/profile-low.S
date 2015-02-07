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
# Fast profiling interrupt handler
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
#include <asm/segment.h>
#include <asm/smp.h>
#include <asm/intctl-regs.h>
#include <asm/timer-regs.h>

#define pi break

	.balign	4
counter:
	.long	-1

###############################################################################
#
# Profiling interrupt entry point
# - intended to run at interrupt priority 1
#
###############################################################################
ENTRY(profile_handler)
	movm	[d2,d3,a2],(sp)

	# ignore userspace
	mov	(12,sp),d2
	and	EPSW_nSL,d2
	bne	out

	# do nothing if there's no buffer
	mov	(prof_buffer),a2
	and	a2,a2
	beq	out
	or	0x20000000,a2

	# calculate relative position in text segment
	mov	(16,sp),d2
	sub	_stext,d2
	mov	(prof_shift),d3
	lsr	d3,d2
	mov	(prof_len),d3
	cmp	d3,d2
	bcc	outside_text

	# increment the appropriate profile bucket
do_inc:
	asl2	d2
	mov	(a2,d2),d3
	inc	d3
	mov	d3,(a2,d2)
out:
	mov	GxICR_DETECT,d2
	movbu	d2,(TM11ICR)		# ACK the interrupt
	movbu	(TM11ICR),d2
	movm	(sp),[d2,d3,a2]
	rti

outside_text:
	sub	1,d3
	mov	d3,d2
	bra	do_inc
