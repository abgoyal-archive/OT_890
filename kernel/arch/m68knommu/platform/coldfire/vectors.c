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

/***************************************************************************/

/*
 *	linux/arch/m68knommu/platform/5307/vectors.c
 *
 *	Copyright (C) 1999-2007, Greg Ungerer <gerg@snapgear.com>
 */

/***************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <asm/traps.h>
#include <asm/machdep.h>
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfdma.h>
#include <asm/mcfwdebug.h>

/***************************************************************************/

#ifdef TRAP_DBG_INTERRUPT

asmlinkage void dbginterrupt_c(struct frame *fp)
{
	extern void dump(struct pt_regs *fp);
	printk(KERN_DEBUG "%s(%d): BUS ERROR TRAP\n", __FILE__, __LINE__);
	dump((struct pt_regs *) fp);
	asm("halt");
}

#endif

/***************************************************************************/

extern e_vector	*_ramvec;

void set_evector(int vecnum, void (*handler)(void))
{
	if (vecnum >= 0 && vecnum <= 255)
		_ramvec[vecnum] = handler;
}

/***************************************************************************/

/* Assembler routines */
asmlinkage void buserr(void);
asmlinkage void trap(void);
asmlinkage void system_call(void);
asmlinkage void inthandler(void);

void __init init_vectors(void)
{
	int i;

	/*
	 *	There is a common trap handler and common interrupt
	 *	handler that handle almost every vector. We treat
	 *	the system call and bus error special, they get their
	 *	own first level handlers.
	 */
	for (i = 3; (i <= 23); i++)
		_ramvec[i] = trap;
	for (i = 33; (i <= 63); i++)
		_ramvec[i] = trap;
	for (i = 24; (i <= 31); i++)
		_ramvec[i] = inthandler;
	for (i = 64; (i < 255); i++)
		_ramvec[i] = inthandler;
	_ramvec[255] = 0;

	_ramvec[2] = buserr;
	_ramvec[32] = system_call;

#ifdef TRAP_DBG_INTERRUPT
	_ramvec[12] = dbginterrupt;
#endif
}

/***************************************************************************/

void enable_vector(unsigned int irq)
{
	/* Currently no action on ColdFire */
}

void disable_vector(unsigned int irq)
{
	/* Currently no action on ColdFire */
}

void ack_vector(unsigned int irq)
{
	/* Currently no action on ColdFire */
}

/***************************************************************************/

void coldfire_reset(void)
{
	HARD_RESET_NOW();
}

/***************************************************************************/
