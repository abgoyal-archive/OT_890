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
 * arch/arm/kernel/thumbee.c
 *
 * Copyright (C) 2008 ARM Limited
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/thread_notify.h>

/*
 * Access to the ThumbEE Handler Base register
 */
static inline unsigned long teehbr_read(void)
{
	unsigned long v;
	asm("mrc	p14, 6, %0, c1, c0, 0\n" : "=r" (v));
	return v;
}

static inline void teehbr_write(unsigned long v)
{
	asm("mcr	p14, 6, %0, c1, c0, 0\n" : : "r" (v));
}

static int thumbee_notifier(struct notifier_block *self, unsigned long cmd, void *t)
{
	struct thread_info *thread = t;

	switch (cmd) {
	case THREAD_NOTIFY_FLUSH:
		thread->thumbee_state = 0;
		break;
	case THREAD_NOTIFY_SWITCH:
		current_thread_info()->thumbee_state = teehbr_read();
		teehbr_write(thread->thumbee_state);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block thumbee_notifier_block = {
	.notifier_call	= thumbee_notifier,
};

static int __init thumbee_init(void)
{
	unsigned long pfr0;
	unsigned int cpu_arch = cpu_architecture();

	if (cpu_arch < CPU_ARCH_ARMv7)
		return 0;

	/* processor feature register 0 */
	asm("mrc	p15, 0, %0, c0, c1, 0\n" : "=r" (pfr0));
	if ((pfr0 & 0x0000f000) != 0x00001000)
		return 0;

	printk(KERN_INFO "ThumbEE CPU extension supported.\n");
	elf_hwcap |= HWCAP_THUMBEE;
	thread_register_notifier(&thumbee_notifier_block);

	return 0;
}

late_initcall(thumbee_init);
