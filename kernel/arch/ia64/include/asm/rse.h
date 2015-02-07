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

#ifndef _ASM_IA64_RSE_H
#define _ASM_IA64_RSE_H

/*
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * Register stack engine related helper functions.  This file may be
 * used in applications, so be careful about the name-space and give
 * some consideration to non-GNU C compilers (though __inline__ is
 * fine).
 */

static __inline__ unsigned long
ia64_rse_slot_num (unsigned long *addr)
{
	return (((unsigned long) addr) >> 3) & 0x3f;
}

/*
 * Return TRUE if ADDR is the address of an RNAT slot.
 */
static __inline__ unsigned long
ia64_rse_is_rnat_slot (unsigned long *addr)
{
	return ia64_rse_slot_num(addr) == 0x3f;
}

/*
 * Returns the address of the RNAT slot that covers the slot at
 * address SLOT_ADDR.
 */
static __inline__ unsigned long *
ia64_rse_rnat_addr (unsigned long *slot_addr)
{
	return (unsigned long *) ((unsigned long) slot_addr | (0x3f << 3));
}

/*
 * Calculate the number of registers in the dirty partition starting at BSPSTORE and
 * ending at BSP.  This isn't simply (BSP-BSPSTORE)/8 because every 64th slot stores
 * ar.rnat.
 */
static __inline__ unsigned long
ia64_rse_num_regs (unsigned long *bspstore, unsigned long *bsp)
{
	unsigned long slots = (bsp - bspstore);

	return slots - (ia64_rse_slot_num(bspstore) + slots)/0x40;
}

/*
 * The inverse of the above: given bspstore and the number of
 * registers, calculate ar.bsp.
 */
static __inline__ unsigned long *
ia64_rse_skip_regs (unsigned long *addr, long num_regs)
{
	long delta = ia64_rse_slot_num(addr) + num_regs;

	if (num_regs < 0)
		delta -= 0x3e;
	return addr + num_regs + delta/0x3f;
}

#endif /* _ASM_IA64_RSE_H */
