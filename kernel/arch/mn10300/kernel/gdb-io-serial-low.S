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
# 16550 serial Rx interrupt handler for gdbstub I/O
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
#include <asm/cpu-regs.h>
#include <asm/thread_info.h>
#include <asm/frame.inc>
#include <asm/intctl-regs.h>
#include <asm/unit/serial.h>

	.text

###############################################################################
#
# GDB stub serial receive interrupt entry point
# - intended to run at interrupt priority 0
#
###############################################################################
	.globl	gdbstub_io_rx_handler
	.type	gdbstub_io_rx_handler,@function
gdbstub_io_rx_handler:
	movm	[d2,d3,a2,a3],(sp)

#if 1
	movbu	(GDBPORT_SERIAL_IIR),d2
#endif

	mov	(gdbstub_rx_inp),a3
gdbstub_io_rx_more:
	mov	a3,a2
	add	2,a3
	and	0x00000fff,a3
	mov	(gdbstub_rx_outp),d3
	cmp	a3,d3
	beq	gdbstub_io_rx_overflow

	movbu	(GDBPORT_SERIAL_LSR),d3
	btst	UART_LSR_DR,d3
	beq	gdbstub_io_rx_done
	movbu	(GDBPORT_SERIAL_RX),d2
	movbu	d3,(gdbstub_rx_buffer+1,a2)
	movbu	d2,(gdbstub_rx_buffer,a2)
	mov	a3,(gdbstub_rx_inp)
	bra	gdbstub_io_rx_more

gdbstub_io_rx_done:
	mov	GxICR_DETECT,d2
	movbu	d2,(XIRQxICR(GDBPORT_SERIAL_IRQ))	# ACK the interrupt
	movhu	(XIRQxICR(GDBPORT_SERIAL_IRQ)),d2	# flush
	movm	(sp),[d2,d3,a2,a3]
	bset	0x01,(gdbstub_busy)
	beq	gdbstub_io_rx_enter
	rti

gdbstub_io_rx_overflow:
	bset	0x01,(gdbstub_rx_overflow)
	bra	gdbstub_io_rx_done

gdbstub_io_rx_enter:
	or	EPSW_IE|EPSW_IM_1,epsw
	add	-4,sp
	SAVE_ALL

	mov	0xffffffff,d0
	mov	d0,(REG_ORIG_D0,fp)
	mov	0x280,d1

	mov	fp,d0
	call	gdbstub_rx_irq[],0	# gdbstub_rx_irq(regs,excep)

	and	~EPSW_IE,epsw
	bclr	0x01,(gdbstub_busy)

	.globl gdbstub_return
gdbstub_return:
	RESTORE_ALL

	.size	gdbstub_io_rx_handler,.-gdbstub_io_rx_handler
