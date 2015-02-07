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
 * Copyright (C) 2006-2007 PA Semi, Inc
 *
 * Maintained by: Olof Johansson <olof@lixom.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <asm/processor.h>
#include <asm/page.h>
#include <asm/ppc_asm.h>
#include <asm/cputable.h>
#include <asm/cache.h>
#include <asm/thread_info.h>
#include <asm/asm-offsets.h>

/* Power savings opcodes since not all binutils have them at this time */
#define DOZE	.long	0x4c000324
#define NAP	.long	0x4c000364
#define SLEEP	.long	0x4c0003a4
#define RVW	.long	0x4c0003e4

/* Common sequence to do before going to any of the
 * powersavings modes.
 */

#define PRE_SLEEP_SEQUENCE	\
	std	r3,8(r1);	\
	ptesync	;		\
	ld	r3,8(r1);	\
1:	cmpd 	r3,r3;		\
	bne	1b

_doze:
	PRE_SLEEP_SEQUENCE
	DOZE
	b	.


_GLOBAL(idle_spin)
	blr

_GLOBAL(idle_doze)
	LOAD_REG_ADDR(r3, _doze)
	b	sleep_common

/* Add more modes here later */

sleep_common:
	mflr	r0
	std	r0, 16(r1)
	stdu	r1,-64(r1)
#ifdef CONFIG_PPC_PASEMI_CPUFREQ
	std	r3, 48(r1)

	/* Only do power savings when in astate 0 */
	bl	.check_astate
	cmpwi	r3,0
	bne	1f

	ld	r3, 48(r1)
#endif
	LOAD_REG_IMMEDIATE(r6,MSR_DR|MSR_IR|MSR_ME|MSR_EE)
	mfmsr	r4
	andc	r5,r4,r6
	mtmsrd	r5,0

	mtctr	r3
	bctrl

	mtmsrd	r4,0

1:	addi	r1,r1,64
	ld	r0,16(r1)
	mtlr	r0
	blr

