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
# MN10300 Low-level gdbstub routines
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
#include <asm/smp.h>
#include <asm/cache.h>
#include <asm/cpu-regs.h>
#include <asm/exceptions.h>
#include <asm/frame.inc>
#include <asm/serial-regs.h>

	.text

###############################################################################
#
# GDB stub read memory with guard
# - D0 holds the memory address to read
# - D1 holds the address to store the byte into
#
###############################################################################
	.globl gdbstub_read_byte_guard
	.globl gdbstub_read_byte_cont
ENTRY(gdbstub_read_byte)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_read_byte_guard:
	movbu	(a0),d1
gdbstub_read_byte_cont:
	movbu	d1,(a1)
	ret	[],0

	.globl gdbstub_read_word_guard
	.globl gdbstub_read_word_cont
ENTRY(gdbstub_read_word)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_read_word_guard:
	movhu	(a0),d1
gdbstub_read_word_cont:
	movhu	d1,(a1)
	ret	[],0

	.globl gdbstub_read_dword_guard
	.globl gdbstub_read_dword_cont
ENTRY(gdbstub_read_dword)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_read_dword_guard:
	mov	(a0),d1
gdbstub_read_dword_cont:
	mov	d1,(a1)
	ret	[],0

###############################################################################
#
# GDB stub write memory with guard
# - D0 holds the byte to store
# - D1 holds the memory address to write
#
###############################################################################
	.globl gdbstub_write_byte_guard
	.globl gdbstub_write_byte_cont
ENTRY(gdbstub_write_byte)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_write_byte_guard:
	movbu	a0,(a1)
gdbstub_write_byte_cont:
	ret	[],0

	.globl gdbstub_write_word_guard
	.globl gdbstub_write_word_cont
ENTRY(gdbstub_write_word)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_write_word_guard:
	movhu	a0,(a1)
gdbstub_write_word_cont:
	ret	[],0

	.globl gdbstub_write_dword_guard
	.globl gdbstub_write_dword_cont
ENTRY(gdbstub_write_dword)
	mov	d0,a0
	mov	d1,a1
	clr	d0
gdbstub_write_dword_guard:
	mov	a0,(a1)
gdbstub_write_dword_cont:
	ret	[],0

###############################################################################
#
# GDB stub BUG() trap
#
###############################################################################
ENTRY(__gdbstub_bug_trap)
	.byte	0xF7,0xF7	# don't use 0xFF as the JTAG unit preempts that
	ret	[],0
