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
 * arch/arm/mach-at91/include/mach/debug-macro.S
 *
 *  Copyright (C) 2003-2005 SAN People
 *
 * Debugging macro include header
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <mach/hardware.h>
#include <mach/at91_dbgu.h>

	.macro	addruart,rx
	mrc	p15, 0, \rx, c1, c0
	tst	\rx, #1						@ MMU enabled?
	ldreq	\rx, =(AT91_BASE_SYS + AT91_DBGU)		@ System peripherals (phys address)
	ldrne	\rx, =(AT91_VA_BASE_SYS	+ AT91_DBGU)		@ System peripherals (virt address)
	.endm

	.macro	senduart,rd,rx
	strb	\rd, [\rx, #(AT91_DBGU_THR - AT91_DBGU)]	@ Write to Transmitter Holding Register
	.endm

	.macro	waituart,rd,rx
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR - AT91_DBGU)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXRDY				@ DBGU_TXRDY = 1 when ready to transmit
	beq	1001b
	.endm

	.macro	busyuart,rd,rx
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR - AT91_DBGU)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXEMPTY				@ DBGU_TXEMPTY = 1 when transmission complete
	beq	1001b
	.endm

