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
 *  linux/arch/h8300/mm/fault.c
 *
 *  Copyright (C) 1998  D. Jeff Dionne <jeff@lineo.ca>,
 *  Copyright (C) 2000  Lineo, Inc.  (www.lineo.com) 
 *
 *  Based on:
 *
 *  linux/arch/m68knommu/mm/fault.c
 *  linux/arch/m68k/mm/fault.c
 *
 *  Copyright (C) 1995  Hamish Macdonald
 */

#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/ptrace.h>

#include <asm/system.h>
#include <asm/pgtable.h>

/*
 * This routine handles page faults.  It determines the problem, and
 * then passes it off to one of the appropriate routines.
 *
 * error_code:
 *	bit 0 == 0 means no page found, 1 means protection fault
 *	bit 1 == 0 means read, 1 means write
 *
 * If this routine detects a bad access, it returns 1, otherwise it
 * returns 0.
 */
asmlinkage int do_page_fault(struct pt_regs *regs, unsigned long address,
			      unsigned long error_code)
{
#ifdef DEBUG
	printk ("regs->sr=%#x, regs->pc=%#lx, address=%#lx, %ld\n",
		regs->sr, regs->pc, address, error_code);
#endif

/*
 * Oops. The kernel tried to access some bad page. We'll have to
 * terminate things with extreme prejudice.
 */
	if ((unsigned long) address < PAGE_SIZE) {
		printk(KERN_ALERT "Unable to handle kernel NULL pointer dereference");
	} else
		printk(KERN_ALERT "Unable to handle kernel access");
	printk(" at virtual address %08lx\n",address);
	if (!user_mode(regs))
		die("Oops", regs, error_code);
	do_exit(SIGKILL);

	return 1;
}

